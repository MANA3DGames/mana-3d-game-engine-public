#ifndef EDITOR_SCENE3D_MANAGER_H
#define EDITOR_SCENE3D_MANAGER_H

#include <vector>
using std::vector;

#include "MString.h"

namespace MANA3D
{
	class EditorGuiManager;
	class Editor3DCamera;
	class Editor3DGrid;
	class GameObject;
	class MMaterial;
	class MShader;
	class MMesh;

	class Editor3DSceneManager
	{
		friend class MANA3DEngine;

	private:
		Editor3DSceneManager( MMaterial* gridMat, MShader* gridShader, MMesh* gridMesh, MMesh* crossGridMesh );
		~Editor3DSceneManager();


		void SetEditorGuiManager( EditorGuiManager* mana3DEditorGUI );

		void SetEditorCamAsMain();


		void SetMainGridColor();
		void SetCrossGridColor();

		GameObject* GetMainGridGO();
		GameObject* GetCrossGridGO();


		EditorGuiManager* mana3DEditorGUI;
		Editor3DGrid* editor3DGrid;
		Editor3DCamera* editorCam;

	private:
		static vector<GameObject*> editorGameObjects;


		static void Update();
	};
}


#endif // !EDITOR_SCENE3D_MANAGER_H
