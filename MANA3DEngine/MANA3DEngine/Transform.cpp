#include <iostream>
#include "Transform.h"
#include "SceneManager.h"
using namespace MANA3D;
using namespace glm;

using std::list;


#pragma region

Transform::Transform() : Component()
{
	parent = nullptr;
}

Transform::~Transform()
{
	if ( parent != nullptr )
		SetParent( nullptr );
}

#pragma endregion [Constructor & Destructor]

#pragma region

void Transform::CopyFrom( Component* trans )
{
	SetLocalModelMatrix( ( (Transform*)trans )->GetLocalModelMatrix() );
}

void Transform::Update()
{
}

#pragma endregion [Virtual Function]

#pragma region 

void Transform::SetPosition( const vec3& pos )
{
	SetLocalPosition( pos );
}
void Transform::SetLocalPosition( const vec3& pos )
{
	modelMatrix[3][0] = pos.x;
	modelMatrix[3][1] = pos.y;
	modelMatrix[3][2] = pos.z;
}

void Transform::Translate( const vec3& val )
{
	mat4 mat;
	mat = translate( mat, val );
	modelMatrix = mat * modelMatrix;
}
void Transform::TranslateX( const float& val )
{
	mat4 mat;
	mat = translate( mat, vec3( val, 0.0f, 0.0f ) );
	modelMatrix = mat * modelMatrix;
}
void Transform::TranslateY( const float& val )
{
	mat4 mat;
	mat = translate( mat, vec3( 0.0f, val, 0.0f ) );
	modelMatrix = mat * modelMatrix;
}
void Transform::TranslateZ( const float& val )
{
	mat4 mat;
	mat = translate( mat, vec3( 0.0f, 0.0f, val ) );
	modelMatrix = mat * modelMatrix;
}

void Transform::TranslateLocally( const vec3& val )
{
	modelMatrix = translate( modelMatrix, val );
}
void Transform::TranslateXLocally( const float& val )
{
	modelMatrix = translate( modelMatrix, vec3( val, 0.0f, 0.0f ) );
}
void Transform::TranslateYLocally( const float& val )
{
	modelMatrix = translate( modelMatrix, vec3( 0.0f, val, 0.0f ) );
}
void Transform::TranslateZLocally( const float& val )
{
	modelMatrix = translate( modelMatrix, vec3( 0.0f, 0.0f, val ) );
}

vec3 GetPositionFromMatrix( const mat4& mat )
{
	return vec3(
		mat[3][0],
		mat[3][1],
		mat[3][2] );
}
vec3 GetPositionFromMatrix( const mat4& mat, const mat4& parent )
{
	mat4 gMat = mat * parent;
	return GetPositionFromMatrix( gMat );
}
vec3 Transform::GetPosition()
{
	if ( parent != nullptr )
		return GetPositionFromMatrix( modelMatrix, parent->GetModelMatrix() );
	else
		return GetLocalPosition();
}
vec3 Transform::GetLocalPosition()
{
	return GetPositionFromMatrix( modelMatrix );
}

#pragma endregion [Position & Translation]

#pragma region

void Transform::SetEulerAngles( const vec3& euler )
{
	SetEulerAngles( euler.x, euler.y, euler.z );
}
void Transform::SetEulerAngles( const float& x, const float& y, const float& z )
{
	SetLocalEulerAngles( x, y, z );
}

void Transform::Rotate( const vec3& euler )
{
	Rotate( euler.x, euler.y, euler.z );
}
void Transform::Rotate( const float& x, const float& y, const float& z )
{
	RotateX( x );
	RotateY( y );
	RotateZ( z );
}
void Transform::RotateX( const float& degree )
{
	mat4 matX;
	matX = rotate( matX, radians( degree ), vec3( 1, 0, 0 ) );
	modelMatrix = matX * modelMatrix;
}
void Transform::RotateY( const float& degree )
{
	mat4 matY;
	matY = rotate( matY, radians( degree ), vec3( 0, 1, 0 ) );
	modelMatrix = matY * modelMatrix;
}
void Transform::RotateZ( const float& degree )
{
	mat4 matZ;
	matZ = rotate( matZ, radians( degree ), vec3( 0, 0, 1 ) );
	modelMatrix = matZ * modelMatrix;
}

