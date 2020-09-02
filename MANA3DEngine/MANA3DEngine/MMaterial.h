#ifndef MMATERIAL_H
#define MMATERIAL_H

#include "MTexture.h"
#include "MCubemap.h"
#include "MShader.h"
#include "MString.h"

#include <map>
#include <list>
#include <vector>



namespace MANA3D
{
	class GameObject;

	struct MTextureIndexPair
	{
		MTexture* tex = nullptr;
		int indexInShader;
	};
	struct MCubemapIndexPair
	{
		MCubemap* cubeMap = nullptr;
		int indexInShader;
	};

	class MMaterial
	{
		friend class MANA3DEngine;
		friend class Renderer;

	private:
		MMaterial( MShader* shader, const MString& name );
		~MMaterial();

	public:
		void SetShader( MShader* shader );

		void SetTexture( const MString &name, MTexture* texture );
		void SetCubemap( const MString &name, MCubemap* cubemap );
		void SetBool( const MString &name, bool value ) const;
		void SetInt( const MString &name, int value ) const;
		void SetFloat( const MString &name, float value ) const;
		void SetDouble( const MString &name, double value ) const;
		void SetVec2( const MString &name, const glm::vec2 &value ) const;
		void SetVec2( const MString &name, float x, float y ) const;
		void SetVec3( const MString &name, const glm::vec3 &value ) const;
		void SetVec3( const MString &name, float x, float y, float z ) const;
		void SetVec4( const MString &name, const glm::vec4 &value ) const;
		void SetVec4( const MString &name, float x, float y, float z, float w );
		void SetMat2( const MString &name, const glm::mat2 &mat ) const;
		void SetMat3( const MString &name, const glm::mat3 &mat ) const;
		void SetMat4( const MString &name, glm::mat4 value ) const;

		void SetMat4Array( const MString& name, glm::mat4* mat4Array ) const;
		void SetMat4Array( const MString& name, std::vector<glm::mat4> mat4Array ) const;

		void BindMaterial();

		MString GetName();
		MString GetShaderName();

		std::map<std::string, MTextureIndexPair>& GetTextureIndexPairsMap();
		std::map<std::string, MCubemapIndexPair>& GetCubemapIndexPairsMap();

	private:

		MString name;
		MShader* shader;

		std::map<std::string, MTextureIndexPair> textureIndexPairsMap;
		std::map<std::string, MCubemapIndexPair> cubemapIndexPairsMap;
		std::map<std::string, float> floatsMap;
		std::map<std::string, int> intsMap;
		std::map<std::string, glm::vec3> vec3sMap;


	// -------- Private Static --------
	private: 

		static std::map<std::string, MMaterial*> privateMaterialsMap;
		static std::map<std::string, MMaterial*> scenePublicMaterialsMap;
		static std::map<std::string, MMaterial*> runtimePublicMaterialsMap;
		// Only for Editor uses.
		static std::list<std::string> publicMaterialNames;


		static MMaterial* Create_Internal( MShader* shader, const MString& name, std::map<std::string, MMaterial*>& container );

		static MMaterial* GetPrivate( const MString& name );

		static void InitPrivateMatrials( std::map<std::string, MTexture*>& privateTexturesMap );

		// Only used from Editor - remove reference for public texture from public material.
		static void RemoveTextureReferences( MTexture* tex );


		static void Destroy_Internal( MMaterial* mat, std::map<std::string, MMaterial*>& container );
		static void DestroyAll();


	// -------- Public Static --------
	public:	
		// Creates public matrial.
		static MMaterial* Create( MShader* shader, const MString& name );

		static void Delete( MMaterial* mat );

		static MMaterial* Get( const MString& name );

		static std::list<std::string>& GetAllMaterialNamesList();
		
		static std::map<std::string, MMaterial*>& GetAllMaterials();

		static void SetMaterialShader( MMaterial* mat, MShader* shader );
		static void SetMaterialShaderbyName( MMaterial* mat, const MString& shaderName );

		static void UpdateName( MMaterial* mat, const MString& newName );
	};
}

#endif // !MMATERIAL_H

