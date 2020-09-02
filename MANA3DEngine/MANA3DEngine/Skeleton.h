#ifndef SKELETON_H
#define SKELETON_H

#include "Joint.h"
#include "Animation.h"
#include "Math3D.h"

#include <map>
#include <assimp/quaternion.h>

namespace MANA3D
{
	const int MAX_JOINT_COUNT = 100;

	class MShader;
	class SkinnedMeshRenderer;

	class Skeleton
	{
		friend class MANA3DEngine;

	public:
		void Update( const SkinnedMeshRenderer* smRenderer );

		void SetRootJoint( const Joint* rootJoint );
		const Joint* GetRootJoint();

		const int& GetJointCount();

	private:
		Skeleton( const int& jointCount, const aiMatrix4x4& globalInverseTransform, const std::map<std::string, Animation*>& animationMap, const int& skinnedMeshCount );
		~Skeleton();


		void CalculateJointsTransformationAt( std::map<int, glm::mat4>& transformtation, const float& timeInSeconds );

		void ReadSkeletonHeirarchy( const float& animationTime, const Joint* joint, const aiMatrix4x4& parentTransform, std::map<int, glm::mat4>& jointsTransformation );
		
		aiQuaternion CalcInterpolatedRotation( const float& animationTime, const KeyFrame* keyframe ) const;
		unsigned int FindRotation( const float& animationTime, const KeyFrame* keyframe ) const;

		aiVector3D CalcInterpolatedScaling( const float& p_animation_time, const KeyFrame* keyframe ) const;
		unsigned int FindScaling( const float& p_animation_time, const KeyFrame* keyframe ) const;

		aiVector3D CalcInterpolatedPosition( const float& p_animation_time, const KeyFrame* keyframe ) const;
		unsigned int FindPosition( const float& p_animation_time, const KeyFrame* keyframe ) const;



		int jointCount;

		const Joint* rootJoint;

		std::map<std::string, Animation*> animationMap;
		Animation* currentAnimation;

		aiMatrix4x4 globalInverseTransform;

		MShader* skinnedShader;

		int skinnedMeshRenderersCount;
		int callsForUpdateCount;

	private:
		static vector<Skeleton*> allSkeletons;

		static void Destroy( const Skeleton* skeleton );
		static void DestroyAll();

	public:
		static Skeleton* Create( const int& jointCount, const aiMatrix4x4& globalInverseTransform, const std::map<std::string, Animation*>& animationMap, const int& skinnedMeshCount );

	};
}

#endif