void Transform::RotateLocally( const vec3& euler )
{
	RotateLocally( euler.x, euler.y, euler.z );
}
void Transform::RotateLocally( const float& x, const float& y, const float& z )
{
	RotateXLocally( x );
	RotateYLocally( y );
	RotateZLocally( z );
}
void Transform::RotateXLocally( const float& degree )
{
	modelMatrix = rotate( modelMatrix, radians( degree ), vec3( 1, 0, 0 ) );
}
void Transform::RotateYLocally( const float& degree )
{
	modelMatrix = rotate( modelMatrix, radians( degree ), vec3( 0, 1, 0 ) );
}
void Transform::RotateZLocally( const float& degree )
{
	modelMatrix = rotate( modelMatrix, radians( degree ), vec3( 0, 0, 1 ) );
}

void Transform::SetRotation( const quat& rotation )
{
	SetLocalRotation( rotation );
}
void Transform::SetLocalRotation( const quat& rotation )
{
	vec3 scale = GetLocalScale();
	for ( int i = 0; i < 3; i++ )
	{
		for ( int j = 0; j < 3; j++ )
			modelMatrix[i][j] = 0;
	}
	modelMatrix[0][0] = 1;
	modelMatrix[1][1] = 1;
	modelMatrix[2][2] = 1;

	glm::mat4 RotationMatrix = glm::toMat4( rotation );
	modelMatrix = modelMatrix * RotationMatrix;

	mat4 scaleMat4;
	scaleMat4 = glm::scale( scaleMat4, scale );
	modelMatrix = modelMatrix * scaleMat4;
}
void Transform::SetLocalEulerAngles( const vec3& euler )
{
	SetLocalEulerAngles( euler.x, euler.y, euler.z );
}
void Transform::SetLocalEulerAngles( const float& x, const float& y, const float& z )
{
	mat4 rotationMatrix;
	rotationMatrix = rotate( rotationMatrix, radians( x ), vec3( 1, 0, 0 ) );
	rotationMatrix = rotate( rotationMatrix, radians( y ), vec3( 0, 1, 0 ) );
	rotationMatrix = rotate( rotationMatrix, radians( z ), vec3( 0, 0, 1 ) );
	SetLocalRotation( toQuat( rotationMatrix ) );
}

quat GetRotationFromMatrix( const mat4& mat )
{
	vec3 forward = vec3(
		mat[0][2],
		mat[1][2],
		mat[2][2] );

	vec3 upwards = vec3(
		mat[0][1],
		mat[1][1],
		mat[2][1] );

	//return LookRotation( forward, upwards );
	return conjugate( Math3D::LookRotation( forward, upwards ) );
}
quat GetRotationFromMatrix( const mat4& mat, const mat4& parent )
{
	mat4 gMat = mat * parent;
	return GetRotationFromMatrix( gMat );
}
quat Transform::GetRotation()
{
	if ( parent != nullptr )
		return GetRotationFromMatrix( modelMatrix, parent->GetModelMatrix() );
	else
		return GetLocalRotation();
}
quat Transform::GetLocalRotation()
{
	return GetRotationFromMatrix( modelMatrix );
}

vec3 GetEulerAnglesFromMatrix( const mat4& mat )
{
	vec3 euler = glm::eulerAngles( GetRotationFromMatrix( mat ) );//* 3.14159f / 180.f;
	euler = glm::degrees( euler );
	return euler;
}
vec3 GetEulerAnglesFromMatrix( const mat4& mat, const mat4& parent )
{
	mat4 gMat = mat * parent;
	return GetEulerAnglesFromMatrix( gMat );
}
vec3 Transform::GetEulerAngles()
{
	if ( parent != nullptr )
		return GetEulerAnglesFromMatrix( modelMatrix, parent->GetModelMatrix() );
	else
		return GetLocalEulerAngles();
}
vec3 Transform::GetLocalEulerAngles()
{
	return GetEulerAnglesFromMatrix( modelMatrix );
}

#pragma endregion [Rotation & Euler Angles]

#pragma region

