#ifndef MANA3D_EDITOR_GUI_ASSETS_OVERVIEW_WINDOW_H
#define MANA3D_EDITOR_GUI_ASSETS_OVERVIEW_WINDOW_H

#include "EditorGuiWindow.h"

namespace MANA3D
{
	class EditorGuiAssetsOverviewWindow : public EditorGuiWindow
	{
		friend class EditorGuiManager;

	protected:
		EditorGuiAssetsOverviewWindow( EditorGuiManager* editorGuiManager );
		virtual ~EditorGuiAssetsOverviewWindow();

		void Display();
	};
}

#endif // !MANA3D_EDITOR_GUI_ASSETS_OVERVIEW_WINDOW_H
