#include "MMesh.h"
#include "MANA3DEngine.h"
using namespace MANA3D;

#include <glad\glad.h>
#include <algorithm>

using std::string;
using std::vector;
using std::map;
using std::list;
using namespace glm;

#define ARRAY_SIZE_IN_ELEMENTS( a ) ( sizeof(a) / sizeof( *( a ) ) )


//void MVertexBoneData::AddBoneData( uint BoneID, float Weight )
//{
//    for ( uint i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS( IDs ) ; i++ ) 
//	{
//        if ( Weights[i] == 0.0 ) 
//		{
//            IDs[i] = BoneID;
//            Weights[i] = Weight;
//            return;
//        }
//    }
//
//    // should never get here - more bones than we have space for
//    assert( 0 );
//} 



MMesh::MMesh( const vector<MVertex>& vertices, const vector<unsigned int>& indices, const MString& meshName, const bool& isSkinned = false )
{
	this->vertices = vertices;
	this->indices = indices;
	this->name = meshName;

	this->vertexCount = vertices.size();
	this->indexCount = indices.size();
	this->triangleCount = indexCount / 3;

	glGenVertexArrays( 1, &VAO );
	glBindVertexArray( VAO );

	glGenBuffers( 1, &VBO );
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof( MVertex ), &vertices[0], GL_STATIC_DRAW );

	glGenBuffers( 1, &EBO );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof( unsigned int ), &indices[0], GL_STATIC_DRAW );

	// vertex positions
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( MVertex ), (void*)0 );
	// vertex normals
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( MVertex ), (void*)offsetof( MVertex, normal ) );
	// vertex texture coords
	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof( MVertex ), (void*)offsetof( MVertex, texCoords ) );
	// vertex tangent
	glEnableVertexAttribArray( 3 );
	glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, sizeof( MVertex ), (void*)offsetof( MVertex, tangent ) );
	// vertex bitangent
	glEnableVertexAttribArray( 4 );
	glVertexAttribPointer( 4, 3, GL_FLOAT, GL_FALSE, sizeof( MVertex ), (void*)offsetof( MVertex, bitangent ) );

	if ( !isSkinned )
		glBindVertexArray( 0 );
}

MMesh::~MMesh()
{
	// Optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays( 1, &VAO );
	glDeleteBuffers( 1, &VBO );
	glDeleteBuffers( 1, &EBO );
}



const vector<MVertex>& MMesh::GetVertices()
{
	return this->vertices;
}
const vector<unsigned int>& MMesh::GetIndices()
{
	return this->indices;
}



const unsigned int& MMesh::GetIndexCount()
{
	return this->indexCount;
}

const unsigned int& MMesh::GetVertexCount()
{
	return this->vertexCount;
}

const unsigned int& MMesh::GetTriangleCount()
{
	return this->triangleCount;
}

const unsigned int& MMesh::GetVAO()
{
	return this->VAO;
}

const MString& MMesh::GetName()
{
	return this->name;
}






#pragma region

map<string, MMesh*> MMesh::privateMeshesMap;
map<string, MMesh*> MMesh::scenePublicMeshesMap;
map<string, MMesh*> MMesh::runtimePublicMeshesMap;
list<string> MMesh::publicMeshNamesList;


MMesh* MMesh::Create_Internal( const vector<MVertex>& vertices, const vector<unsigned int>& indices, const MString& meshName, map<string, MMesh*>& container )
{
	// Check if we already have a mesh with this name?
	int num = 1;
	string newName( meshName );
	while ( true )
	{
		map<string, MMesh*>::iterator it = container.find( newName );
		if ( it != container.end() )
		{
			num++;
			newName = MString( "%s%i", newName.c_str(), num );
		}
		else
			break;
	}

	MMesh *mesh = new MMesh( vertices, indices, newName );
	container.insert( std::pair<string, MMesh*>( newName, mesh ) );
	return mesh;
}

MMesh* MMesh::Create( const vector<MVertex>& vertices, const vector<unsigned int>& indices, const MString& meshName )
{
	bool isEditor = MANA3DEngine::GetIsEditorLoop();
	MMesh* mesh = Create_Internal( vertices, indices, meshName, isEditor ? scenePublicMeshesMap : runtimePublicMeshesMap );
	if ( isEditor )
		publicMeshNamesList.push_back( meshName.GetStr() );
	return mesh;
}


void MMesh::Destroy_Internal( MMesh* mesh, map<string, MMesh*>& container )
{
	if ( container == scenePublicMeshesMap )
		publicMeshNamesList.remove( mesh->GetName().GetStr() );

	for ( map<string, MMesh*>::iterator it = container.begin(); it != container.end(); ++it )
	{
		if ( it->second == mesh )
		{
			// Remove all references to this pointer before erasing it.
			OnDestroyMeshCallbackfunc( mesh );

			container.erase( it );
			break;
		}
	}
	delete mesh;
}

