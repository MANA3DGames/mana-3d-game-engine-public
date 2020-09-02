#ifndef MSKINNED_MESH_H
#define MSKINNED_MESH_H

#include "MMesh.h"


namespace MANA3D
{
	class MSkinnedMesh : public MMesh
	{
		friend class MANA3DEngine;
		friend class Model3DImporter;

	protected:
		MSkinnedMesh( const std::vector<MVertex>& vertices, const std::vector<unsigned int>& indices, const MString& meshName );
		virtual ~MSkinnedMesh();



	private:
		static std::map<std::string, MSkinnedMesh*> allSkinnedMeshesMap;
		static std::list<std::string> allSkinnedMeshNamesList;

		static void DestroyAll();

	public:
		static MSkinnedMesh* Create( const std::vector<MVertex>& vertices, const std::vector<unsigned int>& indices, const MString& meshName );
		static void Destroy( MSkinnedMesh* );


		static MSkinnedMesh* Get( const MString& name );

		static std::list<std::string> GetAllSkinnedMeshNamesList();
	};
}

#endif
