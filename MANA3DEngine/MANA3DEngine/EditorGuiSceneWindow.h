#ifndef MANA3D_EDITOR_GUI_SCENE_WINDOW_H
#define MANA3D_EDITOR_GUI_SCENE_WINDOW_H

#include "EditorGuiWindow.h"

namespace MANA3D
{
	class EditorGuiSceneWindow : public EditorGuiWindow
	{
		friend class EditorGuiManager;

	protected:
		EditorGuiSceneWindow( EditorGuiManager* editorGuiManager );
		virtual ~EditorGuiSceneWindow();

		void Display( const unsigned int& textureColorbuffer, const int& drawCallsCount );
		void DisplayStatistic( bool* p_open, const int& drawCallsCount );


	private:
		void MakeDragDropTargetForPrefabAssets( const ImVec2& size );

	};
}

#endif // !MANA3D_EDITOR_GUI_SCENE_WINDOW_H

