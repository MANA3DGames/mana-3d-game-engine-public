#ifndef MANA3D_EDITOR_GUI_UTILITIES_WINDOW_H
#define MANA3D_EDITOR_GUI_UTILITIES_WINDOW_H

#include "EditorGuiWindow.h"

#include <map>
#include <string>

namespace MANA3D
{
	class MTexture;
	class EditorGuiLogWindow;
	class EditorGuiAssetsWindow;
	class EditorGuiSettingsWindow;

	class EditorGuiUtilitesWindow : public EditorGuiWindow
	{
		friend class EditorGuiManager;

	protected:
		EditorGuiUtilitesWindow( EditorGuiManager* editorGuiManager );
		virtual ~EditorGuiUtilitesWindow();

		void Display(std::map<std::string, MTexture*>& privateTexturesMap);

	private:
		EditorGuiLogWindow* guiLogWindow;
		EditorGuiAssetsWindow* guiAssetsWindow;
		EditorGuiSettingsWindow* guiSettingsWindow;

	};
}

#endif // !MANA3D_EDITOR_GUI_UTILITIES_WINDOW_H
