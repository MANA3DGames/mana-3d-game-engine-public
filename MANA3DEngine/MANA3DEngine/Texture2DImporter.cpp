#include "Texture2DImporter.h"
#include "Application.h"
#include "MTexture.h"
#include "Debug.h"
using namespace MANA3D;

#include <GL3W/gl3w.h>
#include <stb_image.h>

using std::vector;


void Texture2DImporter::LoadEditorSubFolderTextures( const char* subFolder )
{
	MString path( "%s%s\\", Application::GetAppDirctory().GetCStr(), subFolder );
	vector<MFile> allFiles = Application::GetAllFiles( path, MFileFilter::MFF_File );

	for ( vector<MFile>::iterator it = allFiles.begin(); it != allFiles.end(); ++it )
	{
		// Load texture from the given file path.
		ImageRawData rawData = ImportRawData( MString( "%s%s", it->path.GetCStr(), it->name.GetCStr() ), false, 1, false );

		// Return the new created texture.
		MTexture* tex = MTexture::CreatePrivate( path, rawData.id, rawData.width, rawData.height, rawData.channels, 0, false, false, it->name );

		//Debug::Log( tex->GetName() );
	}
}
void Texture2DImporter::LoadEditorTextures()
{
	LoadEditorSubFolderTextures( "Editor" );
	LoadEditorSubFolderTextures( "Editor/Gizmo" );
	LoadEditorSubFolderTextures( "Editor/Icons" );
}


ImageRawData Texture2DImporter::ImportRawData( const MString& path, const bool& flipY, const int& wrapMode, const bool& useSRGB )
{
	ImageRawData rawData;

	// Generate texture ID.
	glGenTextures( 1, &rawData.id );
	
	// Tell stb_image.h weather to flip loaded texture's on the y-axis or not.
	stbi_set_flip_vertically_on_load( flipY ); 
	
	// Load image, create texture and generate mipmaps
	unsigned char *data = stbi_load( path.GetCStr(), &rawData.width, &rawData.height, &rawData.channels, 0 );

	// Check if we got an actual image data.
	if ( data )
	{
		// Check how many channels we have.
		GLenum format = {};
		if ( rawData.channels == 1 )
			format = GL_RED;
		else if ( rawData.channels == 3 )
			format = GL_RGB;
		else if ( rawData.channels == 4 )
			format = GL_RGBA;

		glBindTexture( GL_TEXTURE_2D, rawData.id );
		glTexImage2D( GL_TEXTURE_2D, 0, format, rawData.width, rawData.height, 0, format, GL_UNSIGNED_BYTE, data );
		glGenerateMipmap( GL_TEXTURE_2D );

		// Check wrapping mode.
		GLenum wrapModeMethod = {};
		if ( wrapMode == 0 )
			wrapModeMethod = GL_REPEAT;
		else if ( wrapMode == 1 )
			wrapModeMethod = GL_CLAMP_TO_EDGE;

		// Set the texture wrapping parameters. [GL_REPEAT] is the default wrapping method.
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapModeMethod );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapModeMethod );

		// Set texture filtering parameters
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	}
	else
	{
		Debug::LogError( MString( "Failed to load texture: %s", path.GetCStr() ) );
	}

	// Free stbi image data as we don't need it anymore.
	stbi_image_free( data );

	return rawData;
}

MTexture* Texture2DImporter::Import( const MString& path, const MString& newName, const bool& flipY, const int& wrapMode, const bool& useSRGB )
{
	// Load texture from the given file path.
	ImageRawData rawData = ImportRawData( path, flipY, wrapMode, useSRGB );

	// Return the new created texture.
	return MTexture::Create( path, rawData.id, rawData.width, rawData.height, rawData.channels, wrapMode, flipY, useSRGB, newName );
}

MTexture* Texture2DImporter::ReImport( MTexture* tex, const bool& flipY, const int& wrapMode, const bool& useSRGB )
{
	// Get current texture path.
	const MString& path = tex->GetPath();
	// Load re-import image raw data.
	ImageRawData rawData = ImportRawData( path, flipY, wrapMode, useSRGB );
	// Update current texture with the new properies.
	MTexture::UpdateData( tex, tex->GetPath(), rawData.id, rawData.width, rawData.height, rawData.channels, wrapMode, flipY, useSRGB );
	// Return the updated texture.
	return tex;
}