void Transform::SetScale( const vec3& scale )
{
	SetLocalScale( scale );
}

void Transform::SetLocalScale( const vec3& scale )
{
	quat rotation = GetLocalRotation();
	for ( int i = 0; i < 3; i++ )
	{
		for ( int j = 0; j < 3; j++ )
		{
			modelMatrix[i][j] = 0;
		}
	}
	modelMatrix[0][0] = 1;
	modelMatrix[1][1] = 1;
	modelMatrix[2][2] = 1;

	mat4 scaleMat4;
	scaleMat4 = glm::scale( scaleMat4, scale );
	modelMatrix = modelMatrix * scaleMat4;

	glm::mat4 RotationMatrix = glm::toMat4( rotation );
	modelMatrix = modelMatrix * RotationMatrix;
}

void Transform::Scale( const vec3& scale )
{
	mat4 mat;
	mat = glm::scale( mat, scale );
	modelMatrix = mat * modelMatrix;
}
void Transform::ScaleX( const float& val )
{
	mat4 mat;
	mat = glm::scale( mat, vec3( val, 0.0f, 0.0f ) );
	modelMatrix = mat * modelMatrix;
}
void Transform::ScaleY( const float& val )
{
	mat4 mat;
	mat = glm::scale( mat, vec3(  0.0f, val, 0.0f ) );
	modelMatrix = mat * modelMatrix;
}
void Transform::ScaleZ( const float& val )
{
	mat4 mat;
	mat = glm::scale( mat, vec3( 0.0f, 0.0f, val ) );
	modelMatrix = mat * modelMatrix;
}

void Transform::ScaleLocally( const vec3& scale )
{
	modelMatrix = glm::scale( modelMatrix, scale );
}
void Transform::ScaleXLocally( const float& val )
{
	modelMatrix = glm::scale( modelMatrix, vec3( val, 0.0f, 0.0f ) );
}
void Transform::ScaleYLocally( const float& val )
{
	modelMatrix = glm::scale( modelMatrix, vec3( 0.0f, val, 0.0f ) );
}
void Transform::ScaleZLocally( const float& val )
{
	modelMatrix = glm::scale( modelMatrix, vec3( 0.0f, 0.0f, val ) );
}

vec3 GetScaleFromMatrix( const mat4& mat )
{
	vec3 scale;
	vec3 xScaleVec = vec3( mat[0][0], mat[1][0], mat[2][0] );
	scale.x = glm::length( xScaleVec );
	vec3 yScaleVec = vec3( mat[0][1], mat[1][1], mat[2][1] );
	scale.y = glm::length( yScaleVec );
	vec3 zScaleVec = vec3( mat[0][2], mat[1][2], mat[2][2] );
	scale.z = glm::length( zScaleVec );
	return scale;
}
vec3 GetScaleFromMatrix( const mat4& mat, const mat4& parent )
{
	mat4 gMat = mat * parent;
	return GetPositionFromMatrix( gMat );
}
vec3 Transform::GetScale()
{
	if ( parent != nullptr )
		return GetScaleFromMatrix( modelMatrix, parent->GetModelMatrix() );
	else
		return GetLocalScale();
}
vec3 Transform::GetLocalScale()
{
	return GetScaleFromMatrix( modelMatrix );
}

#pragma endregion [Scale]

#pragma region

// The right / forward / up vectors are the column vectors of the matrix T.
// You can either read them directly or get them by multiplying the matrix with
// ( 1, 0, 0, 0 ) ( right ) 
// ( 0, 1, 0, 0 ) ( up )
// ( 0, 0, 1, 0 ) ( for / backward )
// ( 0, 0, 0, 1 ) ( position )

vec3 Transform::GetRight()
{
	return vec3(
		modelMatrix[0][0],
		modelMatrix[0][1],
		modelMatrix[0][2] );
}
vec3 Transform::GetUp()
{
	return vec3(
		modelMatrix[1][0],
		modelMatrix[1][1],
		modelMatrix[1][2] );
}
vec3 Transform::GetForward()
{
	return vec3(
		modelMatrix[2][0],
		modelMatrix[2][1],
		modelMatrix[2][2] );
}


