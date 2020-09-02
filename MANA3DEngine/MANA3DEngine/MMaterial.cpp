#include "MMaterial.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Debug.h"
#include "MANA3DEngine.h"
using namespace MANA3D;

#include <GL3W/gl3w.h>
#include <algorithm>

using std::map;
using std::list;
using std::vector;
using std::string;

using namespace glm;



MMaterial::MMaterial( MShader* shader, const MString& name )
{
	this->name = name;
	SetShader( shader );
}

MMaterial::~MMaterial()
{
}


void MMaterial::SetShader( MShader* shader )
{
	this->shader = shader;

	// Make temp copy of texturesMap.
	map<string, MTextureIndexPair> textureIndexPairsMapTemp;
	for ( map<string, MTextureIndexPair>::iterator it = textureIndexPairsMap.begin(); it != textureIndexPairsMap.end(); ++it )
		textureIndexPairsMapTemp.insert( std::pair<string, MTextureIndexPair>( it->first, it->second ) );
	// Now clear texturesMap.
	textureIndexPairsMap.clear();

	// Make temp copy of cubemapsMap.
	map<string, MCubemapIndexPair> cubemapIndexPairsMapTemp;
	for ( map<string, MCubemapIndexPair>::iterator it = cubemapIndexPairsMap.begin(); it != cubemapIndexPairsMap.end(); ++it )
		cubemapIndexPairsMapTemp.insert( std::pair<string, MCubemapIndexPair>( it->first, it->second ) );
	// Now clear cubemapsMap.
	cubemapIndexPairsMap.clear();

	// Make temp copy of floatsMap.
	map<string, float> floatsMapTemp;
	for ( map<string, float>::iterator it = floatsMap.begin(); it != floatsMap.end(); ++it )
		floatsMapTemp.insert( std::pair<string, float>( it->first, it->second ) );
	// Now clear floatsMap.
	floatsMap.clear();

	// Make temp copy of intsMap
	map<string, int> intsMapTemp;
	for ( map<string, int>::iterator it = intsMap.begin(); it != intsMap.end(); ++it )
		intsMapTemp.insert( std::pair<string, int>( it->first, it->second ) );
	// Now clear intsMap.
	intsMap.clear();

	// Make temp copy of vec3sMap
	map<string, vec3> vec3sMapTemp;
	for ( map<string, vec3>::iterator it = vec3sMap.begin(); it != vec3sMap.end(); ++it )
		vec3sMapTemp.insert( std::pair<string, vec3>( it->first, it->second ) );
	// Now clear vec3sMap.
	vec3sMap.clear();

	map<string, MUniformData> uniformsMap = shader->GetUniformsMap();
	for ( map<string, MUniformData>::iterator it = uniformsMap.begin(); it != uniformsMap.end(); ++it )
	{
		MUniformType utype = it->second.type;
		
		switch ( utype )
		{
		case MUniformType::MUT_uSampler2D:
			textureIndexPairsMap.insert( std::pair<string, MTextureIndexPair>( it->first, textureIndexPairsMapTemp[it->first] ) );
			break;
		case MUniformType::MUT_uCubemap:
			cubemapIndexPairsMap.insert( std::pair<string, MCubemapIndexPair>( it->first, cubemapIndexPairsMapTemp[it->first] ) );
			break;
		case MUniformType::MUT_uFloat:
			floatsMap.insert( std::pair<string, float>( it->first, floatsMapTemp[it->first] ) );
			break;
		case MUniformType::MUT_uInt:
			intsMap.insert( std::pair<string, int>( it->first, intsMapTemp[it->first] ) );
			break;
		case MUniformType::MUT_uVec3:
			vec3sMap.insert( std::pair<string, vec3>( it->first, vec3sMapTemp[it->first] ) );
			break;
		}
	}

	textureIndexPairsMapTemp.clear();
	cubemapIndexPairsMapTemp.clear();
	floatsMapTemp.clear();
	intsMapTemp.clear();
	vec3sMapTemp.clear();
}

