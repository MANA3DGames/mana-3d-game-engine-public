#ifndef SKINNED_MESH_RENDERER_H
#define SKINNED_MESH_RENDERER_H

#include "MeshRenderer.h"
#include "MSkinnedMesh.h"

namespace MANA3D
{
	class Skeleton;

	class SkinnedMeshRenderer : public MeshRenderer
	{
		friend class SceneManager;
		friend class EditorGuiManager;
		friend class GameObject;
		friend class Camera;

	public:
		void SetSkinnedMesh( MSkinnedMesh* mesh );
		MSkinnedMesh* GetSkinnedMesh();

		void SetSkeleton( Skeleton* skeleton );

	protected:
		SkinnedMeshRenderer();
		virtual ~SkinnedMeshRenderer();

		void Render() override;

		void CopyFrom( Component* ) override;

	private:
		MSkinnedMesh* mesh;

		Skeleton* skeleton;

	};
}

#endif // !SKINNED_MESH_RENDERER_H

