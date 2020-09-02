#include "Skeleton.h"
#include "KeyFrame.h"
#include "Math3D.h"
#include "MShader.h"
#include "Time.h"
#include "Debug.h"
using namespace MANA3D;
using namespace glm;

#include <glad/glad.h>

using std::map;
using std::string;


Skeleton::Skeleton( const int& jointCount, const aiMatrix4x4& globalInverseTransform, const map<string, Animation*>& animationMap, const int& skinnedMeshCount ) :
    jointCount( jointCount ), 
    globalInverseTransform( globalInverseTransform ), 
    animationMap( animationMap ), 
    skinnedMeshRenderersCount( skinnedMeshCount ), 
    rootJoint( nullptr ), 
    callsForUpdateCount( 0 )
{
    if ( this->animationMap.size() > 0 )
        currentAnimation = this->animationMap.begin()->second;

    skinnedShader = MShader::Get( "DiffuseSkinned" );
}

Skeleton::~Skeleton()
{
	delete rootJoint;

    while ( animationMap.size() > 0 )
	{
        map<string, Animation*>::iterator it = animationMap.begin();
        Animation* anim = it->second;
        animationMap.erase( it );
		delete anim;
	}
    animationMap.clear();
}


void Skeleton::SetRootJoint( const Joint* rootJoint )
{
    this->rootJoint = rootJoint;
}
const Joint* Skeleton::GetRootJoint()
{
    return rootJoint;
}

const int& Skeleton::GetJointCount()
{
    return jointCount;
}




void Skeleton::Update( const SkinnedMeshRenderer* smRenderer )
{
    if ( callsForUpdateCount == skinnedMeshRenderersCount )
        callsForUpdateCount = 0;

    if ( callsForUpdateCount > 0 )
    {
        callsForUpdateCount++;
        return;
    }

    callsForUpdateCount = 1;



    static float currentTime = 0.0f;

	float time = currentTime / 1000.0f;

    skinnedShader->Use();

    map<int, mat4> jointTransforms;
    CalculateJointsTransformationAt( jointTransforms, time );
	for ( map<int, mat4>::iterator it = jointTransforms.begin(); it != jointTransforms.end(); ++it )
	{
		MString str( "jointTrans[%i]", it->first );
		const char* cStr = str.GetCStr();
        skinnedShader->SetMat4( MString( "%s.matrix", cStr ), it->second );
	}
	jointTransforms.clear();

    currentTime += Time::GetTime();
}

void Skeleton::CalculateJointsTransformationAt( map<int, mat4>& transformtation, const float& timeInSeconds )
{
    aiMatrix4x4 Identity;

    float timeInTicks = timeInSeconds * currentAnimation->GetTicksPerSecond();
    float animationTime = fmod( timeInTicks, currentAnimation->GetDuration() );

    ReadSkeletonHeirarchy( animationTime, rootJoint, Identity, transformtation );
}

void Skeleton::ReadSkeletonHeirarchy( const float& animationTime, const Joint* joint, const aiMatrix4x4& parentTransform, map<int, mat4>& jointsTransformation )
{
    string jointName( joint->GetName().GetStr() );

    aiMatrix4x4 nodeTransformation = joint->GetTransformAIMatrix();

    const KeyFrame* keyframe = currentAnimation->GetKeyFrame( jointName );

    if ( keyframe != nullptr ) 
    {
        // Interpolate scaling and generate scaling transformation matrix
        aiVector3D scale = CalcInterpolatedScaling( animationTime, keyframe );
        aiMatrix4x4 scalingMatrix;
        aiMatrix4x4::Scaling( scale, scalingMatrix );

        // Interpolate rotation and generate rotation transformation matrix
        aiQuaternion rotation = CalcInterpolatedRotation( animationTime, keyframe );
        aiMatrix4x4 rotationMatrix( rotation.GetMatrix() );

        // Interpolate translation and generate translation transformation matrix
        aiVector3D translation = CalcInterpolatedPosition( animationTime, keyframe );
        aiMatrix4x4 translateMatrix;
        aiMatrix4x4::Translation( translation, translateMatrix );

        // Combine the above transformations
        nodeTransformation = translateMatrix * rotationMatrix * scalingMatrix;
    }

    aiMatrix4x4 jointGlobalTransformation = parentTransform * nodeTransformation;
    aiMatrix4x4 jointFinalTransformation = globalInverseTransform * jointGlobalTransformation * joint->GetOffsetAIMatrix();

    mat4 finalTransformMatrix;
    Math3D::ConvertToMat4( jointFinalTransformation, finalTransformMatrix );

    jointsTransformation.insert( std::pair<int, mat4>( joint->GetIndex(), finalTransformMatrix ) );

    if ( joint->GetChildrenCount() > 0 )
    {
        vector<Joint*> children = joint->GetChildren();
        for ( vector<Joint*>::iterator it = children.begin(); it != children.end(); ++it )
            ReadSkeletonHeirarchy( animationTime, *it, jointGlobalTransformation, jointsTransformation );
    }
}