void MMaterial::SetTexture( const MString &uniName, MTexture* texture )
{
	map<string, MTextureIndexPair>::iterator it = textureIndexPairsMap.find( uniName.GetStr() );
	if ( it != textureIndexPairsMap.end() )
	{
		it->second.tex = texture;
		printf( "SetTexture %i\t\n", it->second.indexInShader );
		shader->Use();
		shader->SetInt( uniName, it->second.indexInShader );
	}
	else
		printf( "Fialed to find uniform %s in shader: %s for mat: %s\n", uniName.GetCStr(), shader->GetName().GetCStr(), name.GetCStr() );
}


void MMaterial::SetCubemap( const MString &uniName, MCubemap* cubemap )
{
	map<string, MCubemapIndexPair>::iterator it = cubemapIndexPairsMap.find( uniName.GetStr() );
	if ( it != cubemapIndexPairsMap.end() )
	{
		it->second.cubeMap = cubemap;

		shader->Use();
		shader->SetInt( uniName, it->second.indexInShader );
	}
}

void MMaterial::SetBool( const MString &name, bool value ) const
{
	shader->Use();
	shader->SetBool( name, value );
}
void MMaterial::SetInt( const MString &name, int value ) const
{
	shader->Use();
	shader->SetInt( name, value );
}
void MMaterial::SetFloat( const MString &name, float value ) const
{
	shader->Use();
	shader->SetFloat( name, value );
}
void MMaterial::SetDouble( const MString &name, double value ) const
{
	shader->Use();
	shader->SetDouble( name, value );
}
void MMaterial::SetVec2( const MString &name, const glm::vec2 &value ) const
{
	shader->Use();
	shader->SetVec2( name, value );
}
void MMaterial::SetVec2( const MString &name, float x, float y ) const
{
	shader->Use();
	shader->SetVec2( name, x, y );
}
void MMaterial::SetVec3( const MString &name, const glm::vec3 &value ) const
{
	shader->Use();
	shader->SetVec3( name, value );
}
void MMaterial::SetVec3( const MString &name, float x, float y, float z ) const
{
	shader->Use();
	shader->SetVec3( name, x, y, z );
}
void MMaterial::SetVec4( const MString &name, const glm::vec4 &value ) const
{
	shader->Use();
	shader->SetVec4( name, value );
}
void MMaterial::SetVec4( const MString &name, float x, float y, float z, float w )
{
	shader->Use();
	shader->SetVec4( name, x, y, z, w );
}
void MMaterial::SetMat2( const MString &name, const glm::mat2 &mat ) const
{
	shader->Use();
	shader->SetMat2( name, mat );
}
void MMaterial::SetMat3( const MString &name, const glm::mat3 &mat ) const
{
	shader->Use();
	shader->SetMat3( name, mat );
}
void MMaterial::SetMat4( const MString &name, glm::mat4 value ) const
{
	shader->Use();
	shader->SetMat4( name, value );
}

void MMaterial::SetMat4Array( const MString& name, mat4* mat4Array ) const
{
	shader->Use();
	shader->SetMat4Array( name, mat4Array );
}
void MMaterial::SetMat4Array( const MString& name, vector<glm::mat4> mat4Array ) const
{
	shader->Use();
	shader->SetMat4Array( name, mat4Array );
}

void MMaterial::BindMaterial()
{
	// Clear any binded texture.
	glBindTexture( GL_TEXTURE_2D, 0 );

	// Use current assigned shader for this material.
	shader->Use();

	// Bind textures.
	for ( map<string, MTextureIndexPair>::iterator it = textureIndexPairsMap.begin(); it != textureIndexPairsMap.end(); ++it )
	{
		if ( it->second.tex == nullptr )
			continue;

		glActiveTexture( GL_TEXTURE0 + it->second.indexInShader );
		glBindTexture( GL_TEXTURE_2D, it->second.tex->GetID() );
	}

	// Bind cubemap.
	for ( map<string, MCubemapIndexPair>::iterator it = cubemapIndexPairsMap.begin(); it != cubemapIndexPairsMap.end(); ++it )
	{
		if ( it->second.cubeMap == nullptr )
			continue;

		glActiveTexture( GL_TEXTURE0 + it->second.indexInShader );
		glBindTexture( GL_TEXTURE_CUBE_MAP, it->second.cubeMap->GetID() );
	}
}


