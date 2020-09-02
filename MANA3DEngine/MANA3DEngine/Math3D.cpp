#include "Math3D.h"
using namespace MANA3D;

using namespace glm;


const vec3 Math3D::VECTOR3_ZERO( 0, 0, 0 );
const vec3 Math3D::VECTOR3_ONE( 1, 1, 1 );
const vec3 Math3D::VECTOR3_UP( 0, 1, 0 );
const vec3 Math3D::VECTOR3_DOWN( 0, -1, 0 );
const vec3 Math3D::VECTOR3_RIGHT( 1, 0, 0 );
const vec3 Math3D::VECTOR3_LEFT( -1, 0, 0 );
const vec3 Math3D::VECTOR3_FORWARD( 0, 0, 1 );
const vec3 Math3D::VECTOR3_BACKWARD( 0, 0, -1 );

const double PI = 3.14159265358979323846;


vec3 Math3D::Normalize( const vec3& v )
{
	return normalize( v );
}


vec3 Math3D::Cross( const vec3& v1, const vec3& v2 )
{
	return cross( v1, v2 );
}


vec3 Math3D::GetPointAtDistance( const vec3& sourcePoint, const vec3& direction, const float& distance )
{
	vec3 point = Normalize( direction );
	point *= distance;
	point.x += sourcePoint.x;
	point.y += sourcePoint.y;
	point.z += sourcePoint.z;
	return point;
}

// Quaternion
quat Math3D::LookRotation( const vec3& lookAt, const vec3& upDirection )
{
	vec3 forward = lookAt;
	vec3 up = upDirection;

	forward = Normalize( forward );
	up = Normalize( up );

	vec3 right = Cross( up, forward );

	float m00 = right.x;
	float m01 = up.x;
	float m02 = forward.x;
	float m10 = right.y;
	float m11 = up.y;
	float m12 = forward.y;
	float m20 = right.z;
	float m21 = up.z;
	float m22 = forward.z;

	quat ret;
	ret.w = sqrtf( 1.0f + m00 + m11 + m22 ) * 0.5f;
	float w4_recip = 1.0f / ( 4.0f * ret.w );
	ret.x = ( m21 - m12 ) * w4_recip;
	ret.y = ( m02 - m20 ) * w4_recip;
	ret.z = ( m10 - m01 ) * w4_recip;

	return ret;
}


//// copy from Row-major to Column major matrix
//// i.e. from aiMatrix4x4 to glm::mat4
//template <typename RM, typename CM>
//void Math3D::ConvertToMat4( const RM& from, CM& to )
//{
//	to[0][0] = from.a1; to[1][0] = from.a2;
//	to[2][0] = from.a3; to[3][0] = from.a4;
//	to[0][1] = from.b1; to[1][1] = from.b2;
//	to[2][1] = from.b3; to[3][1] = from.b4;
//	to[0][2] = from.c1; to[1][2] = from.c2;
//	to[2][2] = from.c3; to[3][2] = from.c4;
//	to[0][3] = from.d1; to[1][3] = from.d2;
//	to[2][3] = from.d3; to[3][3] = from.d4;
//}


void Math3D::ConvertToMat4( const aiMatrix4x4& from, mat4& to )
{
	to[0][0] = from.a1; to[1][0] = from.a2;
	to[2][0] = from.a3; to[3][0] = from.a4;
	to[0][1] = from.b1; to[1][1] = from.b2;
	to[2][1] = from.b3; to[3][1] = from.b4;
	to[0][2] = from.c1; to[1][2] = from.c2;
	to[2][2] = from.c3; to[3][2] = from.c4;
	to[0][3] = from.d1; to[1][3] = from.d2;
	to[2][3] = from.d3; to[3][3] = from.d4;
}