// *** WARNING: Only use these 3 functions for camera transform ***
void Transform::SetRight( const vec3& right )
{
	modelMatrix[0][0] = right[0];
	modelMatrix[0][1] = right[1];
	modelMatrix[0][2] = right[2];
}
void Transform::SetUp( const vec3& up )
{
	modelMatrix[1][0] = up[0];
	modelMatrix[1][1] = up[1];
	modelMatrix[1][2] = up[2];
}
void Transform::SetForward( const vec3& forward )
{
	modelMatrix[2][0] = forward[0];
	modelMatrix[2][1] = forward[1];
	modelMatrix[2][2] = forward[2];
}

#pragma endregion [Direction]

#pragma region

void Transform::SetLocalModelMatrix( const mat4& modelMat4 )
{
	modelMatrix = modelMat4;
}

mat4 Transform::GetModelMatrix()
{
	if ( parent != nullptr )
		return modelMatrix * parent->GetModelMatrix();
	else
		return modelMatrix;
}
mat4 Transform::GetLocalModelMatrix()
{
	return modelMatrix;
}

void Transform::DecomposeModelMatrix( vec3& scale, quat& rotation, vec3& translation, vec3& skew, vec4& perspective )
{
	decompose( modelMatrix, scale, rotation, translation, skew, perspective );
	rotation = conjugate( rotation );
}

void Transform::PrintModelMatrix()
{
	for ( int x = 0; x < 4; x++ )
	{
		for ( int y = 0; y < 4; y++ )
		{
			std::cout << modelMatrix[x][y] << "	";
		}

		std::cout << std::endl;
	}
}

#pragma endregion [Model Matrix]

#pragma region

void Transform::SetParent( Transform *newParent )
{
	// Check if the new parent is me or it is my current parent.
	if ( newParent == this || newParent == parent )
		return;

	// Make sure the new parent is not one of our descendants.
	// Root
	//   |_____ Parent1
	//   |          |_______ Child1
	//   |              
	//   |_____ Parent2 
	//              |_______ Child1
	//              |_______ Child2
	//
	// Parent1, parent2, child1 or child2 cannot be a new parent for Root.
	Transform* temp = newParent;
	while ( temp != nullptr )
	{
		// Check if parent is the same as my transform.
		if ( temp->parent == this )
		{
			Debug::LogError( "Invalid parenting operaiton" );
			return;
		}

		// Move up one level till we reach the root.
		temp = temp->parent;
	}

	// Remove reference from current parent.
	if ( parent != nullptr )
	{
		// Remove my transfrom reference from current parent's children list.
		parent->children.remove( this );

		// Keep current world transformation as it is. 
		modelMatrix *= parent->GetModelMatrix();
	}
	
	// Add me to the new parent.
	if ( newParent != nullptr )
	{
		// Set my new parent to the new given parent.
		parent = newParent;

		// Add me to the new parent's children list.
		parent->children.push_back( this );
	}
	else
		// There is no new parent.
		parent = nullptr;
}
Transform* Transform::GetParent()
{
	return parent;
}


Transform* Transform::GetChild( const unsigned int& index )
{
	// Make sure that the given index is less than the total count of children.
	if ( children.size() > index )
	{
		// First, get the first iterator.
		list<Transform*>::iterator it = children.begin();
		// Advance first iterator (it) to the target index [it + index].
		advance( it, index );
		// 'it' points to the element at index 'index'
		return *it;
	}

	// Couldn't find a child with the given index.
	return nullptr;
}

Transform* Transform::FindChild( const MString &name )
{
	for ( list<Transform*>::iterator it = children.begin(); it != children.end(); ++it )
	{
		if ( ( *it )->gameObject->name == name )
			return *it;
	}

	return nullptr;
}

list<Transform*>& Transform::GetChildren()
{
	return children;
}

size_t Transform::GetChildCount()
{
	return children.size();
}

void Transform::DetachChildren()
{
	for ( list<Transform*>::iterator it = children.begin(); it != children.end(); ++it )
	{
		( *it )->modelMatrix *= ( *it )->parent->GetModelMatrix();
		( *it )->parent = nullptr;
	}

	children.clear();
}

#pragma endregion [Parent & Children]
