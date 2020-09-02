#ifndef MPRIMITIVE_H
#define MPRIMITIVE_H

#include "GameObject.h"
#include "MeshRenderer.h"
#include "MMesh.h"
using namespace MANA3D;

#include <vector>
using std::vector;

namespace MANA3D
{
	class MMesh;

	class MPrimitive
	{
		friend class MANA3DEngine;

	protected:
		MPrimitive( const char* str );
		virtual ~MPrimitive();

		void AddMeshRenderer( MMesh* mesh );

		GameObject* gameObject;


	public:
		GameObject* GetGameObject();



	private:// Static
		static void CreateAllMeshes();
		static void CreateRectangleMesh();
		static void CreateCubeMesh();
		static void CreateSphereMesh();

		static void DestroyAll();

	protected: // Static
		static vector<MPrimitive*> scenePrimitives;
		static vector<MPrimitive*> runtimePrimitives;

		static MMesh* rectangleMesh;
		static MMesh* cubeMesh;
		static MMesh* sphereMesh;

	public:	// Static

		static void Destroy( MPrimitive* );
	};

}

#endif // !MPRIMITIVE_H

