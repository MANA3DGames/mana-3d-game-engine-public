#include "MTexture.h"
#include "MANA3DEngine.h"
#include "Debug.h"
using namespace MANA3D;

#include <iostream>

#include <GL3W/gl3w.h>

using std::map;
using std::string;




MTexture::MTexture( const MString& path, const unsigned int& id, const int& width, const int& height, const int& channels, const int& wrapMode, const bool& isYFlipped, const bool& sRGB, const MString& newName ) :
	name( newName ),
	path( path ),
	id( id ),
	width( width ),
	height( height ),
	channels( channels ),
	wrapMode( wrapMode ),
	isYFlipped( isYFlipped ),
	isSRGB( sRGB )
{
	if ( name == "" )
	{
		name = MString::EraseSubStr( path, 0, MString::GetLastFoundIndex( path, '/' ) + 1 );
		name = MString::EraseSubStr( name, 0, MString::GetLastFoundIndex( name, '\\' ) + 1 );
	}
}

MTexture::~MTexture()
{
	DestroyMyID();

	id = NULL;
	width = NULL;
	height = NULL;
	channels = NULL;

	path.Clear();
	name.Clear();
}


void MTexture::DestroyMyID()
{
	glDeleteTextures( 1, &id );
}


unsigned int MTexture::GetID()
{
	return id;
}

const MString& MTexture::GetPath()
{
	return path;
}

const MString& MTexture::GetName()
{
	return name;
}


const int& MTexture::GetWidth()
{
	return width;
}
const int& MTexture::GetHeight()
{
	return height;
}
const int& MTexture::GetChannels()
{
	return channels;
}
const int& MTexture::GetWrapMode()
{
	return wrapMode;
}

const bool& MTexture::GetIsYFlipped()
{
	return isYFlipped;
}
const bool& MTexture::GetIsSRGB()
{
	return isSRGB;
}

MString MTexture::GetWrapModeName()
{
	return wrapMode == 0 ? "Repeat" : "Clamp To Edge";
}



// ****************** Statics ******************
map<string, MTexture*> MTexture::privateTexturesMap;
map<string, MTexture*> MTexture::scenePublicTexturesMap;
map<string, MTexture*> MTexture::runtimePublicTexturesMap;


MTexture* MTexture::Create_Internal( const MString& path, const unsigned int& id, const int& width, const int& height, const int& channels, const int& wrapMode, const bool& isYFlipped, const bool& sRGB, const MString& newName, map<string, MTexture*>& container )
{
	MTexture *tex = new MTexture( path, id, width, height, channels, wrapMode, isYFlipped, sRGB, newName );

	// Check if we already have a texture with this name?
	int num = 1;
	string toBeNewName( tex->name );
	while ( true )
	{
		map<string, MTexture*>::iterator it = container.find( toBeNewName );
		if ( it != container.end() )
		{
			num++;
			toBeNewName = MString( "%s%i", toBeNewName.c_str(), num );
		}
		else
			break;
	}
	tex->name = toBeNewName;
	container.insert( std::pair<string, MTexture*>( tex->name, tex ) );
	return tex;
}

MTexture* MTexture::Create( const MString& path, const unsigned int& id, const int& width, const int& height, const int& channels, const int& wrapMode, const bool& isYFlipped, const bool& sRGB, const MString& newName )
{
	return Create_Internal( path, id, width, height, channels, wrapMode, isYFlipped, sRGB, newName, MANA3DEngine::GetIsEditorLoop() ? scenePublicTexturesMap : runtimePublicTexturesMap );
}
MTexture* MTexture::CreatePrivate( const MString& path, const unsigned int& id, const int& width, const int& height, const int& channels, const int& wrapMode, const bool& isYFlipped, const bool& sRGB, const MString& newName )
{
	return Create_Internal( path, id, width, height, channels, wrapMode, isYFlipped, sRGB, newName, privateTexturesMap );
}

void MTexture::UpdateData( MTexture* tex, const MString& path, const unsigned int& id, const int& width, const int& height, const int& channels, const int& wrapMode, const bool& isYFlipped, const bool& sRGB )
{
	tex->DestroyMyID();

	tex->id = id;
	tex->path = path;
	tex->width = width;
	tex->height = height;
	tex->channels = channels;
	tex->wrapMode = wrapMode;
	tex->isYFlipped = isYFlipped;
	tex->isSRGB = sRGB;
}


void MTexture::DestroyAll()
{
	while ( !privateTexturesMap.empty() )
	{
		MTexture* temp = privateTexturesMap.begin()->second;
		privateTexturesMap.erase( privateTexturesMap.begin() );
		delete temp;
	}
	privateTexturesMap.clear();

	while ( !scenePublicTexturesMap.empty() )
	{
		MTexture* temp = scenePublicTexturesMap.begin()->second;
		scenePublicTexturesMap.erase( scenePublicTexturesMap.begin() );
		delete temp;
	}
	scenePublicTexturesMap.clear();

	while ( !runtimePublicTexturesMap.empty() )
	{
		MTexture* temp = runtimePublicTexturesMap.begin()->second;
		runtimePublicTexturesMap.erase( runtimePublicTexturesMap.begin() );
		delete temp;
	}
	runtimePublicTexturesMap.clear();
}

 void MTexture::Destroy_Internal( MTexture* tex, map<string, MTexture*>& container )
 {
	 for ( map<string, MTexture*>::iterator it = container.begin(); it != container.end(); ++it )
	 {
		 if ( it->second == tex )
		 {
			 // Remove all references to this pointer before erasing it.
			 OnDestroyTextureCallbackFunc( tex );

			 container.erase( it );
			 break;
		 }
	 }

	 delete tex;
 }

void MTexture::Delete( MTexture* tex )
{
	Destroy_Internal( tex, MANA3DEngine::GetIsEditorLoop() ? scenePublicTexturesMap : runtimePublicTexturesMap );
}


 MTexture* MTexture::GetPrivate( const MString& texName )
 {
	map<string, MTexture*>::iterator it = privateTexturesMap.find( texName.GetStr() );
	if ( it != privateTexturesMap.end() )
		return it->second;
	else
		return nullptr;
 }

MTexture* MTexture::Get( const MString& name )
{
	map<string, MTexture*> tMap = MANA3DEngine::GetIsEditorLoop() ? scenePublicTexturesMap : runtimePublicTexturesMap;
	map<string, MTexture*>::iterator it = tMap.find( name.GetStr() );
	if ( it != tMap.end() )
		return it->second;
	else
		return nullptr;
}


map<string, MTexture*>& MTexture::GetAllTextures()
{
	return MANA3DEngine::GetIsEditorLoop() ? scenePublicTexturesMap : runtimePublicTexturesMap;
}





void ( *MTexture::OnDestroyTextureCallbackFunc )( MTexture* texToBeDestroyed );
void MTexture::SetOnDestroyTextureCallbackFunc( void(*func)( MTexture* texToBeDestroyed ) )
{
	OnDestroyTextureCallbackFunc = func;
}