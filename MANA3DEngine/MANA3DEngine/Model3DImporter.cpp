#include "Model3DImporter.h"
#include "Texture2DImporter.h"
#include "MSkinnedMesh.h"
#include "SkinnedMeshRenderer.h"
#include "Skeleton.h"
#include "Animation.h"
#include "KeyFrame.h"
#include "Transform.h"
#include "SceneManager.h"
#include "Math3D.h"
using namespace MANA3D;
using namespace Assimp;

#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

using std::map;
using std::string;
using std::vector;


aiMatrix4x4 aiGlobalInverseTransform;
map<string, Joint*> jointsMap;
vector<SkinnedMeshRenderer*> skinnedMeshRenderersForCurrentFile;



void PrintHeirarchy( aiNode* node, int increment )
{
	MString spacing;
	for ( int i = 0; i < increment; i++ )
		spacing.Append( "\t" );

	Debug::Log( MString( "%s%s", spacing.GetCStr(), node->mName.C_Str() ) );

	increment++;
	for ( unsigned int i = 0; i < node->mNumChildren; i++)
	{
		PrintHeirarchy( node->mChildren[i], increment );
	}
}
void PrintSkeleton( Joint* joint, int increment )
{
	MString spacing;
	for ( int i = 0; i < increment; i++)
		spacing.Append( "\t" );

	Debug::Log( MString( "%s%s", spacing.GetCStr(), joint->GetName().GetCStr() ) );

	increment++;
	for ( int i = 0; i < joint->GetChildrenCount(); i++)
	{
		PrintSkeleton( joint->GetChildren()[i], increment );
	}
}




