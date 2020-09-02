#include "MShader.h"
#include "Debug.h"
#include "Application.h"
#include "MANA3DEngine.h"
using namespace MANA3D;

#include <GL3W/gl3w.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using std::map;
using std::list;
using std::string;
using std::vector;



MShader::MShader( const MString& vertexPath, const MString& fragmentPath, const MString& name )
{
	this->name = name;

	// 1. retrieve the vertex/fragment source code from filePath
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
	fShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
	
	try
	{
		// open files
		vShaderFile.open( vertexPath.GetCStr() );
		fShaderFile.open( fragmentPath.GetCStr() );
		std::stringstream vShaderStream, fShaderStream;

		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();


		/*const char* samplerStr = "uniform sampler2D";
		size_t pos = fragmentCode.find( samplerStr, 0 );
		while ( pos != string::npos )
		{
			int startIndex = pos + 17;
			int endIndex = fragmentCode.find( ";", startIndex +1 );
			if ( endIndex != string::npos )
			{
				MString sName = MString::GetSubStr( fragmentCode, startIndex, endIndex - startIndex );
				sName.Trim();
				sampler2DNames.push_back( sName );
			}
			pos = fragmentCode.find( samplerStr, endIndex+1 );
		}*/
	}
	catch ( std::ifstream::failure e )
	{
		Debug::LogError( MString( "Failed to read shader %s %s", name.GetCStr(), e.what() ) );
	}
	
	const char* vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();
	
	// 2. compile shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];
	
	// vertex shader
	vertex = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vertex, 1, &vShaderCode, nullptr );
	glCompileShader( vertex );
	CheckCompileErrors( vertex, "VERTEX" );
	
	// fragment Shader
	fragment = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fragment, 1, &fShaderCode, nullptr );
	glCompileShader( fragment );
	CheckCompileErrors( fragment, "FRAGMENT" );
	
	// shader Program
	id = glCreateProgram();
	glAttachShader( id, vertex );
	glAttachShader( id, fragment );
	glLinkProgram( id );
	CheckCompileErrors( id, "PROGRAM" );
	
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader( vertex );
	glDeleteShader( fragment );





	GLint uniformIndex;
	GLint activeUniformsCount;
	GLint uniformSize; // size of the variable
	GLenum uniformType; // type of the variable (float, vec3 or mat4, etc)
	const GLsizei bufSize = 24; // maximum name length
	GLchar nameBuf[bufSize]; // variable name in GLSL
	GLsizei length; // name length
	glGetProgramiv( id, GL_ACTIVE_UNIFORMS, &activeUniformsCount ); //GL_ACTIVE_ATTRIBUTES   // GL_ACTIVE_UNIFORMS

	int texIndex = 0;

	//printf( "Active Uniforms: %d\n", count );
	for ( uniformIndex = 0; uniformIndex < activeUniformsCount; uniformIndex++ )
	{
		glGetActiveUniform( id, (GLuint)uniformIndex, bufSize, &length, &uniformSize, &uniformType, nameBuf );
		//glGetActiveAttrib( id, (GLuint)uniformIndex, bufSize, &length, &uniformSize, &uniformType, nameBuf );
		//printf( "Uniform #%d Type: %u Name: %s\n", uniformIndex, uniformType, nameBuf );

		/*string uniName( nameBuf );
		if ( uniName == "viewPos" ||
			 uniName == "gammaCorrection" ||
			 uniName == "directionalLightsCount" ||
			 uniName == "pointLightsCount" ||
			 uniName == "spotLightsCount" ||
			 uniName == "dirLights" || 
			 uniName == "pointLights" ||
			 uniName == "spotLights" ||
			 uniName.find( "[" ) != string::npos )
			continue;*/


		MUniformType utype = (MUniformType)uniformType;

		if ( uniformType != (GLenum)MUniformType::MUT_uMat4 &&
			 uniformType != (GLenum)MUniformType::MUT_uSampler2D &&
			 uniformType != (GLenum)MUniformType::MUT_uCubemap &&
			 uniformType != (GLenum)MUniformType::MUT_uFloat &&
			 uniformType != (GLenum)MUniformType::MUT_uInt &&
			 uniformType != (GLenum)MUniformType::MUT_uVec3 &&
			 uniformType != (GLenum)MUniformType::MUT_uVec4 &&
			 uniformType != (GLenum)MUniformType::MUT_uBool )
			continue;

		MUniformData uData;
		uData.locationID = uniformIndex;
		uData.type = utype;

		if ( uniformType == (GLenum)MUniformType::MUT_uSampler2D ||
			 uniformType == (GLenum)MUniformType::MUT_uCubemap )
		{
			// NEED Some review??????
			uData.texIndex = texIndex;
			texIndex++;
		}

		uniformsMap.insert( std::pair<string, MUniformData>( nameBuf, uData ) );
	}

	//PrintLog();
}

