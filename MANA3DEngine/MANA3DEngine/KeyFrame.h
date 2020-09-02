#ifndef KEY_FRAME_H
#define KEY_FRAME_H

#include <assimp/anim.h>
#include <vector>
#include "MString.h"


namespace MANA3D
{
	struct VectorKey
	{
		double time;
		aiVector3D value;
	};

	struct RotationKey
	{
		double time;
		aiQuaternion value;
	};


	class KeyFrame
	{
	public:
		KeyFrame( const MString& jointName, const std::vector<VectorKey>& positionKeyframes, const std::vector<RotationKey>& rotationKeyframes, const std::vector<VectorKey>& scaleKeyframes );
		~KeyFrame();


		const int& GetPositionKeyframesCount() const;
		const int& GetRotationKeyframesCount() const;
		const int& GetScaleKeyframesCount() const;

		const VectorKey& GetPositionKeyframeAt( const int& index ) const;
		const RotationKey& GetRotationKeyframeAt( const int& index ) const;
		const VectorKey& GetScaleKeyframeAt( const int& index ) const;


	private:
		MString jointName;

		int positionKeyframesCount;
		int rotationKeyframesCount;
		int scaleKeyframesCount;
		std::vector<VectorKey> positionKeyframes;
		std::vector<RotationKey> rotationKeyframes;
		std::vector<VectorKey> scaleKeyframes;
	};
}

#endif