GameObject* Model3DImporter::Import( const MString& path )
{
	// Read 3d model file with ASSIMP.
	Importer importer;
	const aiScene* scene = importer.ReadFile( path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace );

	// Check for errors?
	if ( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
	{
		Debug::LogError( MString( "Error: Failed to import 3d model with error %s", importer.GetErrorString() ) );
		return nullptr;
	}

	// Check if this is an actual 3d mesh or not?
	if ( !scene->HasMeshes() )
	{
		Debug::LogError( "Error: This file has no 3d mesh" );
		return nullptr;
	}


	// Create a new prefab object for this model.
	GameObject* root = SceneManager::CreatePrefabAsset( scene->mRootNode->mName.C_Str(), true );

	// Define a new container for textures.
	vector<MTexture*> textures;

	// Retrieve the directory path of the filepath
	MString directory = MString::GetSubStr( path, 0, MString::GetLastFoundIndex( path, '\\' ) );

	// Calculate global inverse transform matrix (for animation).
	aiGlobalInverseTransform = scene->mRootNode->mTransformation;
	aiGlobalInverseTransform.Inverse();

	// Process ASSIMP's root node recursively.
	ProcessNode( scene->mRootNode, scene, root, textures, directory );


	// Check if this file has animations
	if ( scene->HasAnimations() )
	{
		// Load animation into our data structure.
		map<string, Animation*> animationMap = LoadAnimations( scene );

		// Create skeleton object.
		Skeleton* skeleton = LoadSkeleton( scene, animationMap );
		
		//// Set animations map for our skeleton.
		//skeleton->SetAnimationMap( animationMap );

		// Set skeleton reference for all Skinned Mesh Renderers. 
		for ( vector<SkinnedMeshRenderer*>::iterator it = skinnedMeshRenderersForCurrentFile.begin(); it != skinnedMeshRenderersForCurrentFile.end(); ++it )
			(*it)->SetSkeleton( skeleton );
	}

	// Clear skinned mesh renderers list we don't need it any more.
	skinnedMeshRenderersForCurrentFile.clear();

	// Return the new created prefab.
	return root;
}


void Model3DImporter::ProcessNode( aiNode *node, const aiScene *scene, GameObject* root, vector<MTexture*>& textures, MString& directory )
{
	// Process each mesh located at the current node.
	for ( unsigned int i = 0; i < node->mNumMeshes; i++ )
	{
		int meshIndex = node->mMeshes[i];
		aiMesh* mesh = scene->mMeshes[meshIndex];
		GameObject* go = ProcessMesh( mesh, node, scene, textures, directory );
		go->transform->SetParent( root->transform );
	}

	// Process children nodes.
	for ( unsigned int i = 0; i < node->mNumChildren; i++ )
	{
		ProcessNode( node->mChildren[i], scene, root, textures, directory );
	}
}

GameObject* Model3DImporter::ProcessMesh( aiMesh *mesh, aiNode* node, const aiScene *scene, vector<MTexture*>& textures, MString& directory )
{
	// data to fill
	vector<MVertex> vertices;
	vector<unsigned int> indices;

	// Walk through each of the mesh's vertices
	for ( unsigned int i = 0; i < mesh->mNumVertices; i++ )
	{
		MVertex vertex;

		for ( int i = 0; i < NUM_BONES_PER_VEREX; i++ )
		{
			vertex.jointIds[i] = -1;
			vertex.jointWeights[i] = 0.0f;
		}

		// We declare a placeholder vector since assimp uses its own vector class that doesn't directly 
		// convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		glm::vec3 vector; 
		
		// Positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		// Normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;
		
		// Texture coordinates
		if ( mesh->mTextureCoords[0] ) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
			vertex.texCoords = glm::vec2( 0.0f, 0.0f );
		
		// Tangent
		if ( mesh->mTangents != nullptr )
		{
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.tangent = vector;
		}
		
		// Bitangent
		if ( mesh->mBitangents != nullptr )
		{
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.bitangent = vector;
		}
		
		vertices.push_back( vertex );
	}

	// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for ( unsigned int i = 0; i < mesh->mNumFaces; i++ )
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for ( unsigned int j = 0; j < face.mNumIndices; j++ )
			indices.push_back( face.mIndices[j] );
	}


	// Process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];


	vector<MTexture*> diffuseMaps = LoadMaterialTextures( material, aiTextureType_DIFFUSE, textures, directory );
	vector<MTexture*> specularMaps = LoadMaterialTextures( material, aiTextureType_SPECULAR, textures, directory );
	vector<MTexture*> normalMaps = LoadMaterialTextures( material, aiTextureType_HEIGHT, textures, directory );
	vector<MTexture*> heightMaps = LoadMaterialTextures( material, aiTextureType_AMBIENT, textures, directory );

	MString mName = mesh->mName.length > 0 ? mesh->mName.C_Str() : "NoNameMesh";
	MString mMaterialName( "%sMat", mName.GetCStr() );

	GameObject* gameObject = SceneManager::CreatePrefabAsset( mName, false );

	MMaterial* mat = nullptr;
	Renderer* renderer = nullptr;

	// Check if it is a SkinnedMeshRenderer or just a normal MeshRenderer??
	if ( mesh->HasBones() )
	{
		// Add skinned mesh renderer component for this gameObject.
		//renderer = gameObject->AddComponent<SkinnedMeshRenderer>();
		SkinnedMeshRenderer* skinnedMeshRenderer = gameObject->AddComponent<SkinnedMeshRenderer>();
		renderer = skinnedMeshRenderer;

		// Keep a reference for the new created SkinnedMeshRenderer to assign a skeleton reference.
		skinnedMeshRenderersForCurrentFile.push_back( skinnedMeshRenderer );

		// Get total number of bones.
		unsigned int bonesCount = mesh->mNumBones;
		// Iteraite over all the bones. 
		for ( size_t jointId = 0; jointId < bonesCount; jointId++ )
		{
			// Get a reference for the current bone.
			aiBone* bone = mesh->mBones[jointId];
			// Get how many vertices this bone influnce. 
			int weightCount = bone->mNumWeights;

			// Add bone weight data to our vertex struct.
			FillJointWeightDataForVertex( vertices, bone, jointId, weightCount );

			// Create joint object.
			Joint* joint = CreateJoint( bone, jointId, weightCount, node->mTransformation );
			jointsMap.insert( std::pair<string, Joint*>( joint->GetName().GetStr(), joint ) );
		}
			
		// Create a MSkinnedMesh object from the extracted mesh data.
		MSkinnedMesh* mmesh = MSkinnedMesh::Create( vertices, indices, mName );
		skinnedMeshRenderer->SetSkinnedMesh( mmesh );

		mat = MMaterial::Create( MShader::Get( "DiffuseSkinned" ), mMaterialName );
	}
	else
	{
		// Add mesh renderer component for this gameObject.
		renderer = gameObject->AddComponent<MeshRenderer>();

		// Create a MMesh object from the extracted mesh data.
		MMesh* mmesh = MMesh::Create( vertices, indices, mName );
		( (MeshRenderer*)renderer )->SetMesh( mmesh );

		mat = MMaterial::Create( MShader::Get( "Diffuse" ), mMaterialName );
	}


	// VERY IMPORTANT: Remove the new created renderer from SceneRenderer Vector 
	// as they are prefabs so we don't want to include them in the render stage.
	Renderer::RemoveRendererFromSceneRenderersContainer( renderer );


	renderer->SetMaterial( mat );
	if ( diffuseMaps.size() > 0 && diffuseMaps[0] != nullptr )
		renderer->SetMaterialTexture( "diffuseMap", diffuseMaps[0] );
	if ( specularMaps.size() > 0 && specularMaps[0] != nullptr )
		renderer->SetMaterialTexture( "specularMap", specularMaps[0] );

	return gameObject;
}



