#ifndef MANA3D_EDITOR_GUI_MENU_BAR_H
#define MANA3D_EDITOR_GUI_MENU_BAR_H

#include "EditorGuiWindow.h"


namespace MANA3D
{
	class EditorGuiMenuBar : public EditorGuiWindow
	{
		friend class EditorGuiManager;

	protected:
		EditorGuiMenuBar( EditorGuiManager* editorGuiManager );
		virtual ~EditorGuiMenuBar(); 

		void Display();
	};
}

#endif