aiVector3D Skeleton::CalcInterpolatedScaling( const float& p_animation_time, const KeyFrame* keyframe ) const
{
	if (keyframe->GetScaleKeyframesCount() == 1)
	{
		return keyframe->GetScaleKeyframeAt(0).value;
	}

	uint scaling_index = FindScaling(p_animation_time, keyframe);
	uint next_scaling_index = scaling_index + 1; 
	assert( next_scaling_index < (uint)keyframe->GetScaleKeyframesCount() );
	
	float delta_time = (float)(keyframe->GetScaleKeyframeAt(next_scaling_index).time - keyframe->GetScaleKeyframeAt(scaling_index).time);
	
	float  factor = (p_animation_time - (float)keyframe->GetScaleKeyframeAt(scaling_index).time) / delta_time;
	assert(factor >= 0.0f && factor <= 1.0f);
	aiVector3D start = keyframe->GetScaleKeyframeAt(scaling_index).value;
	aiVector3D end = keyframe->GetScaleKeyframeAt(next_scaling_index).value;
	aiVector3D delta = end - start;

	return start + factor * delta;
}
unsigned int Skeleton::FindScaling( const float& p_animation_time, const KeyFrame* keyframe ) const
{
    uint count = keyframe->GetScaleKeyframesCount() - 1;
    for (uint i = 0; i < count; i++)
    {
        if (p_animation_time < (float)keyframe->GetScaleKeyframeAt(i + 1).time)
        {
            return i;
        }
    }

    assert(0);
    return 0;
}

aiQuaternion Skeleton::CalcInterpolatedRotation( const float& animationTime, const KeyFrame* keyframe ) const
{
    aiQuaternion Out;

    if ( keyframe->GetRotationKeyframesCount() == 0 )
        return Out;

    // we need at least two values to interpolate...
    if ( keyframe->GetRotationKeyframesCount() == 1 ) 
    {
        return keyframe->GetRotationKeyframeAt( 0 ).value;
    }

    uint rotationIndex = FindRotation( animationTime, keyframe );
    uint nextRotationIndex = ( rotationIndex + 1 );
    assert( nextRotationIndex < (uint)keyframe->GetRotationKeyframesCount() );

    float deltaTime = (float)( keyframe->GetRotationKeyframeAt( nextRotationIndex ).time - keyframe->GetRotationKeyframeAt( rotationIndex ).time );
    float factor = ( animationTime - (float)keyframe->GetRotationKeyframeAt( rotationIndex ).time ) / deltaTime;
    assert( factor >= 0.0f && factor <= 1.0f );

    const aiQuaternion& startRotationQ = keyframe->GetRotationKeyframeAt( rotationIndex ).value;
    const aiQuaternion& endRotationQ = keyframe->GetRotationKeyframeAt( nextRotationIndex ).value;
    aiQuaternion::Interpolate( Out, startRotationQ, endRotationQ, factor );
    Out = Out.Normalize();

    return Out;
}
unsigned int Skeleton::FindRotation( const float& animationTime, const KeyFrame* keyframe ) const
{
    int count = keyframe->GetRotationKeyframesCount();
    assert( count > 0 );

    for ( int i = 0; i < count - 1; i++ )
    {
        if ( animationTime < (float)keyframe->GetRotationKeyframeAt( i + 1 ).time )
        {
            return i;
        }
    }

    assert(0);
    return 0;
}

aiVector3D Skeleton::CalcInterpolatedPosition( const float& p_animation_time, const KeyFrame* keyframe ) const
{
    if (keyframe->GetPositionKeyframesCount() == 1)
    {
        return keyframe->GetPositionKeyframeAt(0).value;
    }

    uint position_index = FindPosition(p_animation_time, keyframe);
    uint next_position_index = position_index + 1; 
    assert( next_position_index < (uint)keyframe->GetPositionKeyframesCount() );
    
    float delta_time = (float)(keyframe->GetPositionKeyframeAt(next_position_index).time - keyframe->GetPositionKeyframeAt(position_index).time);
    
    float factor = (p_animation_time - (float)keyframe->GetPositionKeyframeAt(position_index).time) / delta_time;
    assert(factor >= 0.0f && factor <= 1.0f);
    aiVector3D start = keyframe->GetPositionKeyframeAt(position_index).value;
    aiVector3D end = keyframe->GetPositionKeyframeAt(next_position_index).value;
    aiVector3D delta = end - start;

    return start + factor * delta;
}
unsigned int Skeleton::FindPosition( const float& p_animation_time, const KeyFrame* keyframe) const
{
    uint count = keyframe->GetPositionKeyframesCount() - 1;
    for (uint i = 0; i < count; i++)
    {
        if (p_animation_time < (float)keyframe->GetPositionKeyframeAt(i + 1).time)
        {
            return i; 
        }
    }

    assert(0);
    return 0;
}



vector<Skeleton*> Skeleton::allSkeletons;

Skeleton* Skeleton::Create( const int& jointCount, const aiMatrix4x4& globalInverseTransform, const map<string, Animation*>& animationMap, const int& skinnedMeshCount )
{
    Skeleton* skeleton = new Skeleton( jointCount, globalInverseTransform, animationMap, skinnedMeshCount );
    allSkeletons.push_back( skeleton );
    return skeleton;
}

void Skeleton::Destroy( const Skeleton* skeleton )
{
    delete skeleton;
}
void Skeleton::DestroyAll()
{
    for ( vector<Skeleton*>::iterator it = allSkeletons.begin(); it != allSkeletons.end(); ++it )
        Destroy( *it );

    allSkeletons.clear();
}