void MMesh::DestroyAll()
{
	while ( !privateMeshesMap.empty() )
	{
		MMesh *temp = privateMeshesMap.begin()->second;
		privateMeshesMap.erase( privateMeshesMap.begin() );
		delete temp;
	}
	privateMeshesMap.clear();

	while ( !scenePublicMeshesMap.empty() )
	{
		MMesh *temp = scenePublicMeshesMap.begin()->second;
		scenePublicMeshesMap.erase( scenePublicMeshesMap.begin() );
		delete temp;
	}
	scenePublicMeshesMap.clear();

	while ( !runtimePublicMeshesMap.empty() )
	{
		MMesh *temp = runtimePublicMeshesMap.begin()->second;
		runtimePublicMeshesMap.erase( runtimePublicMeshesMap.begin() );
		delete temp;
	}
	runtimePublicMeshesMap.clear();

	publicMeshNamesList.clear();
}

void MMesh::Delete( MMesh *mesh )
{
	Destroy_Internal( mesh, MANA3DEngine::GetIsEditorLoop() ? scenePublicMeshesMap : runtimePublicMeshesMap );
}

MMesh* MMesh::GetPrivate( const MString& name )
{
	map<string, MMesh*>::iterator it = privateMeshesMap.find( name.GetStr() );
	if ( it != privateMeshesMap.end() )
		return it->second;
	else
		return nullptr;
}

MMesh* MMesh::Get( const MString& name )
{
	map<string, MMesh*> tMap = MANA3DEngine::GetIsEditorLoop() ? scenePublicMeshesMap : runtimePublicMeshesMap;
	map<string, MMesh*>::iterator it = tMap.find( name.GetStr() );
	if ( it != tMap.end() )
		return it->second;
	else
		return nullptr;
}

list<string>& MMesh::GetAllMeshNamesList()
{
	return publicMeshNamesList;
}

map<string, MMesh*>& MMesh::GetAllMeshes()
{
	return MANA3DEngine::GetIsEditorLoop() ? scenePublicMeshesMap : runtimePublicMeshesMap;
}


void MMesh::CreatePrivateMeshes()
{
	CreateMainGridMesh();
	CreateCrossGridMesh();
}
void MMesh::CreateMainGridMesh()
{
	vector<MVertex> vertices;
	vector<unsigned int> indices;

	int totalRow = 50;
	int totalCol = 51;

	float startX = -25; // totalRow * -0.5f;
	float startZ = -25; // totalCol * -0.5f;

	for ( int iZ = 0; iZ <= totalRow; iZ++ )
	{
		startX = -25;// totalRow * -0.5f;
		for ( int i = 0; i < totalCol; i++ )
		{
			MVertex v0;
			v0.position = vec3( startX, 0.0f, startZ );
			v0.normal = vec3( 0.0f, 0.0f, -1.0f );
			v0.texCoords = vec2( 1.0f, 1.0f );
			vertices.push_back( v0 );

			if ( i > 0 )
			{
				indices.push_back( ( iZ * totalCol ) + i - 1 );
				indices.push_back( ( iZ * totalCol ) + i );
			}

			if ( iZ > 0 )
			{
				indices.push_back( ( iZ * totalCol ) + i );
				indices.push_back( ( iZ * totalCol ) + i - totalCol );
			}

			startX++;
		}
		 
		startZ++;
	}

	MMesh::Create_Internal( vertices, indices, "Grid", privateMeshesMap );
}
void MMesh::CreateCrossGridMesh()
{
	vector<MVertex> vertices;
	vector<unsigned int> indices;


	MVertex v0;
	v0.position = vec3( 0.0f, 0.0f, 25.0f );
	v0.normal = vec3( 0.0f, 0.0f, -1.0f );
	v0.texCoords = vec2( 1.0f, 1.0f );
	vertices.push_back( v0 );

	MVertex v1;
	v1.position = vec3( 0.0f, 0.0f, -25.0f );
	v1.normal = vec3( 0.0f, 0.0f, -1.0f );
	v1.texCoords = vec2( 1.0f, 1.0f );
	vertices.push_back( v1 );


	indices.push_back( 0 );
	indices.push_back( 1 );


	MVertex v2;
	v2.position = vec3( 25.0f, 0.0f, 0.0f );
	v2.normal = vec3( 0.0f, 0.0f, -1.0f );
	v2.texCoords = vec2( 1.0f, 1.0f );
	vertices.push_back( v2 );

	MVertex v3;
	v3.position = vec3( -25.0f, 0.0f, 0.0f );
	v3.normal = vec3( 0.0f, 0.0f, -1.0f );
	v3.texCoords = vec2( 1.0f, 1.0f );
	vertices.push_back( v3 );


	indices.push_back( 2 );
	indices.push_back( 3 );

	MMesh::Create_Internal( vertices, indices, "CrossGrid", privateMeshesMap );
}


void ( *MMesh::OnDestroyMeshCallbackfunc )( MMesh* mesh );
void MMesh::SetOnDestroyMeshCallbackfunc( void ( *callbackFunc )( MMesh* mesh ) )
{
	OnDestroyMeshCallbackfunc = callbackFunc;
}

#pragma endregion [Static Functions]
