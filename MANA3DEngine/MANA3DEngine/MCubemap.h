#ifndef MCUBEMAP_H
#define MCUBEMAP_H

#include "MString.h"

#include <list>
#include <map>


namespace MANA3D
{
	class MTexture;

	class MCubemap
	{
		friend class MANA3DEngine;

	public:
		unsigned int GetID();

		const MString& GetPath();
		const MString& GetName();

		MTexture* GetThumbTex();

	private:
		MCubemap( const MString& name, const MString& path, const unsigned int& id, MTexture* thumbTex );
		~MCubemap();


		unsigned int id;

		MString path;
		MString name;

		MTexture* thumbTex;



		// Static
	private:
		static std::list<std::string> allCubmapsMapNamesList;

		static std::map<std::string, MCubemap*> allCubmapsMap;

		static void DestroyAll();



	public:
		static MCubemap* Create( const MString& name, const MString& path, const unsigned int& id, MTexture* thumbTex );

		static MCubemap* Get( const MString& );

		static std::map<std::string, MCubemap*> GetAllCubemaps();
		static std::list<std::string> GetAllCubemapNamesList();

		static void Destroy( MCubemap* cubmap );
	};
}

#endif // !MCUBEMAP_H

