#ifndef MSHADER_H
#define MSHADER_H

#include "Math3D.h"
#include "MString.h"

#include <map>
#include <list>


namespace MANA3D
{
	enum class MUniformType
	{
		MUT_uInt = 5124,
		MUT_uFloat = 5126,
		MUT_uVec3 = 35665,
		MUT_uVec4 = 35666,
		MUT_uBool = 35670,
		MUT_uMat4 = 35676,
		MUT_uSampler2D = 35678,
		MUT_uCubemap = 35680
	};

	struct MUniformData
	{
		int locationID;
		MUniformType type;
		int texIndex;
	};

	class MShader
	{
		friend class MANA3DEngine;
		friend class MMaterial;
		friend class Camera;
		friend class GraphicSettings;

	private:
		MShader( const MString& vertexPath, const MString& fragmentPath, const MString& name );
		~MShader();

		void CheckCompileErrors( unsigned int shader, MString type );

		void PrintLog();


		unsigned int id;

		std::string vertexCode;
		std::string fragmentCode;

		MString name;

		std::map<std::string, MUniformData> uniformsMap;


	public:
		void Use();
		
		void SetBool( const MString &uniName, bool value );
		void SetInt( const MString & uniName, int value );
		void SetFloat( const MString & uniName, float value );
		void SetDouble( const MString & uniName, double value );
		void SetVec2( const MString & uniName, const glm::vec2 &value );
		void SetVec2( const MString & uniName, float x, float y );
		void SetVec3( const MString & uniName, const glm::vec3 &value );
		void SetVec3( const MString & uniName, float x, float y, float z );
		void SetVec4( const MString & uniName, const glm::vec4 &value );
		void SetVec4( const MString & uniName, float x, float y, float z, float w );
		void SetMat2( const MString & uniName, const glm::mat2 &mat );
		void SetMat3( const MString & uniName, const glm::mat3 &mat );
		void SetMat4( const MString & uniName, glm::mat4 mat4 );

		void SetMat4Array( const MString & uniName, glm::mat4* mat4Array );
		void SetMat4Array( const MString & uniName, std::vector<glm::mat4> mat4Array );


		unsigned int GetID();

		MString GetName();

		bool HasUniform( MString uniformName );

		int GetUniformLocation( const MString& uniformName );

		std::map<std::string, MUniformData> GetUniformsMap();






	// -------- Private Static --------
	private:
		static std::map<std::string, MShader*> privateShadersMap;
		static std::map<std::string, MShader*> scenePublicShadersMap;
		static std::map<std::string, MShader*> runtimePublicShadersMap;
		// Only for Editor uses.
		static std::list<std::string> publicShaderNamesList;

		
		static MShader* Create_Internal( const MString& vertexPath, const MString& fragmentPath, const MString& name, std::map<std::string, MShader*>& container );

		static MShader* GetPrivate( const MString& name );

		static void GetFiles( MString subFolder, MString& shadersDir, std::vector<MString>& vertexShaders, std::vector<MString>& fragmentShaders );
		static void LoadShadersFolder();

		//static MShader* defaultShader;

		static void Destroy_Internal( MShader* shader, std::map<std::string, MShader*>& container );
		static void DestroyAll();

		static void SetUniformBlocks( glm::mat4 projection, glm::mat4 view );


	// -------- Private Static --------
	public:
		static MShader* Create( const MString& vertexPath, const MString& fragmentPath, const MString& name );

		//static void Delete( MShader* );

		//static MShader* GetDefault();

		static MShader* Get( const MString& name );

		static std::list<std::string> GetAllShaderNamesList();
	};
}

#endif