MString MMaterial::GetName()
{
	return name;
}
MString MMaterial::GetShaderName()
{
	return shader->GetName();
}

map<string, MTextureIndexPair>& MMaterial::GetTextureIndexPairsMap()
{
	return textureIndexPairsMap;
}

map<string, MCubemapIndexPair>& MMaterial::GetCubemapIndexPairsMap()
{
	return cubemapIndexPairsMap;
}






#pragma region

map<string, MMaterial*> MMaterial::privateMaterialsMap;
map<string, MMaterial*> MMaterial::scenePublicMaterialsMap;
map<string, MMaterial*> MMaterial::runtimePublicMaterialsMap;
list<string> MMaterial::publicMaterialNames;


MMaterial* MMaterial::Create_Internal( MShader* shader, const MString& name, map<string, MMaterial*>& container )
{
	// Check if we already have a material with this name?
	int num = 1;
	string newName( name );
	while ( true )
	{
		map<string, MMaterial*>::iterator it = container.find( newName );
		if ( it != container.end() )
		{
			num++;
			newName = MString( "%s%i", newName.c_str(), num );
		}
		else
			break;
	}

	MMaterial *mat = new MMaterial( shader, newName );
	container.insert( std::pair<string, MMaterial*>( newName, mat ) );
	
	return mat;
}

MMaterial* MMaterial::Create( MShader* shader, const MString& name )
{
	bool isEditor = MANA3DEngine::GetIsEditorLoop();
	MMaterial* mat = Create_Internal( shader, name, isEditor ? scenePublicMaterialsMap : runtimePublicMaterialsMap );
	if ( isEditor )
		publicMaterialNames.push_back( mat->GetName().GetStr() );
	return mat;
}

void MMaterial::Destroy_Internal( MMaterial* mat, map<string, MMaterial*>& container )
{
	if ( container == scenePublicMaterialsMap )
		publicMaterialNames.remove( mat->GetName().GetStr() );

	// Only public matrial.
	for ( map<string, MMaterial*>::iterator it = container.begin(); it != container.end(); ++it )
	{
		if ( it->second == mat )
		{
			container.erase( it );
			break;
		}
	}
	delete mat;
}
void MMaterial::DestroyAll()
{
	while ( !privateMaterialsMap.empty() )
	{
		MMaterial *temp = privateMaterialsMap.begin()->second;
		privateMaterialsMap.erase( privateMaterialsMap.begin() );
		delete temp;
	}
	privateMaterialsMap.clear();

	while ( !scenePublicMaterialsMap.empty() )
	{
		MMaterial *temp = scenePublicMaterialsMap.begin()->second;
		scenePublicMaterialsMap.erase( scenePublicMaterialsMap.begin() );
		delete temp;
	}
	scenePublicMaterialsMap.clear();

	while ( !runtimePublicMaterialsMap.empty() )
	{
		MMaterial *temp = runtimePublicMaterialsMap.begin()->second;
		runtimePublicMaterialsMap.erase( runtimePublicMaterialsMap.begin() );
		delete temp;
	}
	runtimePublicMaterialsMap.clear();

	publicMaterialNames.clear();
}

void MMaterial::Delete( MMaterial* mat )
{
	Destroy_Internal( mat, MANA3DEngine::GetIsEditorLoop() ? scenePublicMaterialsMap : runtimePublicMaterialsMap );
}

void MMaterial::RemoveTextureReferences( MTexture* tex )
{
	map<string, MMaterial*>& matMap = MANA3DEngine::GetIsEditorLoop() ? scenePublicMaterialsMap : runtimePublicMaterialsMap;

	for ( map<string, MMaterial*>::iterator it = matMap.begin(); it != matMap.end(); ++it )
	{
		MMaterial* mat = it->second;
		map<string, MTextureIndexPair>& matTextureIndexPairs = mat->GetTextureIndexPairsMap();
		for ( map<string, MTextureIndexPair>::iterator it = matTextureIndexPairs.begin(); it != matTextureIndexPairs.end(); ++it )
		{
			if ( it->second.tex == nullptr )
				break;
			else if ( it->second.tex == tex )
			{
				//Debug::Log( MString( "Found a reference in %s", mat->GetName().GetCStr() ) );
				it->second.tex = nullptr;
			}
		}
	}
}



