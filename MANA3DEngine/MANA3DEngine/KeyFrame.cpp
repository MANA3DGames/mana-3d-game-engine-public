#include "KeyFrame.h"
using namespace MANA3D;

using namespace Assimp;
using std::vector;


KeyFrame::KeyFrame( const MString& jointName, const vector<VectorKey>& positionKeyframes, const vector<RotationKey>& rotationKeyframes, const vector<VectorKey>& scaleKeyframes ) :
	jointName( jointName ),
	positionKeyframes( positionKeyframes ),
	rotationKeyframes( rotationKeyframes ),
	scaleKeyframes( scaleKeyframes )
{
	positionKeyframesCount = positionKeyframes.size();
	rotationKeyframesCount = rotationKeyframes.size();;
	scaleKeyframesCount = scaleKeyframes.size();
}

KeyFrame::~KeyFrame()
{
	positionKeyframes.clear();
	rotationKeyframes.clear();
	scaleKeyframes.clear();
}



const int& KeyFrame::GetPositionKeyframesCount() const
{
	return positionKeyframesCount;
}
const int& KeyFrame::GetRotationKeyframesCount() const
{
	return rotationKeyframesCount;
}
const int& KeyFrame::GetScaleKeyframesCount() const
{
	return scaleKeyframesCount;
}




const VectorKey& KeyFrame::GetPositionKeyframeAt( const int& index ) const
{
	return positionKeyframes[index];
}
const RotationKey& KeyFrame::GetRotationKeyframeAt( const int& index ) const
{
	return rotationKeyframes[index];
}
const VectorKey& KeyFrame::GetScaleKeyframeAt( const int& index ) const
{
	return scaleKeyframes[index];
}