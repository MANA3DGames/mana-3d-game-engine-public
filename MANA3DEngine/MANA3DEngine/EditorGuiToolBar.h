#ifndef MANA3D_EDITOR_GUI_TOOL_BAR_H
#define MANA3D_EDITOR_GUI_TOOL_BAR_H

#include "EditorGuiWindow.h"

#include <map>
#include <string>


namespace MANA3D
{
	class MTexture;

	class EditorGuiToolBar : public EditorGuiWindow
	{
		friend class EditorGuiManager;

	protected:
		EditorGuiToolBar( EditorGuiManager* editorGuiManager );
		virtual ~EditorGuiToolBar();

		void Display( std::map<std::string, MTexture*>& privateTexturesMap );
	};
}

#endif // !MANA3D_EDITOR_GUI_TOOL_BAR_H

