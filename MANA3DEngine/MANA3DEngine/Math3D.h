#ifndef MATH3D_H
#define MATH3D_H

#include <glm\glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/matrix4x4.h>


//# define M_PI           3.14159265358979323846

namespace MANA3D
{
	class Math3D
	{
	public:
		static glm::vec3 Normalize( const glm::vec3& );

		static glm::vec3 Cross( const glm::vec3& v1, const glm::vec3& v2 );

		static glm::vec3 GetPointAtDistance( const glm::vec3& sourcePoint, const glm::vec3& direction, const float& distance );

		static glm::quat LookRotation( const glm::vec3& lookAt, const glm::vec3& upDirection );




		///*template <typename RM, typename CM>
		//static void ConvertToMat4( const RM& from, CM& to );*/

		//// copy from Row-major to Column major matrix
		//// i.e. from aiMatrix4x4 to glm::mat4
		//template <typename RM, typename CM>
		//static void ConvertToMat4( const RM& from, CM& to )
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
		static void ConvertToMat4( const aiMatrix4x4& from, glm::mat4& to );



		static const glm::vec3 VECTOR3_ZERO;				// Shorthand for writing Vector3( 0,  0,  0 ).
		static const glm::vec3 VECTOR3_ONE;					// Shorthand for writing Vector3( 1,  1,  1 ).
		static const glm::vec3 VECTOR3_UP;					// Shorthand for writing Vector3( 0,  1,  0 ).
		static const glm::vec3 VECTOR3_DOWN;				// Shorthand for writing Vector3( 0, -1,  0 ).
		static const glm::vec3 VECTOR3_RIGHT;				// Shorthand for writing Vector3( 1,  0,  0 ).
		static const glm::vec3 VECTOR3_LEFT;				// Shorthand for writing Vector3(-1,  0,  0 ).
		static const glm::vec3 VECTOR3_FORWARD;				// Shorthand for writing Vector3( 0,  0,  1 ).
		static const glm::vec3 VECTOR3_BACKWARD;			// Shorthand for writing Vector3( 0,  0, -1 ).

		static const double PI;
	};
}

#endif