void Model3DImporter::FillJointWeightDataForVertex( vector<MVertex>& vertices, aiBone* bone, int boneID, int weightCount )
{
	// For every weight influnce do:
	for ( int wCI = 0; wCI < weightCount; wCI++ )
	{
		// Get the current index of the vertex affected by this bone.
		int verId = bone->mWeights[wCI].mVertexId;
		float boneWeight = bone->mWeights[wCI].mWeight;
		MVertex& vertex = vertices[verId];

		// This index counter will be used to find an empty slot in the Vertex.jointIds & Vertex.jointWeights
		int index = 0;
		// Keep search for empty slot (-1)
		while ( true )
		{
			// Check if this is an empty slot (which has -1 value)
			if ( vertex.jointIds[index] == -1 )
			{
				// Set the joint ID value of the current bone/jointId. 
				vertex.jointIds[index] = boneID;
				// Set the weight value of the current bone's weight. 
				vertex.jointWeights[index] = boneWeight;
				// Quit the while loop.
				break;
			}

			// Check if the index excceed the max allowed number of bone that influnce each vertex.
			if ( index >= NUM_BONES_PER_VEREX )
			{
				// Sort our weights array.
				int n = sizeof( vertex.jointWeights ) / sizeof( vertex.jointWeights[0] );
				std::sort( vertex.jointWeights, vertex.jointWeights+n );

				// Check if boneWeight is greater than minWeight?
				if ( boneWeight > vertex.jointWeights[0] )
				{
					vertex.jointIds[0] = boneID;
					vertex.jointWeights[0] = boneWeight;
				}

				break;
			}

			// Increament the index till we find an empty slot.
			index++;
		}
	}
}


Joint* Model3DImporter::CreateJoint( aiBone* bone, const int& index, const int& weightCount, const aiMatrix4x4& transformMatrix )
{
	// Get the name of the joint.
	MString name = bone->mName.C_Str();

	// Get all vertex weights.
	vector<MVertexWeightPair> vertexWeightPairs;
	for ( int i = 0; i < weightCount; i++ )
	{
		MVertexWeightPair vwPair;
		vwPair.vertexID = bone->mWeights[i].mVertexId;
		vwPair.weight = bone->mWeights[i].mWeight;

		vertexWeightPairs.push_back( vwPair );
	}

	// Create a new return the new created Joint.
	return new Joint( index, name, vertexWeightPairs, transformMatrix, bone->mOffsetMatrix );
}


void Model3DImporter::CheckJointNode( const aiNode *node, Joint* parent, Skeleton* skeleton )
{
	map<string, Joint*>::iterator it = jointsMap.find( node->mName.C_Str() );
	Joint* joint = nullptr;
	if ( it != jointsMap.end() )
	{
		joint = it->second;
		if ( parent != nullptr )
			parent->AddJoint( joint );

		if ( skeleton->GetRootJoint() == nullptr )
			skeleton->SetRootJoint( joint );
	}

	for ( unsigned int i = 0; i < node->mNumChildren; i++ )
	{
		if ( joint == nullptr )
			joint = parent;

		CheckJointNode( node->mChildren[i], joint, skeleton );
	}
}


