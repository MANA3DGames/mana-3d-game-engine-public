#include "EditorGuiUtilitesWindow.h"
#include "EditorGuiLogWindow.h"
#include "EditorGuiAssetsWindow.h"
#include "EditorGuiSettingsWindow.h"
#include "MMaterial.h"
#include "MTexture.h"
#include "EditorGuiManager.h"
#include "Screen.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "MMesh.h"
#include "GraphicSettings.h"
using namespace MANA3D;

using std::map;
using std::string;
using std::list;


EditorGuiUtilitesWindow::EditorGuiUtilitesWindow( EditorGuiManager* editorGuiManager ) : EditorGuiWindow( editorGuiManager )
{
	guiLogWindow = new EditorGuiLogWindow;
	guiAssetsWindow = new EditorGuiAssetsWindow( editorGuiManager );
	guiSettingsWindow = new EditorGuiSettingsWindow( editorGuiManager );
}

EditorGuiUtilitesWindow::~EditorGuiUtilitesWindow() 
{
	if ( guiLogWindow != nullptr )
		delete guiLogWindow;

	if ( guiAssetsWindow != nullptr )
		delete guiAssetsWindow;
}



void EditorGuiUtilitesWindow::Display( map<string, MTexture*>& privateTexturesMap )
{
	bool isUtilitesOpened = true;

	ImGui::Begin( "Utilites ", &isUtilitesOpened, ImVec2( 0 , 0 ), 1.0f,
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove | 
		ImGuiWindowFlags_NoTitleBar );

	ImGui::SetWindowPos( ImVec2( 0, editorGuiManager->GetSceneWindowSize().y + editorGuiManager->GetSceneWindowTopOffsetY() ) );
	ImGui::SetWindowSize( ImVec2( editorGuiManager->GetSceneHierarchyWindowSize().x + editorGuiManager->GetSceneWindowSize().x, 
								  Screen::GetHeight() - editorGuiManager->GetSceneWindowSize().y - editorGuiManager->GetSceneWindowTopOffsetY() ) );

	UpdateWindProperties();

	ImGui::BeginTabBar( "Utilites#left_tabs_bar" );

	ImGuiTabItemFlags tabFlags = 0;
	// Check if we've selected a texture from inspector MeshRenderer Component.
	if ( editorGuiManager->GetTextureToBeSelectedFromAsset() != nullptr )
		// Select Assets Browser by default to open texture window.
		tabFlags = ImGuiTabItemFlags_SetSelected;

	if ( ImGui::BeginTabItem( "Assets Browser", (bool*)0, tabFlags ) )
	{
		guiAssetsWindow->Display( privateTexturesMap );
		ImGui::EndTabItem();
	}

	if ( ImGui::BeginTabItem( "Graphic Settings" ) )
	{
		guiSettingsWindow->Display();
		ImGui::EndTabItem();
	}

	if ( ImGui::BeginTabItem( "Console" ) )
	{
		guiLogWindow->Display();
		ImGui::EndTabItem();
	}

	ImGui::EndTabBar();

	ImGui::End();
}
