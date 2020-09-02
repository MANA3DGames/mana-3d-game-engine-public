#ifndef MODEL_3D_IMPORTER_H
#define MODEL_3D_IMPORTER_H

#include "GameObject.h"
#include "MTexture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

namespace MANA3D
{
	class MSkinnedMesh;
	class Skeleton;
	class Joint;
	class Animation;
	struct MVertex;
	

	class Model3DImporter
	{
		friend class EditorGuiManager;

	private:
		// This function imports a 3d model (meshes & textures) and encapsulates all its assets in one single gameObject.
		static GameObject* Import( const MString& path );

		// Processes a node in a recursive fashion. 
		static void ProcessNode( aiNode *node, const aiScene *scene, GameObject* root, std::vector<MTexture*>& textures, MString& directory );
		
		// Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
		static GameObject* ProcessMesh( aiMesh *mesh, aiNode* node, const aiScene *scene, std::vector<MTexture*>& textures, MString& directory );

		static void CheckJointNode( const aiNode *node, Joint* parent, Skeleton* skeleton );
		static Skeleton* LoadSkeleton( const aiScene *scene, std::map<std::string, Animation*>& animationMap );

		static void FillJointWeightDataForVertex( std::vector<MVertex>& vertices, aiBone* bone, int boneID, int weightCount );
		static Joint* CreateJoint( aiBone* bone, const int& index, const int& weightCount, const aiMatrix4x4& transformMatrix );

		// Checks all material textures of a given type and loads the textures if they're not loaded yet. the required info is returned as a Texture struct.
		static std::vector<MTexture*> LoadMaterialTextures( aiMaterial *mat, aiTextureType type, std::vector<MTexture*>& textures, MString& directory );

		static std::map<std::string, Animation*> LoadAnimations( const aiScene *scene );
	};
}

#endif // MODEL_3D_IMPORTER_H