Skeleton* Model3DImporter::LoadSkeleton( const aiScene *scene, map<string, Animation*>& animationMap )
{
	//PrintHeirarchy( scene->mRootNode, 0 );
	Skeleton* skeleton = Skeleton::Create( jointsMap.size(), aiGlobalInverseTransform, animationMap, skinnedMeshRenderersForCurrentFile.size() );
	aiNode* rootNode = scene->mRootNode;
	CheckJointNode( rootNode, nullptr, skeleton );
	//PrintSkeleton( skeleton->GetRootJoint(), 0 );
	return skeleton;
}

map<string, Animation*> Model3DImporter::LoadAnimations( const aiScene *scene )
{
	map<string, Animation*> animationsMap;

	for ( size_t i = 0; i < scene->mNumAnimations; i++ )
	{
		aiAnimation* anim = scene->mAnimations[i];

		MString name = anim->mName.C_Str();
		float ticksPerSecond = anim->mTicksPerSecond > 0.0 ? (float)anim->mTicksPerSecond : 25.0f;
		float duration = (float)anim->mDuration;
		int channelsCount = anim->mNumChannels;

		map<string, KeyFrame*> jointKeyFramesMap;

		for ( int ch = 0; ch < channelsCount; ch++ )
		{
			aiNodeAnim* nodeAnim = anim->mChannels[ch];
			string jointName = nodeAnim->mNodeName.C_Str();

			map<string, Joint*>::iterator it = jointsMap.find( jointName );
			if ( it != jointsMap.end() )
			{
				vector<VectorKey> positionKeyframes;
				vector<RotationKey> rotationKeyframes;
				vector<VectorKey> scaleKeyframes;

				// Position Keyframes.
				for ( size_t fi = 0; fi < nodeAnim->mNumPositionKeys; fi++ )
				{
					VectorKey key;
					key.time = nodeAnim->mPositionKeys[fi].mTime;
					key.value = nodeAnim->mPositionKeys[fi].mValue;
					positionKeyframes.push_back( key );
				}
				// Rotation Keyframes.
				for ( size_t fi = 0; fi < nodeAnim->mNumRotationKeys; fi++ )
				{
					RotationKey key;
					key.time = nodeAnim->mRotationKeys[fi].mTime;
					key.value = nodeAnim->mRotationKeys[fi].mValue;
					rotationKeyframes.push_back( key );
				}
				// Scale Keyframes.
				for (size_t fi = 0; fi < nodeAnim->mNumScalingKeys; fi++)
				{
					VectorKey key;
					key.time = nodeAnim->mScalingKeys[fi].mTime;
					key.value = nodeAnim->mScalingKeys[fi].mValue;
					scaleKeyframes.push_back( key );
				}

				KeyFrame* keyframe = new KeyFrame( jointName, positionKeyframes, rotationKeyframes, scaleKeyframes );

				jointKeyFramesMap.insert( std::pair<string, KeyFrame*>( jointName, keyframe ) );
			}
		}

		// Create a new animation.
		Animation* animation = new Animation( name, ticksPerSecond, duration, channelsCount, jointKeyFramesMap );

		// Add the new created animation to our animation map.
		animationsMap.insert( std::pair<string, Animation*>( name, animation ) );
	}

	return animationsMap;
}


vector<MTexture*> Model3DImporter::LoadMaterialTextures( aiMaterial *mat, aiTextureType type, vector<MTexture*>& textures, MString& directory )
{
	vector<MTexture*> mTextures;

	for ( unsigned int i = 0; i < mat->GetTextureCount( type ); i++ )
	{
		aiString str;
		mat->GetTexture( type, i, &str );
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for ( unsigned int j = 0; j < textures.size(); j++ )
		{
			if ( std::strcmp( textures[j]->GetPath().GetCStr(), str.C_Str() ) == 0 )
			{
				mTextures.push_back( textures[j] );
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}

		if ( !skip )
		{   // if texture hasn't been loaded already, load it
			MString filename( "%s\\%s", directory.GetCStr(), str.C_Str() );
			MTexture* tex = Texture2DImporter::Import( filename );
			textures.push_back( tex );
			mTextures.push_back( tex );
		}
	}

	return mTextures;
}