MShader::~MShader()
{

}

void MShader::CheckCompileErrors( unsigned int shader, MString type )
{
	int success;
	char infoLog[1024];
	if ( type != "PROGRAM" )
	{
		glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
		if ( !success )
		{
			glGetShaderInfoLog( shader, 1024, nullptr, infoLog );
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv( shader, GL_LINK_STATUS, &success );
		if ( !success )
		{
			glGetProgramInfoLog( shader, 1024, nullptr, infoLog );
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

void MShader::PrintLog()
{
	printf( "*********************************************\n" );
	printf( "%s\n", name.GetCStr() );
	printf("*********************************************\n");

	for ( map<string, MUniformData>::iterator it = uniformsMap.begin(); it != uniformsMap.end(); ++it )
	{
		string typeStr;
		switch (it->second.type)
		{
		case MUniformType::MUT_uSampler2D:
			typeStr = "uSampler2D";
			break;
		case MUniformType::MUT_uCubemap:
			typeStr = "uCubemap";
			break;
		case MUniformType::MUT_uFloat:
			typeStr = "uFloat";
			break;
		case MUniformType::MUT_uInt:
			typeStr = "uInt";
			break;
		case MUniformType::MUT_uVec3:
			typeStr = "uVec3";
			break;
		}

		int getInde = glGetUniformLocation( id, it->first.c_str() );
		printf( "%i\n", getInde );
		printf( "%s  -> \n\tlocation: %i \n\ttype: %s\n", it->first.c_str(), it->second.locationID, typeStr.c_str() );
	}

	printf( "\n\n\n\n" );
}


void MShader::Use()
{
	// Activate the shader
	glUseProgram( id );
}



int MShader::GetUniformLocation( const MString& uniformName )
{
	map<string, MUniformData>::iterator it = uniformsMap.find( uniformName.GetStr() );
	if ( it != uniformsMap.end() )
		return it->second.locationID;
	else
		return -1;
}

void MShader::SetBool( const MString& uniName, bool value )
{
	//glUniform1i( glGetUniformLocation( id, name.GetCStr() ), (int)value );
	glUniform1i( GetUniformLocation( uniName ), (int)value );
}
void MShader::SetInt( const MString& uniName, int value )
{
	//glUniform1i( glGetUniformLocation( id, uniName.GetCStr() ), value );
	glUniform1i( GetUniformLocation( uniName ), value );
}
void MShader::SetFloat( const MString& uniName, float value )
{
	//glUniform1f( glGetUniformLocation( id, uniName.GetCStr() ), value );
	glUniform1f( GetUniformLocation( uniName ), value );
}
void MShader::SetDouble( const MString& uniName, double value )
{
	//glUniform1d( glGetUniformLocation( id, uniName.GetCStr() ), value );
	glUniform1d( GetUniformLocation( uniName ), value );
}
void MShader::SetVec2( const MString & uniName, const glm::vec2 &value )
{
	//glUniform2fv( glGetUniformLocation( id, uniName.GetCStr() ), 1, &value[0] );
	glUniform2fv( GetUniformLocation( uniName ), 1, &value[0] );
}
void MShader::SetVec2( const MString & uniName, float x, float y )
{
	//glUniform2f( glGetUniformLocation( id, uniName.GetCStr() ), x, y );
	glUniform2f( GetUniformLocation( uniName ), x, y );
}
void MShader::SetVec3( const MString & uniName, const glm::vec3 &value )
{
	//glUniform3fv( glGetUniformLocation( id, uniName.GetCStr() ), 1, &value[0] );
	glUniform3fv( GetUniformLocation( uniName ), 1, &value[0] );
}
void MShader::SetVec3( const MString & uniName, float x, float y, float z )
{
	//glUniform3f( glGetUniformLocation( id, uniName.GetCStr() ), x, y, z );
	glUniform3f( GetUniformLocation( uniName ), x, y, z );
}
void MShader::SetVec4( const MString & uniName, const glm::vec4 &value )
{
	//glUniform4fv( glGetUniformLocation( id, uniName.GetCStr() ), 1, &value[0] );
	glUniform4fv( GetUniformLocation( uniName ), 1, &value[0] );
}
void MShader::SetVec4( const MString & uniName, float x, float y, float z, float w )
{
	//glUniform4f( glGetUniformLocation( id, uniName.GetCStr() ), x, y, z, w );
	glUniform4f( GetUniformLocation( uniName ), x, y, z, w );
}
void MShader::SetMat2( const MString & uniName, const glm::mat2 &mat )
{
	//glUniformMatrix2fv( glGetUniformLocation( id, uniName.GetCStr() ), 1, GL_FALSE, &mat[0][0] );
	glUniformMatrix2fv( GetUniformLocation( uniName ), 1, GL_FALSE, &mat[0][0] );
}
void MShader::SetMat3( const MString & uniName, const glm::mat3 &mat )
{
	//glUniformMatrix3fv( glGetUniformLocation( id, uniName.GetCStr() ), 1, GL_FALSE, &mat[0][0] );
	glUniformMatrix3fv( GetUniformLocation( uniName ), 1, GL_FALSE, &mat[0][0] );
}
void MShader::SetMat4( const MString & uniName, glm::mat4 mat4 )
{
	//glUniformMatrix4fv( glGetUniformLocation( id, uniName.GetCStr() ), 1, GL_FALSE, glm::value_ptr( mat4 ) );
	glUniformMatrix4fv( GetUniformLocation( uniName ), 1, GL_FALSE, glm::value_ptr( mat4 ) );
}

void MShader::SetMat4Array( const MString & uniName, glm::mat4* mat4Array )
{
	glUniformMatrix4fv( glGetUniformLocation( id, uniName.GetCStr() ), 35, GL_FALSE, glm::value_ptr( mat4Array[0] ) );
}
void MShader::SetMat4Array( const MString & uniName, vector<glm::mat4> mat4Array )
{
	glUniformMatrix4fv( glGetUniformLocation( id, uniName.GetCStr() ), mat4Array.size(), GL_FALSE, glm::value_ptr( mat4Array[0] ) );
}




unsigned int MShader::GetID()
{
	return id;
}

MString MShader::GetName()
{
	return name;
}

bool MShader::HasUniform( MString uniformName )
{
	return vertexCode.find( uniformName );
}


map<string, MUniformData> MShader::GetUniformsMap()
{
	return uniformsMap;
}



#pragma region

map<string, MShader*> MShader::privateShadersMap;
map<string, MShader*> MShader::scenePublicShadersMap;
map<string, MShader*> MShader::runtimePublicShadersMap;
list<string> MShader::publicShaderNamesList;
//MShader* MShader::defaultShader;


MShader* MShader::Create_Internal( const MString& vertexPath, const MString& fragmentPath, const MString& name, map<string, MShader*>& container )
{
	// Check if we already have a shader with this name?
	int num = 1;
	string newName( name );
	while ( true )
	{
		map<string, MShader*>::iterator it = container.find( newName );
		if ( it != container.end() )
		{
			num++;
			newName = MString( "%s%i", newName.c_str(), num );
		}
		else
			break;
	}

	MShader *shader = new MShader( vertexPath, fragmentPath, newName );
	container.insert( std::pair<string, MShader*>( newName, shader ) );
	return shader;
}

MShader* MShader::Create( const MString& vertexPath, const MString& fragmentPath, const MString& name )
{
	bool isEditor = MANA3DEngine::GetIsEditorLoop();
	MShader* shader = Create_Internal( vertexPath, fragmentPath, name, isEditor ? scenePublicShadersMap : runtimePublicShadersMap );
	if ( isEditor )
		publicShaderNamesList.push_back( name.GetStr() );
	return shader;
}


void MShader::Destroy_Internal( MShader* shader, map<string, MShader*>& container )
{
	if ( container == scenePublicShadersMap )
		publicShaderNamesList.remove( shader->GetName().GetStr() );

	for ( map<string, MShader*>::iterator it = container.begin(); it != container.end(); ++it )
	{
		if ( it->second == shader )
		{
			container.erase( it );
			break;
		}
	}

	delete shader;
}

//void MShader::Delete( MShader *shader )
//{
//	Destroy_Internal( shader, MANA3DEngine::GetIsEditorLoop() ? scenePublicShadersMap : runtimePublicShadersMap );
//}

void MShader::DestroyAll()
{
	while ( !privateShadersMap.empty() )
	{
		MShader *temp = ( *privateShadersMap.begin() ).second;
		privateShadersMap.erase( privateShadersMap.begin() );
		delete temp;
	}
	privateShadersMap.clear();

	while ( !scenePublicShadersMap.empty() )
	{
		MShader *temp = ( *scenePublicShadersMap.begin() ).second;
		scenePublicShadersMap.erase( scenePublicShadersMap.begin() );
		delete temp;
	}
	scenePublicShadersMap.clear();

	while ( !runtimePublicShadersMap.empty() )
	{
		MShader *temp = ( *runtimePublicShadersMap.begin() ).second;
		runtimePublicShadersMap.erase( runtimePublicShadersMap.begin() );
		delete temp;
	}
	runtimePublicShadersMap.clear();

	publicShaderNamesList.clear();
}


list<string> MShader::GetAllShaderNamesList()
{
	return publicShaderNamesList;
}


//MShader* MShader::GetDefault()
//{
//	if ( defaultShader == nullptr )
//		defaultShader = Get( "Diffuse" );//Specular
//
//	return defaultShader;
//}


MShader* MShader::GetPrivate( const MString& name )
{
	map<string, MShader*>::iterator it = privateShadersMap.find( name.GetStr() );
	if ( it != privateShadersMap.end() )
		return it->second;
	else
		return nullptr;
}

MShader* MShader::Get( const MString& name )
{
	map<string, MShader*>::iterator it = runtimePublicShadersMap.find( name.GetStr() );
	if ( it != runtimePublicShadersMap.end() )
		return it->second;
	else
	{
		it = scenePublicShadersMap.find( name.GetStr() );
		if ( it != scenePublicShadersMap.end() )
			return it->second;
	}

	return nullptr;
	//MShader* shader = runtimePublicShadersMap[name.GetStr()];
	//if ( shader == nullptr )
	//	shader = scenePublicShadersMap[name.GetStr()];

	////return MANA3DEngine::GetIsEditorLoop() ? scenePublicShadersMap[name.GetStr()] : runtimePublicShadersMap[name.GetStr()];
	//return shader;
}



void MShader::GetFiles( MString subFolder, MString& shadersDir, vector<MString>& vertexShaders, vector<MString>& fragmentShaders )
{
	shadersDir = MString( "%s%s", Application::GetAppDirctory().GetCStr(), subFolder.GetCStr() );
	vector<MString> allFiles = Application::GetFileNames( shadersDir );

	for ( vector<MString>::iterator it = allFiles.begin(); it != allFiles.end(); ++it )
	{
		string str = *it;
		str = str.substr( str.rfind( "." ) + 1 );
		
		if ( str == "vert" )
			vertexShaders.push_back( MString::EraseSubStr( (*it), ".vert" ) );
		else if ( str == "frag" )
			fragmentShaders.push_back( MString::EraseSubStr( ( *it ), ".frag" ) );
	}
}
void MShader::LoadShadersFolder()
{
	vector<MString> vertexShaders;
	vector<MString> fragmentShaders;
	MString shadersDir;

	GetFiles( "/Shaders/Public/", shadersDir, vertexShaders, fragmentShaders );

	// Vertex shaders & fragment shaders must be equal.
	assert( vertexShaders.size() == fragmentShaders.size() );

	for ( size_t i = 0; i < vertexShaders.size(); i++ )
	{
		if ( vertexShaders.at( i ) == fragmentShaders.at( i ) )
		{
			MShader* shader = MShader::Create( 
				MString( "%s%s.vert", shadersDir.GetCStr(), vertexShaders.at( i ).GetCStr() ), 
				MString( "%s%s.frag", shadersDir.GetCStr(), fragmentShaders.at( i ).GetCStr() ),
				vertexShaders.at( i ) );
		}
	}

	vector<MString> privateVertexShaders;
	vector<MString> privateFragmentShaders;

	GetFiles( "/Shaders/Private/", shadersDir, privateVertexShaders, privateFragmentShaders );

	// Vertex shaders & fragment shaders must be equal.
	assert( privateVertexShaders.size() == privateFragmentShaders.size() );

	for ( size_t i = 0; i < privateVertexShaders.size(); i++ )
	{
		if ( privateVertexShaders.at( i ) == privateFragmentShaders.at( i ) )
		{
			MShader* shader = MShader::Create_Internal( 
				MString( "%s%s.vert", shadersDir.GetCStr(), privateVertexShaders.at( i ).GetCStr() ),
				MString( "%s%s.frag", shadersDir.GetCStr(), privateFragmentShaders.at( i ).GetCStr() ),
				privateVertexShaders.at( i ),
				privateShadersMap );
		}
	}
}


void MShader::SetUniformBlocks( glm::mat4 projection, glm::mat4 view )
{
	//// Set the uniform block of the vertex shaders equal to binding point 0.
	//for ( map<string, MShader*>::iterator it = allShaders.begin(); it != allShaders.end(); ++it )
	//{
	//	unsigned int shaderID = it->second->GetID();
	//	unsigned int uniformBlockIndex = glGetUniformBlockIndex( shaderID, "Matrices" );
	//	if ( uniformBlockIndex >= 0 )
	//		glUniformBlockBinding( shaderID, uniformBlockIndex, 0 );
	//}

	// Create the actual uniform buffer object and also bind the buffer to binding point 0.
	unsigned int uboMatrices;
	glGenBuffers( 1, &uboMatrices );
	glBindBuffer( GL_UNIFORM_BUFFER, uboMatrices );
	glBufferData( GL_UNIFORM_BUFFER, 2 * sizeof( glm::mat4 ), nullptr, GL_STATIC_DRAW );
	glBindBuffer( GL_UNIFORM_BUFFER, 0 );
	glBindBufferRange( GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof( glm::mat4 ) );

	// Fill the buffer with projection matrix.
	glBindBuffer( GL_UNIFORM_BUFFER, uboMatrices );
	glBufferSubData( GL_UNIFORM_BUFFER, 0, sizeof( glm::mat4 ), glm::value_ptr( projection ) );
	glBindBuffer( GL_UNIFORM_BUFFER, 0 );
	// Fill the buffer with view matrix.
	glBindBuffer( GL_UNIFORM_BUFFER, uboMatrices );
	glBufferSubData( GL_UNIFORM_BUFFER, sizeof( glm::mat4 ), sizeof( glm::mat4 ), glm::value_ptr( view ) );
	glBindBuffer( GL_UNIFORM_BUFFER, 0 ); 
}


#pragma endregion [Static Functions]