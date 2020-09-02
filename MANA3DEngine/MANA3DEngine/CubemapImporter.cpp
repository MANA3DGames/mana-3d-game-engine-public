#include "CubemapImporter.h"
#include "Texture2DImporter.h"
#include "MTexture.h"
#include "MCubemap.h"
#include "Application.h"
#include "Debug.h"
using namespace MANA3D;

#include <GL3W/gl3w.h>
#include <stb_image.h>

using std::string;
using std::vector;


// Loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
MCubemap* CubemapImporter::Import( const MFile& file )
{
    MString path = file.path;//MString( "%s%s/", file.path.GetCStr(), file.name.GetCStr() );

	vector<string> faces
	{
		MString( "%sright.png", path.GetCStr() ).GetStr(),
		MString( "%sleft.png", path.GetCStr() ).GetStr(),
		MString( "%stop.png", path.GetCStr() ).GetStr(),
		MString( "%sbottom.png", path.GetCStr() ).GetStr(),
		MString( "%sfront.png", path.GetCStr() ).GetStr(),
		MString( "%sback.png", path.GetCStr() ).GetStr()
	};

    unsigned int textureID;
    glGenTextures( 1, &textureID );
    glBindTexture( GL_TEXTURE_CUBE_MAP, textureID );

    int width, height, nrChannels;
    for ( unsigned int i = 0; i < faces.size(); i++ )
    {
        unsigned char *data = stbi_load( faces[i].c_str(), &width, &height, &nrChannels, 0 );
        if ( data )
        {
            glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
            stbi_image_free( data );
        }
        else
        {
            Debug::LogError( MString( "Cubemap texture failed to load at path: %s", faces[i].c_str() ) );
            stbi_image_free( data );
        }
    }

    // Take textrue at index 2 (which is supposed to be the front) as a thumb image.
	MTexture* thumbTex = Texture2DImporter::Import( faces[2], file.name );
	
	faces.clear();

    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

    return MCubemap::Create( file.name, path, textureID, thumbTex );
}
