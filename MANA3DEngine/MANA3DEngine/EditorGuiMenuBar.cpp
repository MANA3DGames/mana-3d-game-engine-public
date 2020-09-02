#include "EditorGuiMenuBar.h"
#include "EditorGuiManager.h"
#include "Screen.h"
#include "Application.h"
#include "SceneManager.h"
#include "MeshRenderer.h"
#include "MRectangle.h"
#include "MCube.h"
using namespace MANA3D;


EditorGuiMenuBar::EditorGuiMenuBar( EditorGuiManager* editorGuiManager  ) : EditorGuiWindow( editorGuiManager )
{
}

EditorGuiMenuBar::~EditorGuiMenuBar()
{
}




void EditorGuiMenuBar::Display()
{
	bool isEditorOpened = true;

	ImGui::Begin( "Menu Bar Window", &isEditorOpened, ImVec2( 1, 1 ), 0.9f, 
		ImGuiWindowFlags_NoResize | 
		ImGuiWindowFlags_NoCollapse | 
		ImGuiWindowFlags_NoMove | 
		ImGuiWindowFlags_NoBringToFrontOnFocus | 
		ImGuiWindowFlags_NoTitleBar | 
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoBackground );

	ImGui::SetWindowPos( ImVec2( 0, 0 ) );
	ImGui::SetWindowSize( ImVec2( (float)Screen::GetWidth(), (float)Screen::GetHeight() ) );

	if ( ImGui::BeginMenuBar() )
	{
		if ( ImGui::BeginMenu( "File" ) )
		{
			ImGui::Spacing();

			ImGui::TextColored( editorGuiManager->GetImVec4( Color::Green ), "Scene" );
			ImGui::Spacing();

			if ( ImGui::MenuItem( "Open Scene", "Ctrl+O" ) ) { /* Do stuff */ }
			if ( ImGui::MenuItem( "Save Scene", "Ctrl+S" ) ) { /* Do stuff */ }
			
			ImGui::Spacing();
			ImGui::Separator();

			ImGui::TextColored( editorGuiManager->GetImVec4( Color::Green ), "Project" );
			ImGui::Spacing();

			if ( ImGui::MenuItem( "Open Project", "" ) ) { /* Do stuff */ }
			if ( ImGui::MenuItem( "Save Project", "" ) ) { /* Do stuff */ }

			ImGui::Spacing();
			ImGui::Separator();

			if ( ImGui::MenuItem( "Exit", "Ctrl+F4" ) ) 
			{ 
				isEditorOpened = false; 
				Application::Quit();
			}

			ImGui::Spacing();

			ImGui::EndMenu();
		}

		if ( ImGui::BeginMenu( "Edit" ) )
		{
			ImGui::Spacing();

			if ( ImGui::MenuItem( "Undo", "Ctrl+Z" ) ) { /* Do stuff */ }
			if ( ImGui::MenuItem( "Redo", "Ctrl+Y" ) ) { /* Do stuff */ }

			ImGui::EndMenu();
		}

		if ( ImGui::BeginMenu( "Assets" ) )
		{
			ImGui::Spacing();

			ImGui::TextColored( editorGuiManager->GetImVec4( Color::Green ), "Import Asset" );
			ImGui::Spacing();

			if ( ImGui::MenuItem( "Import 3D Model", "" ) ) 
				editorGuiManager->StartOpenFileDialogAs( "Import Asset : 3D Model", "Import", OpenFileDialogAction::OFDA_Import3DModel );

			if ( ImGui::MenuItem( "Import Texture", "" ) )
				editorGuiManager->StartOpenFileDialogAs( "Import Asset : 2D Texture", "Import", OpenFileDialogAction::OFDA_Import2DTexture );

			if ( ImGui::MenuItem( "Import Cubemap", "" ) )
				editorGuiManager->StartOpenFileDialogAs( "Import Asset : Cubemap", "Import", OpenFileDialogAction::OFDA_ImportCubemap );

			ImGui::Spacing();
			ImGui::Separator();

			ImGui::TextColored( editorGuiManager->GetImVec4( Color::Green ), "Create New Asset" );
			ImGui::Spacing();

			if ( ImGui::MenuItem( "Create New Material", "" ) )
				editorGuiManager->CreateNewMaterial();

			ImGui::Spacing();

			ImGui::EndMenu();
		}

		if ( ImGui::BeginMenu( "GameObjects" ) )
		{
			ImGui::Spacing();

			if ( ImGui::MenuItem( "Create Empty GameObject", "" ) )
				SceneManager::AddGameObject( "Empty GameObject" );

			ImGui::Spacing();
			ImGui::Separator();

			ImGui::TextColored( editorGuiManager->GetImVec4( Color::Green ), "Primitives" );
			ImGui::Spacing();

			if ( ImGui::MenuItem( "Create Plane", "" ) )
			{
				auto prim = MRectangle::Create();
				editorGuiManager->OnClickItemInSceneHierarchy( prim->GetGameObject()->transform );
				editorGuiManager->OnCreateNewSceneObjectFromMenuBar();
			}

			if ( ImGui::MenuItem( "Create Cube", "" ) )
			{
				auto prim = MCube::Create();
				editorGuiManager->OnClickItemInSceneHierarchy( prim->GetGameObject()->transform );
				editorGuiManager->OnCreateNewSceneObjectFromMenuBar();
			}

			ImGui::Spacing();

			ImGui::Spacing();
			ImGui::Separator();

			ImGui::TextColored( editorGuiManager->GetImVec4( Color::Green ), "Lights" );
			ImGui::Spacing();

			if ( ImGui::MenuItem( "Create Point Light", "" ) ) 
			{
				SceneManager::CreatePointLightGameObject();
			}
			if ( ImGui::MenuItem( "Create Spot Light", "" ) ) 
			{ 
				SceneManager::CreateSpotLightGameObject();
			}
			if ( ImGui::MenuItem( "Create Directional Light", "" ) )
			{
				SceneManager::CreateDirectionalLightGameObject();
			}

			ImGui::Spacing();

			ImGui::Spacing();
			ImGui::Separator();

			ImGui::TextColored( editorGuiManager->GetImVec4( Color::Green ), "Camera" );
			ImGui::Spacing();
			if ( ImGui::MenuItem( "Create Camera", "" ) ) 
				SceneManager::AddCamera();

			ImGui::Spacing();

			ImGui::EndMenu();
		}

		if ( ImGui::BeginMenu( "Component" ) )
		{
			const GameObject* currentSelectedGameObjectInScene = editorGuiManager->GetCurrentSelectedGameObjectInScene();
			if ( currentSelectedGameObjectInScene == nullptr )
				editorGuiManager->StartDisableElement();

			ImGui::Spacing();
			ImGui::TextColored( editorGuiManager->GetImVec4( Color::Green ), "Renderer" );
			ImGui::Spacing();

			if ( ImGui::MenuItem( "Add Mesh Renderer", "" ) ) 
				editorGuiManager->AddMeshRendererToCurrentSelectedGameObjectInScene();

			if ( ImGui::MenuItem( "Add Skinned Mesh Renderer", "" ) ) { /* Do stuff */ }

			ImGui::Spacing();
			ImGui::Separator();

			ImGui::TextColored( editorGuiManager->GetImVec4( Color::Green ), "Controller" );
			ImGui::Spacing();

			if ( ImGui::MenuItem( "Add FPS Camera Controller", "" ) ) { /* Do stuff */ }

			ImGui::Spacing();
			ImGui::Separator();

			ImGui::TextColored( editorGuiManager->GetImVec4( Color::Green ), "Physics" );
			ImGui::Spacing();

			if ( ImGui::MenuItem( "Add Rigidbody", "" ) ) { /* Do stuff */ }
			if ( ImGui::MenuItem( "Add Box Collider", "" ) ) { /* Do stuff */ }
			if ( ImGui::MenuItem( "Add Sphere Collider", "" ) ) { /* Do stuff */ }
			if ( ImGui::MenuItem( "Add Mesh Collider", "" ) ) { /* Do stuff */ }

			if ( currentSelectedGameObjectInScene == nullptr )
				editorGuiManager->EndDisableElement();

			ImGui::Spacing();

			ImGui::EndMenu();
		}

		if ( ImGui::BeginMenu( "Help" ) )
		{
			ImGui::Spacing();

			if ( ImGui::MenuItem( "About", "" ) ) { /* Do stuff */ }

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	//we are done working with this window
	ImGui::End();
}