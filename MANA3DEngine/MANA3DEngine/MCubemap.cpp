#include "MCubemap.h"
#include "Debug.h"
#include "MTexture.h"
using namespace MANA3D;

#include <GL3W/gl3w.h>

using std::list;
using std::map;
using std::string;



MCubemap::MCubemap( const MString& name, const MString& path, const unsigned int& id, MTexture* thumbTex )
{
	this->name = name;
	this->path = path;
	this->id = id;
	this->thumbTex = thumbTex;
}
MCubemap::~MCubemap()
{
	glDeleteTextures( 1, &id );
}


unsigned int MCubemap::GetID()
{
	return id;
}

const MString& MCubemap::GetName()
{
	return name;
}

const MString& MCubemap::GetPath()
{
	return path;
}

MTexture* MCubemap::GetThumbTex()
{
	return thumbTex;
}



// ****************** Statics ******************
map<string, MCubemap*> MCubemap::allCubmapsMap;
list<string> MCubemap::allCubmapsMapNamesList;


MCubemap* MCubemap::Create( const MString& name, const MString& path, const unsigned int& id, MTexture* thumbTex )
{
	// Check if we already have a cubemap with this name?
	int num = 1;
	string newName( name );
	while ( true )
	{
		map<string, MCubemap*>::iterator it = allCubmapsMap.find( newName );
		if ( it != allCubmapsMap.end() )
		{
			num++;
			newName = MString( "%s%i", newName.c_str(), num );
		}
		else
			break;
	}

	MCubemap *cubemap = new MCubemap( newName, path, id, thumbTex );
	allCubmapsMap.insert( std::pair<string, MCubemap*>( cubemap->name, cubemap ) );
	allCubmapsMapNamesList.push_back( cubemap->name.GetStr() );
	return cubemap;
}

void MCubemap::DestroyAll()
{
	while ( !allCubmapsMap.empty() )
	{
		MCubemap* temp = allCubmapsMap.begin()->second;
		allCubmapsMap.erase( allCubmapsMap.begin() );
		delete temp;
	}

	allCubmapsMap.clear();
	allCubmapsMapNamesList.clear();
}

 void MCubemap::Destroy( MCubemap* cubemap )
 {
	 for ( map<string, MCubemap*>::iterator it = allCubmapsMap.begin(); it != allCubmapsMap.end(); ++it )
	 {
		 if ( it->second == cubemap )
		 {
			 allCubmapsMap.erase( it );
			 break;
		 }
	 }

	 delete cubemap;
 }


 MCubemap* MCubemap::Get( const MString& name )
{
	map<string, MCubemap*>::iterator it = allCubmapsMap.find( name.GetStr() );
	return it->second;
}


map<string, MCubemap*> MCubemap::GetAllCubemaps()
{
	return allCubmapsMap;
}

list<string> MCubemap::GetAllCubemapNamesList()
{
	return allCubmapsMapNamesList;
}