#include "EditorGuiWindow.h"
#include "Debug.h"
using namespace MANA3D;

EditorGuiWindow::EditorGuiWindow( EditorGuiManager* editorGuiManager )
	: editorGuiManager( editorGuiManager ),
	  topOffsetY( 0.0f ),
	  isFocused( false )
{ 
}

EditorGuiWindow::~EditorGuiWindow() {}



void EditorGuiWindow::UpdateWindProperties()
{
	windSize = ImGui::GetWindowSize();
	windPos = ImGui::GetWindowPos();
	isFocused = ImGui::IsWindowFocused();
}


float EditorGuiWindow::GetTopOffsetY()
{
	return topOffsetY;
}
bool EditorGuiWindow::GetIsFocused()
{
	return isFocused;
}

ImVec2 EditorGuiWindow::GetWindPos()
{
	return windPos;
}
ImVec2 EditorGuiWindow::GetWindSize()
{
	return windSize;
}