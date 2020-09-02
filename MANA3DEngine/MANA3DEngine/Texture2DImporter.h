#ifndef TEXTURE_2D_IMPORTER_H
#define TEXTURE_2D_IMPORTER_H

#include "MString.h"

namespace MANA3D
{
	class MTexture;

	struct ImageRawData
	{
		unsigned int id;
		int width;
		int height;
		int channels;
	};

	class Texture2DImporter
	{
		friend class MANA3DEngine;
		friend class EditorGuiManager;
		friend class CubemapImporter;
		friend class Model3DImporter;

	private:
		static void LoadEditorSubFolderTextures( const char* subFolder );
		static void LoadEditorTextures();

		static ImageRawData ImportRawData( const MString &path, const bool& flipY, const int& wrapMode, const bool& useSRGB );
		static MTexture* Import( const MString &path, const MString& newName = "", const bool& flipY = false, const int& wrapMode = 0, const bool& useSRGB = false );

	public:
		static MTexture* ReImport( MTexture* tex, const bool& flipY = false, const int& wrapMode = 0, const bool& useSRGB = false );
	};
}

#endif // TEXTURE_2D_IMPORTER_H

