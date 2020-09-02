#include "EditorGuiSceneWindow.h"
#include "EditorGuiManager.h"
#include "Time.h"
#include "Debug.h"
#include "MString.h"
#include "SceneManager.h"
using namespace MANA3D;


EditorGuiSceneWindow::EditorGuiSceneWindow( EditorGuiManager* editorGuiManager )
	: EditorGuiWindow( editorGuiManager )
{
	isFocused = false;
	topOffsetY = 70.0f;
}

EditorGuiSceneWindow::~EditorGuiSceneWindow() {}



void EditorGuiSceneWindow::Display( const unsigned int& textureColorbuffer, const int& drawCallsCount )
{
	bool isSceneOpened = true;

	ImVec2 tempSize = windSize;

	ImGui::SetNextWindowSizeConstraints( ImVec2( 1024, 576 ), ImVec2( 3840, 2160 ) );//, CustomConstraints::Square ); 

	ImGui::Begin( "Scene Window", &isSceneOpened, ImVec2( 1024, 576 ), 1.0f, 
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar | 
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove );

	ImGui::SetWindowPos( ImVec2( 300, topOffsetY ) );
	windPos = ImGui::GetWindowPos();

	isFocused = ImGui::IsWindowFocused();

	if ( !isFocused &&
		 ImGui::IsMouseHoveringWindow() && 
		 ( ImGui::IsMouseClicked( 0 ) || ImGui::IsMouseClicked( 1 ) ) )
	{
		ImGui::SetWindowFocus();
	}

	windSize = ImGui::GetWindowSize();

	if ( windSize.y > windSize.x )
		windSize.x = windSize.y * 1.777777777777778f;
	else
		windSize.y = windSize.x / 1.777777777777778f;

	ImGui::SetWindowSize( windSize );

	UpdateWindProperties();

	MakeDragDropTargetForPrefabAssets( windSize );

	ImVec2 imgSize = windSize;
	imgSize.x -= 20.0f;
	imgSize.y -= 20.0f;
	ImGui::Image( (void*)(intptr_t)textureColorbuffer, imgSize, ImVec2( 0, 1 ), ImVec2( 1, 0 ) );
	
	bool openSt = true;
	DisplayStatistic( &openSt, drawCallsCount );


	ImGui::End();
}


void EditorGuiSceneWindow::DisplayStatistic( bool* p_open, const int& drawCallsCount )
{
    static int corner = 0;
    ImGuiIO& io = ImGui::GetIO();
    if ( corner != -1 )
    {
        ImVec2 window_pos_pivot = ImVec2( ( corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f );
        ImGui::SetNextWindowPos( ImVec2( windPos.x + 10, windPos.y + 10 ), ImGuiCond_Always, window_pos_pivot );
    }

    ImGui::SetNextWindowBgAlpha( 0.3f ); // Transparent background
    if ( ImGui::BeginChild( "Scene Statistics", ImVec2( 250, 100 ), true, 
							( corner != -1 ? ImGuiWindowFlags_NoMove : 0 ) | 
							ImGuiWindowFlags_NoTitleBar | 
							ImGuiWindowFlags_NoResize | 
							ImGuiWindowFlags_AlwaysAutoResize | 
							ImGuiWindowFlags_NoSavedSettings | 
							ImGuiWindowFlags_NoFocusOnAppearing | 
							ImGuiWindowFlags_NoNav  ) )
	{
        ImGui::Text( "Scene Statistics" );
        ImGui::Separator();

		ImGui::Text( "Draw Calls: %i", drawCallsCount );
		ImGui::Text( "%f ms/frame\n", Time::GetTimeNeededToDrawFrame() );
		ImGui::Text( "%i FPS\n", Time::GetFPS() );
    }
    ImGui::EndChild();
}



void EditorGuiSceneWindow::MakeDragDropTargetForPrefabAssets( const ImVec2& size )
{
	ImVec2 itemMin = ImGui::GetItemRectMin(); // Last Item which is seperator or spacing
	ImVec2 itemMax = ImGui::GetItemRectMax();
	ImVec2 rectMin = ImVec2( itemMin.x + 5, itemMin.y + 5 );
	ImVec2 rectMax = ImVec2( itemMin.x + size.x - 10, itemMax.y + size.y - ( itemMax.y - topOffsetY + 5 ) );
	ImRect rect = ImRect( rectMin, rectMax );
	if ( ImGui::BeginDragDropTargetCustom( rect, ImGuiID( "ScenePrefabDragDrop" ) ) )
	{
		editorGuiManager->OnDropPrefabOnScene();
		ImGui::EndDragDropTarget();
	}
}
