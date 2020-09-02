#ifndef MANA3D_EDITOR_GUI_SCENE_HIERARCHY_WINDOW_H
#define MANA3D_EDITOR_GUI_SCENE_HIERARCHY_WINDOW_H

#include "EditorGuiWindow.h"
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

namespace MANA3D
{
	class Transform;
	class MString;
	struct DragDropGameObject;

	class EditorGuiSceneHierarchyWindow : public EditorGuiWindow
	{
		friend class EditorGuiManager;

	protected:
		EditorGuiSceneHierarchyWindow( EditorGuiManager* editorGuiManager );
		virtual ~EditorGuiSceneHierarchyWindow();

		void Display();

	private:
		void DrawSceneHierarchyNodeChildren( Transform* root, int& id );

		// Only for internal use.
		void SetSceneHierarchyNode_clicked_ID( int id );


		void MakeDragDropSourceItem( const MString& ddID, const MString& label, const DragDropGameObject& data );
		void MakeDragDropTargetItem( const ImRect& rect, Transform* newParent );
		void OnDropItemOnSlot( const ImRect& rect, Transform* newParent );

		void MakeDragDropTargetForItemUnparenting();

		void MakeDragDropTargetForPrefabAssets();



		bool isFirstRun = true;

		// Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
		int sceneHierarchyNode_clicked_ID = -1;

		ImVec2 windSize;
	};
}

#endif // !MANA3D_EDITOR_GUI_SCENE_HIERARCHY_WINDOW_H

