#include "EditorGuiToolBar.h"
#include "MTexture.h"
#include "EditorGuiManager.h"
#include "Screen.h"
using namespace MANA3D;

using std::map;
using std::string;

EditorGuiToolBar::EditorGuiToolBar( EditorGuiManager* editorGuiManager ) : EditorGuiWindow( editorGuiManager )
{
}

EditorGuiToolBar::~EditorGuiToolBar() {}



void EditorGuiToolBar::Display( map<string, MTexture*>& privateTexturesMap )
{
	bool isEditorOpened = true;

	ImGui::Begin( "Tool Bar", &isEditorOpened, ImVec2( (float)Screen::GetWidth(), 50.0f ), 0.9f, 
		ImGuiWindowFlags_NoResize | 
		ImGuiWindowFlags_NoCollapse | 
		ImGuiWindowFlags_NoMove | 
		ImGuiWindowFlags_NoTitleBar );

	ImGui::SetWindowPos( ImVec2( 0, 20.0f ) );
	ImGui::SetWindowSize( ImVec2( (float)Screen::GetWidth(), 50.0f ) );

	//ImGui::Dummy( ImVec2( 0, 0.5 ) );
	ImGui::Spacing();
	ImGui::SameLine();

	ImVec2 btnSize = ImVec2( 25, 25 );
	ImVec2 uv0 = ImVec2( 0, 0 );
	ImVec2 uv1 = ImVec2( 1, 1 );
	ImVec4 bgColor = editorGuiManager->GetImVec4( Color::Black );

	// Play Btn.;
	if ( ImGui::ImageButton( (void*)(intptr_t)privateTexturesMap.find( "Play.png" )->second->GetID(), btnSize, uv0, uv1, -1, bgColor ) )
		editorGuiManager->OnClickToolBarPlayBtn();
	if ( ImGui::IsItemHovered() )
		ImGui::SetTooltip( "Play" );

	ImGui::SameLine();
	ImGui::Dummy( ImVec2( 20.0f, 0.0f ) );
	ImGui::SameLine();
	if ( ImGui::ImageButton( (void*)(intptr_t)privateTexturesMap.find( "Move.png" )->second->GetID(), btnSize, uv0, uv1, -1, bgColor ) )
	{
	}
	if ( ImGui::IsItemHovered() )
		ImGui::SetTooltip( "Translate Tool" );

	ImGui::SameLine();
	if ( ImGui::ImageButton( (void*)(intptr_t)privateTexturesMap.find( "Rotate3D.png" )->second->GetID(), btnSize, uv0, uv1, -1, bgColor ) )
	{
	}
	if ( ImGui::IsItemHovered() )
		ImGui::SetTooltip( "Rotate Tool" );

	ImGui::SameLine();
	if ( ImGui::ImageButton( (void*)(intptr_t)privateTexturesMap.find( "Scale.png" )->second->GetID(), btnSize, uv0, uv1, -1, bgColor ) )
	{
	}
	if ( ImGui::IsItemHovered() )
		ImGui::SetTooltip( "Scale Tool" );

	ImGui::SameLine();
	ImGui::Spacing();
	ImGui::SameLine();
	if ( ImGui::ImageButton( (void*)(intptr_t)privateTexturesMap.find( "Local.png" )->second->GetID(), btnSize, uv0, uv1, -1, bgColor ) )
	{
	}
	if ( ImGui::IsItemHovered() )
		ImGui::SetTooltip( "Local Transformation Mode" );

	ImGui::SameLine();
	if ( ImGui::ImageButton( (void*)(intptr_t)privateTexturesMap.find( "Global.png" )->second->GetID(), btnSize, uv0, uv1, -1, bgColor ) )
	{
	}
	if ( ImGui::IsItemHovered() )
		ImGui::SetTooltip( "Global Transformation Mode" );

	//we are done working with this window
	ImGui::End();
}