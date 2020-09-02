#include "MSkinnedMesh.h"
#include "Skeleton.h"
using namespace MANA3D;

#include <glad\glad.h>
using std::map;
using std::list;
using std::vector;
using std::string;

MSkinnedMesh::MSkinnedMesh( const vector<MVertex>& vertices, const vector<unsigned int>& indices, const MString& meshName ) : MMesh( vertices, indices, meshName, true )
{
	// Joint ID
	glEnableVertexAttribArray( 5 );
	glVertexAttribIPointer( 5, NUM_BONES_PER_VEREX, GL_INT, sizeof( MVertex ), (void*)offsetof( MVertex, jointIds ) );

	// Joint weight.
	glEnableVertexAttribArray( 6 );
	glVertexAttribPointer( 6, NUM_BONES_PER_VEREX, GL_FLOAT, GL_FALSE, sizeof( MVertex ), (void*)offsetof( MVertex, jointWeights ) );


	glBindVertexArray( 0 );
}

MSkinnedMesh::~MSkinnedMesh()
{
	MMesh::~MMesh();
}









map<string, MSkinnedMesh*> MSkinnedMesh::allSkinnedMeshesMap;
list<string> MSkinnedMesh::allSkinnedMeshNamesList;

MSkinnedMesh* MSkinnedMesh::Create( const vector<MVertex>& vertices, const vector<unsigned int>& indices, const MString& meshName )
{
	// Check if we already have a mesh with this name?
	int num = 1;
	string newName( meshName );
	while ( true )
	{
		map<string, MSkinnedMesh*>::iterator it = allSkinnedMeshesMap.find( newName );
		if ( it != allSkinnedMeshesMap.end() )
		{
			num++;
			newName = MString( "%s%i", newName.c_str(), num );
		}
		else
			break;
	}

	MSkinnedMesh*mesh = new MSkinnedMesh( vertices, indices, newName );
	allSkinnedMeshesMap.insert( std::pair<string, MSkinnedMesh*>( newName, mesh ) );
	allSkinnedMeshNamesList.push_back( newName );
	return mesh;
}


void MSkinnedMesh::Destroy( MSkinnedMesh *mesh )
{
	for ( map<string, MSkinnedMesh*>::iterator it = allSkinnedMeshesMap.begin(); it != allSkinnedMeshesMap.end(); ++it )
	{
		if ( it->second == mesh )
		{
			allSkinnedMeshesMap.erase( it );
			break;
		}
	}

	delete mesh;
}

void MSkinnedMesh::DestroyAll()
{
	while ( !allSkinnedMeshesMap.empty() )
	{
		MSkinnedMesh*temp = allSkinnedMeshesMap.begin()->second;
		allSkinnedMeshesMap.erase( allSkinnedMeshesMap.begin() );
		delete temp;
	}

	allSkinnedMeshesMap.clear();
}



MSkinnedMesh* MSkinnedMesh::Get( const MString& name )
{
	map<string, MSkinnedMesh*>::iterator it = allSkinnedMeshesMap.find( name.GetCStr() );
	if ( it != allSkinnedMeshesMap.end() )
		return it->second;
	else
		return nullptr;
}

list<string> MSkinnedMesh::GetAllSkinnedMeshNamesList()
{
	return allSkinnedMeshNamesList;
}