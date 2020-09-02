#ifndef EDITOR_3D_GRID_H
#define EDITOR_3D_GRID_H

namespace MANA3D
{
	class GameObject;
	class MeshRenderer;
	class MShader;
	class MMaterial;
	class MMesh;

	class Editor3DGrid
	{
		friend class Editor3DSceneManager;

	private:
		Editor3DGrid( GameObject* grid, GameObject* cross, MMaterial* gridMat, MShader* gridShader, MMesh* gridMesh, MMesh* crossGridMesh );
		~Editor3DGrid();

		void SetMainGridColor();
		void SetCrossGridColor();


		MShader* gridShader;

		GameObject* mainGridGameObject;
		MeshRenderer* mainGridMeshRenderer;

		GameObject* crossGameObject;
		MeshRenderer* crossMeshRenderer;

	};
}

#endif