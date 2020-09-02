#ifndef MMESH_H
#define MMESH_H

#include <vector>
#include <map>
#include <list>
#include <glm\glm.hpp>

#include "MString.h"

namespace MANA3D
{
	static const int NUM_BONES_PER_VEREX = 4;

	struct MVertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
		glm::vec3 tangent;
		glm::vec3 bitangent;

		int jointIds[NUM_BONES_PER_VEREX];
		float jointWeights[NUM_BONES_PER_VEREX];
	};

	class MMesh
	{
		friend class MANA3DEngine;
		friend class Model3DImporter;

	public:
		const std::vector<MVertex>& GetVertices();
		const std::vector<unsigned int>& GetIndices();

		const unsigned int& GetVertexCount();
		const unsigned int& GetIndexCount();
		const unsigned int& GetTriangleCount();

		const unsigned int& GetVAO();

		const MString& GetName();


	protected:
		MMesh( const std::vector<MVertex>& vertices, const std::vector<unsigned int>& indices, const MString& meshName, const bool& isSkinned );
		virtual ~MMesh();


		unsigned int VBO, VAO, EBO;

		std::vector<MVertex> vertices;
		std::vector<unsigned int> indices;

		int vertexCount;
		int indexCount;
		int triangleCount;

		MString name;



	private:
		static std::map<std::string, MMesh*> privateMeshesMap;
		static std::map<std::string, MMesh*> scenePublicMeshesMap;
		static std::map<std::string, MMesh*> runtimePublicMeshesMap;
		// Only for Editor uses.
		static std::list<std::string> publicMeshNamesList;


		static MMesh* Create_Internal( const std::vector<MVertex>& vertices, const std::vector<unsigned int>& indices, const MString& meshName, std::map<std::string, MMesh*>& container );

		static void CreatePrivateMeshes();
		static void CreateMainGridMesh();
		static void CreateCrossGridMesh();

		static MMesh* GetPrivate( const MString& name ); 

		static void Destroy_Internal( MMesh*, std::map<std::string, MMesh*>& container );
		static void DestroyAll();



		static void ( *OnDestroyMeshCallbackfunc )( MMesh* mesh );
		static void SetOnDestroyMeshCallbackfunc( void ( *callbackFunc )( MMesh* mesh ) );

	public:
		static MMesh* Create( const std::vector<MVertex>& vertices, const std::vector<unsigned int>& indices, const MString& meshName );

		static void Delete( MMesh* mesh );


		static MMesh* Get( const MString& name );

		static std::map<std::string, MMesh*>& GetAllMeshes();
		static std::list<std::string>& GetAllMeshNamesList();
	};
}

#endif // !MMESH_H

