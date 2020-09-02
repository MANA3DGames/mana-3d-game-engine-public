#ifndef EDITOR_GUI_SETTINGS_WINDOW_H
#define EDITOR_GUI_SETTINGS_WINDOW_H

namespace MANA3D
{
	class EditorGuiManager;

	class EditorGuiSettingsWindow
	{
		friend class EditorGuiUtilitesWindow;

	private:
		EditorGuiSettingsWindow( EditorGuiManager* editorGuiManager );
		~EditorGuiSettingsWindow();

		void Display();


		EditorGuiManager* editorGuiManager;
	};
}

#endif // !EDITOR_GUI_SETTINGS_WINDOW_H