void MMaterial::InitPrivateMatrials( map<string, MTexture*>& privateTexturesMap )
{
	MMaterial::Create_Internal( MShader::Get( "Diffuse" ), "DefaultMat", privateMaterialsMap );
	MMaterial::Create_Internal( MShader::Get( "Reflection" ), "Reflection", privateMaterialsMap );
	MMaterial::Create_Internal( MShader::Get( "Refraction" ), "Refraction", privateMaterialsMap );

	MMaterial::Create_Internal( MShader::GetPrivate( "Editor3DGrid" ), "Editor3DGridMat", privateMaterialsMap );
	MMaterial::Create_Internal( MShader::GetPrivate( "EditorOutline" ),	"EditorOutlineMat", privateMaterialsMap );


	MShader* gizmoShader = MShader::GetPrivate( "Gizmo" );
	MMaterial* CameraGizmoMat = MMaterial::Create_Internal( gizmoShader, "CameraGizmoMat", privateMaterialsMap );
	CameraGizmoMat->SetTexture( "diffuseMap", privateTexturesMap.find( "Camera_Gizmo.png" )->second );

	MMaterial* PointLightGizmoMat = MMaterial::Create_Internal( gizmoShader, "PointLightGizmoMat", privateMaterialsMap );
	PointLightGizmoMat->SetTexture( "diffuseMap", privateTexturesMap.find( "PointLight_Gizmo.png" )->second );

	MMaterial* SpotLightGizmoMat = MMaterial::Create_Internal( gizmoShader,"SpotLightGizmoMat", privateMaterialsMap );
	SpotLightGizmoMat->SetTexture( "diffuseMap", privateTexturesMap.find( "SpotLight_Gizmo.png" )->second );

	MMaterial* DirectionalLightGizmoMat = MMaterial::Create_Internal( gizmoShader, "DirectionalLightGizmoMat", privateMaterialsMap );
	DirectionalLightGizmoMat->SetTexture( "diffuseMap", privateTexturesMap.find( "DirectionalLight_Gizmo.png" )->second );
}

MMaterial* MMaterial::GetPrivate( const MString& name )
{
	map<string, MMaterial*>::iterator it = privateMaterialsMap.find( name.GetStr() );
	if ( it != privateMaterialsMap.end() )
		return it->second;
	else
		return nullptr;
}

MMaterial* MMaterial::Get( const MString& name )
{
	const string str = name.GetStr();
	map<string, MMaterial*>& tMap = MANA3DEngine::GetIsEditorLoop() ? scenePublicMaterialsMap : runtimePublicMaterialsMap;
	map<string, MMaterial*>::iterator it = tMap.find( str );
	if ( it != tMap.end() )
		return it->second;
	else
		return nullptr;
}

list<string>& MMaterial::GetAllMaterialNamesList()
{
	return publicMaterialNames;
}

map<string, MMaterial*>& MMaterial::GetAllMaterials()
{
	return MANA3DEngine::GetIsEditorLoop() ? scenePublicMaterialsMap : runtimePublicMaterialsMap;
}

void MMaterial::SetMaterialShader( MMaterial* mat, MShader* shader )
{ 
	mat->SetShader( shader );
}
void MMaterial::SetMaterialShaderbyName( MMaterial* mat, const MString& shaderName )
{
	mat->SetShader( MShader::Get( shaderName ) );
}


void MMaterial::UpdateName( MMaterial* mat, const MString& newName )
{
	string matName = mat->GetName().GetStr();

	publicMaterialNames.remove( matName );

	int index = 0;
	map<string, MMaterial*>& mMap = GetAllMaterials();
	for ( map<string, MMaterial*>::iterator it = mMap.begin(); it != mMap.end(); ++it )
	{
		if ( it->second == mat )
		{
			mMap.erase( it );
			break;
		}
		index++;
	}
	mat->name = newName;
	mMap.insert( std::pair<string, MMaterial*>( newName.GetStr(), mat ) );
	list<string>::iterator it = publicMaterialNames.begin();
	publicMaterialNames.push_back( newName.GetStr() );
}

#pragma endregion [Static Functions]