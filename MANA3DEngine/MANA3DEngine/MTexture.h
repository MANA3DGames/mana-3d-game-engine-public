#ifndef MTEXTURE_H
#define MTEXTURE_H

#include <map>

#include "MString.h"


namespace MANA3D
{
	class MTexture
	{
		friend class MANA3DEngine;

	public:
		unsigned int GetID();

		const MString& GetPath();
		const MString& GetName();

		const int& GetWidth();
		const int& GetHeight();
		const int& GetChannels();
		const int& GetWrapMode();

		const bool& GetIsYFlipped();
		const bool& GetIsSRGB();

		MString GetWrapModeName();

	private:
		MTexture( const MString& path, const unsigned int& id, const int& width, const int& height, const int& channels, const int& wrapMode, const bool& isYFlipped, const bool& sRGB, const MString& newName = "" );
		~MTexture();

		void DestroyMyID();

		unsigned int id;

		int width;
		int height;
		int channels;
		int wrapMode;

		bool isYFlipped;
		bool isSRGB;

		MString path;
		MString name;



		// Static 
	private:
		static std::map<std::string, MTexture*> privateTexturesMap;
		static std::map<std::string, MTexture*> scenePublicTexturesMap;
		static std::map<std::string, MTexture*> runtimePublicTexturesMap;


		static MTexture* Create_Internal( const MString& path, const unsigned int& id, const int& width, const int& height, const int& channels, const int& wrapMode, const bool& isYFlipped, const bool& sRGB, const MString& newName, std::map<std::string, MTexture*>& container );

		static MTexture* GetPrivate( const MString& texName );

		static void Destroy_Internal( MTexture* mat, std::map<std::string, MTexture*>& container );
		static void DestroyAll();

		static void( *OnDestroyTextureCallbackFunc )( MTexture* texToBeDestroyed );
		static void SetOnDestroyTextureCallbackFunc( void(*func)( MTexture* texToBeDestroyed ) );
		

	public:
		static MTexture* Create( const MString &path, const unsigned int& id, const int& width, const int& height, const int& channels, const int& wrapMode, const bool& isYFlipped, const bool& sRGB, const MString& newName );
		static MTexture* CreatePrivate( const MString &path, const unsigned int& id, const int& width, const int& height, const int& channels, const int& wrapMode, const bool& isYFlipped, const bool& sRGB, const MString& newName );

		static void UpdateData( MTexture* tex, const MString& path, const unsigned int& id, const int& width, const int& height, const int& channels, const int& wrapMode, const bool& isYFlipped, const bool& sRGB );

		static MTexture* Get( const MString& texName );

		static std::map<std::string, MTexture*>& GetAllTextures();

		static void Delete( MTexture* tex );
	};
}

#endif

