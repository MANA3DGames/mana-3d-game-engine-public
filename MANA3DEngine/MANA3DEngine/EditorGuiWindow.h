#ifndef MANA3D_EDITOR_GUI_WINDOWS_H
#define MANA3D_EDITOR_GUI_WINDOWS_H

#include <ImGui/imgui.h>

namespace MANA3D
{
	class EditorGuiWindow
	{
		friend class EditorGuiManager;

	protected:
		EditorGuiWindow( EditorGuiManager* editorGuiManager );
		virtual ~EditorGuiWindow();
		
		virtual void UpdateWindProperties();

		virtual float GetTopOffsetY();
		virtual bool GetIsFocused();

		virtual ImVec2 GetWindPos();
		virtual ImVec2 GetWindSize();




		float topOffsetY;

		bool isFocused;

		ImVec2 windPos;
		ImVec2 windSize;

		EditorGuiManager* editorGuiManager;
	};
}

#endif