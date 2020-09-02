// We only need the following headers just to get the special directoreis for FileOpenDialog function.
#include <windows.h>
#include <shlobj.h>
//#include <ImGUI/imgui_internal.h>

#include "EditorGuiManager.h"
#include "EditorGuiMenuBar.h"
#include "EditorGuiToolBar.h"
#include "EditorGuiSceneHierarchyWindow.h"
#include "EditorGuiSceneWindow.h"
#include "EditorGuiUtilitesWindow.h"
#include "EditorGuiInspectorWindow.h"
#include "EditorGuiAssetsOverviewWindow.h"
#include "SceneManager.h"
#include "Application.h"
#include "SkinnedMeshRenderer.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Screen.h"
#include "Camera.h"
#include "Time.h"
#include "Input.h"
#include "Model3DImporter.h"
#include "Texture2DImporter.h"
#include "GraphicSettings.h"
#include "CubemapImporter.h"
#include "MRectangle.h"
#include "CommandManager.h"
using namespace MANA3D;
using namespace glm;

using std::list;
using std::stack;
using std::map;
using std::string;


static int selection_mask = (0 << 2);
static const char* current_GoTag = nullptr;

bool didReleaseZKey = true;
bool didReleaseYKey = true;





EditorGuiManager::EditorGuiManager() :
	isEditorOpened( true ),
	isSceneOpened( true ),
	isUtilitesOpened( true ),
	sceneHierarchyWindowSize( ImVec2( 0, 0 ) ),
	utilitesWindowSize( ImVec2( 0, 0 ) )
{
	gOpenFileDialogData.driveLetter = "C:\\";
	GetSpecialDirectories();

	guiMenuBar = new EditorGuiMenuBar( this );
	guiToolBar = new EditorGuiToolBar( this );
	guiSceneHierarchyWindow = new EditorGuiSceneHierarchyWindow( this );
	guiSceneWindow = new EditorGuiSceneWindow( this );
	guiUtilitesWindow = new EditorGuiUtilitesWindow( this );
	guiInspectorWindow = new EditorGuiInspectorWindow( this );
	guiAssetsOverviewWindow = new EditorGuiAssetsOverviewWindow( this );
}

EditorGuiManager::~EditorGuiManager()
{
	if ( guiMenuBar != nullptr )
		delete guiMenuBar;
}



void EditorGuiManager::Display( const unsigned int& textureColorbuffer, const int& drawCallsCount, map<string, MTexture*>& privateTexturesMap )
{
	/*ImGui::ShowDemoWindow();
	return;*/

	if ( isOpenFileDialogOpend )
		DisplayOpenFileDialog();
	
	if ( !isEditorOpened )
		return;

	guiMenuBar->Display();
	guiToolBar->Display( privateTexturesMap );
	guiSceneHierarchyWindow->Display();
	guiSceneWindow->Display( textureColorbuffer, drawCallsCount );
	guiUtilitesWindow->Display( privateTexturesMap );
	guiInspectorWindow->Display( privateTexturesMap );
	guiAssetsOverviewWindow->Display();




	if ( Input::GetKeyPressed( KeyCode::KC_LeftControl ) && Input::GetKeyPressed( KeyCode::KC_Z ) && didReleaseZKey )
	{
		CommandManager::UndoCommand();
		didReleaseZKey = false;
	}
	if ( Input::GetKeyReleased( KeyCode::KC_Z ) && !didReleaseZKey )
		didReleaseZKey = true;

	if ( Input::GetKeyPressed( KeyCode::KC_LeftControl ) && Input::GetKeyPressed( KeyCode::KC_Y ) && didReleaseYKey )
	{
		CommandManager::RedoCommand();
		didReleaseYKey = false;
	}
	if ( Input::GetKeyReleased( KeyCode::KC_Y ) && !didReleaseYKey )
		didReleaseYKey = true;
}






MString EditorGuiManager::AddAssetElementTitle( const MString& str )
{
	MString titleText = str;
	if ( titleText.GetLength() > 18 )
	{
		titleText = titleText.GetSubStr( str, 0, 15 );
		titleText.Append( "..." );
	}
				
	ImVec2 txtSize = ImGui::CalcTextSize( titleText.GetCStr() );
	ImGui::Dummy( ImVec2( 57.0f - ( txtSize.x * 0.5f ), 0.0f ) );
	ImGui::SameLine();
	ImGui::Text( titleText.GetCStr() );

	return titleText;
}

void EditorGuiManager::SetTempTargetWrapMode( const MString& str )
{
	gtempTargetWrapMode = str;
}
MString EditorGuiManager::GetTempTargetWrapMode()
{
	return gtempTargetWrapMode;
}

void EditorGuiManager::NulloutCurrentSelectedAssets()
{
	currentSelectedPrefabInAsset = nullptr;
	currentSelectedTextureInAsset = nullptr;
	currentSelectedCubemapInAsset = nullptr;
	currentSelectedMeshInAsset = nullptr;
	currentSelectedMaterialInAsset = nullptr;
}



void EditorGuiManager::DisplayMaterialComponent( MMaterial* mat )
{
	// Shader
	MString label = "Shader";
	ImGui::Spacing();
	ImGui::Dummy( ImVec2( 10.0f, 0.0f ) );
	ImGui::SameLine();
	ImGui::Text( label );
	ImGui::SameLine();
	ImGui::Dummy( ImVec2( 45.0f, 0.0f ) );
	ImGui::SameLine();

	ImGui::PushItemWidth( 200 );

	MString id( "##hidelabel_%sCombo", label.GetCStr() );

	if ( ImGui::BeginCombo( id, mat->GetShaderName() ) )
	{
		list<string> choicesList = MShader::GetAllShaderNamesList();

		for ( list<string>::iterator it = choicesList.begin(); it != choicesList.end(); ++it )
		{
			MString str = (*it);
			bool is_selected = ( mat->GetShaderName().GetCStr() == str );
			if ( ImGui::Selectable( str, is_selected ) )
				mat->SetShader( MShader::Get( str ) );
			if ( is_selected )
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGui::PopItemWidth();


	ImGui::Dummy( ImVec2( 0, 10 ) );

	// Textures
	map<string, MTextureIndexPair>& textureIndexPairs = mat->GetTextureIndexPairsMap();
	for ( map<string, MTextureIndexPair>::iterator it = textureIndexPairs.begin(); it != textureIndexPairs.end(); ++it )
	{
		MString label( it->first );
		MTexture* tex = it->second.tex;

		ImGui::Spacing();
		ImGui::Dummy( ImVec2( 10, 0 ) );
		ImGui::SameLine();
		ImGui::Text( label.GetCStr() );
		ImGui::SameLine();
		ImGui::Dummy( ImVec2( CalcTextOffset( label.GetCStr(), 150 ), 0 ) );
		ImGui::SameLine();

		ImTextureID texID = nullptr;
		if ( tex != nullptr )
			texID = (void*)(intptr_t)tex->GetID();

		if ( ImGui::ImageButton( texID, ImVec2( 100.0f, 100.0f ), ImVec2( 0, 0 ), ImVec2( 1, 1 ), -1, GetImVec4( Color::Black ) ) )
		{
			if ( texID != nullptr )
				textureToBeSelectedFromAsset = tex;
		}

		ImRect rect = ImRect( ImGui::GetItemRectMin(), ImGui::GetItemRectMax() );
		MakeDragDropTargetTexture( rect, it->second.indexInShader, it->first, mat );

		if ( texID != nullptr )
		{
			ImGui::Dummy( ImVec2( 115, 0 ) );
			ImGui::SameLine();
			ImGui::TextColored( ImVec4( 0, 1, 0 , 1 ), tex->GetName() );
		}
	}



	// Cubemap Textures
	map<string, MCubemapIndexPair>& cubemapIndexPairs = mat->GetCubemapIndexPairsMap();
	for ( map<string, MCubemapIndexPair>::iterator it = cubemapIndexPairs.begin(); it != cubemapIndexPairs.end(); ++it )
	{
		MString label( it->first );
		MCubemap* cubemap = it->second.cubeMap;

		ImGui::Spacing();
		ImGui::Dummy( ImVec2( 10, 0 ) );
		ImGui::SameLine();
		ImGui::Text( label.GetCStr() );
		ImGui::SameLine();
		ImGui::Dummy( ImVec2( CalcTextOffset( label.GetCStr(), 150 ), 0 ) );
		ImGui::SameLine();

		ImTextureID texID = nullptr;
		if ( cubemap != nullptr )
			texID = (void*)(intptr_t)cubemap->GetThumbTex()->GetID();

		if ( ImGui::ImageButton( texID, ImVec2( 100.0f, 100.0f ), ImVec2( 0, 0 ), ImVec2( 1, 1 ), -1, GetImVec4( Color::Black ) ) )
		{
			if ( texID != nullptr )
				cubemapToBeSelectedFromAsset = cubemap;
		}

		ImRect rect = ImRect( ImGui::GetItemRectMin(), ImGui::GetItemRectMax() );
		MakeDragDropTargetTexture( rect, it->second.indexInShader, "", mat );

		if ( texID != nullptr )
		{
			ImGui::Dummy( ImVec2( 115, 0 ) );
			ImGui::SameLine();
			ImGui::TextColored( ImVec4( 0, 1, 0 , 1 ), cubemap->GetName() );
		}
	}
}

void EditorGuiManager::DisplayOpenFileDialog()
{
	ImGui::Begin( gOpenFileDialogData.actionData.dialogTitle, &isOpenFileDialogOpend, 
		ImGuiWindowFlags_NoCollapse | 
		ImGuiWindowFlags_NoScrollbar | 
		ImGuiWindowFlags_NoResize | 
		ImGuiWindowFlags_NoMove );

	ImVec2 size = ImVec2( (float)Screen::GetWidth(), (float)Screen::GetHeight() );
	ImGui::SetWindowSize( size );
	ImGui::SetWindowPos( ImVec2( 0, 0 ) );

	DrawComboDrop( ImVec2( 0.0f, 0.0f ), ImVec2( 0.0f, 0.0f ), 50.0f,
					"", gOpenFileDialogData.driveLetter, Application::GetDrives(), gOpenFileDialogData.driveLetter,
					this, &EditorGuiManager::BrowseDrive );

	for ( size_t i = 0; i < gOpenFileDialogData.splitPathData.size(); i++ )
	{
		ImGui::SameLine();

		MString label;
		if ( gOpenFileDialogData.splitPathData[i].name.GetLength() > 30 )
		{
			MString subStr = MString::GetSubStr( gOpenFileDialogData.splitPathData[i].name, 0, 30 );
			label = MString( "%s...", subStr.GetCStr() );
		}
		else
			label = gOpenFileDialogData.splitPathData[i].name.GetCStr();

		if ( ImGui::Button( label ) )
		{
			gOpenFileDialogData.previousPathStack.push( gOpenFileDialogData.currentPath );
			gOpenFileDialogData.currentPath = gOpenFileDialogData.splitPathData[i].path;

			gOpenFileDialogData.splitPathData.erase( gOpenFileDialogData.splitPathData.begin() + i + 1, gOpenFileDialogData.splitPathData.begin() + gOpenFileDialogData.splitPathData.size() );
			gOpenFileDialogData.currentSelectedFolderIndex = -1;
			gOpenFileDialogData.currentSelectedFileIndex = -1;
			break;
		}
	}

	ImVec2 childSize = ImVec2( size.x * 0.85f, size.y * 0.9f );

	ImGui::Spacing();

	ImGui::Dummy( ImVec2( 10.0f, 10.0f ) );
	ImGui::SameLine();

	ImGui::BeginChild( "SpecialDirectoryBrowser", ImVec2( size.x * 0.1f, childSize.y ), true );

	ImVec2 btnSize = ImVec2( size.x * 0.09f, 20.0f );

	for ( size_t i = 0; i < gOpenFileDialogData.specialDirectories.size(); i++ )
	{
		if ( ImGui::Button( gOpenFileDialogData.specialDirectories[i].name, btnSize ) )
		{
			gOpenFileDialogData.currentPath = gOpenFileDialogData.specialDirectories[i].path;
			CreateNewSplitPath( gOpenFileDialogData.currentPath );
			gOpenFileDialogData.currentSelectedFolderIndex = -1;
			gOpenFileDialogData.currentSelectedFileIndex = -1;
		}

		if ( i == 0 )
		{
			ImGui::Separator();
			ImGui::Dummy( ImVec2( 0.0f, 10.0f ) );
		}

		ImGui::Dummy( ImVec2( 0.0f, 1.0f ) );
	}

	ImGui::Dummy( ImVec2( 0.0f, childSize.y - ( ( gOpenFileDialogData.specialDirectories.size() + 3 ) * ( btnSize.y + 10 ) ) ) );

	ImGui::Separator();

	// Check if a folder is selected. 
	bool disableActionBtn = gOpenFileDialogData.currentSelectedFolderIndex != -1 || gOpenFileDialogData.currentSelectedFileIndex == -1;
	if ( disableActionBtn )
		StartDisableElement();
	if ( ImGui::Button( gOpenFileDialogData.actionData.btnLabel, btnSize ) )
	{
		OnDoubleClickFileInOpenFileDialog( gOpenFileDialogData.currentSelectedFilePathData );
	}
	if ( ImGui::Button( "Import All", btnSize ) )
	{
		OnDoubleClickFileInOpenFileDialog( gOpenFileDialogData.currentSelectedFilePathData, true );
	}
	if ( disableActionBtn )
		EndDisableElement();

	if ( ImGui::Button( "Cancel", btnSize ) )
	{
		isOpenFileDialogOpend = false;
		gOpenFileDialogData.currentSelectedFolderIndex = -1;
		gOpenFileDialogData.currentSelectedFileIndex = -1;
	}

	ImGui::EndChild();

	
	ImGui::SameLine();

	// GetAllFiles is a very heavy process (at least for now) so we don't want to call it each frame so we need to call it only when path is changed
	// and in order to do that we need to check if the current path was changed.
	if ( gOpenFileDialogData.currentPath != gOpenFileDialogData.prevPath )
	{
		gOpenFileDialogData.currentAlldirectories = Application::GetAllFiles( gOpenFileDialogData.currentPath, MFileFilter::MFF_Directory );
		gOpenFileDialogData.currentAllfiles = Application::GetAllFiles( gOpenFileDialogData.currentPath, MFileFilter::MFF_File );

		gOpenFileDialogData.prevPath = gOpenFileDialogData.currentPath;
	}


	ImGui::BeginChild( "DirectoryAndFileBrowser", childSize, true );

	ImGui::PushStyleColor( ImGuiCol_Button, GetImVec4( Color::Red ) );
	if ( ImGui::ArrowButton( "BackBtn", ImGuiDir_Left ) )
	{
		if ( !gOpenFileDialogData.previousPathStack.empty() )
		{
			gOpenFileDialogData.currentPath = gOpenFileDialogData.previousPathStack.top();
			gOpenFileDialogData.previousPathStack.pop();
			CreateNewSplitPath( gOpenFileDialogData.currentPath );
			gOpenFileDialogData.currentSelectedFolderIndex = -1;
			gOpenFileDialogData.currentSelectedFileIndex = -1;
		}
	}
	ImGui::PopStyleColor( 1 );
	ImGui::SameLine();
	
	ImGui::TextColored( GetImVec4( Color::Red ), gOpenFileDialogData.currentPath );
	ImGui::Separator();
	ImGui::Dummy( ImVec2( 0.0f, 10.0f ) );

	ImGui::PushStyleColor( ImGuiCol_Text, GetImVec4( Color::Yellow ) );
	for ( size_t i = 0; i < gOpenFileDialogData.currentAlldirectories.size(); i++ )
	{
		bool is_selected = ( gOpenFileDialogData.currentSelectedFolderIndex == i );
        if ( ImGui::Selectable( gOpenFileDialogData.currentAlldirectories[i].name, is_selected ) )
		{
			gOpenFileDialogData.currentSelectedFolderIndex = i;
			gOpenFileDialogData.currentSelectedFileIndex = -1;
		}
		if ( is_selected )
			ImGui::SetItemDefaultFocus();

        if ( ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked( 0 ) )
        {
			gOpenFileDialogData.previousPathStack.push( gOpenFileDialogData.currentPath );
			gOpenFileDialogData.currentPath = MString( "%s%s\\", gOpenFileDialogData.currentAlldirectories[i].path.GetCStr(), gOpenFileDialogData.currentAlldirectories[i].name.GetCStr() );

			AddFolderPathToSplitPath( gOpenFileDialogData.currentAlldirectories[i].name, gOpenFileDialogData.currentPath );

			gOpenFileDialogData.currentSelectedFolderIndex = -1;
			gOpenFileDialogData.currentSelectedFileIndex = -1;
        }
	}
	ImGui::PopStyleColor( 1 );

	for ( size_t i = 0; i < gOpenFileDialogData.currentAllfiles.size(); i++ )
	{
		bool is_selected = ( gOpenFileDialogData.currentSelectedFileIndex == i );
		if ( ImGui::Selectable( gOpenFileDialogData.currentAllfiles[i].name, is_selected ) )
		{
			gOpenFileDialogData.currentSelectedFileIndex = i;
			gOpenFileDialogData.currentSelectedFolderIndex = -1;
		}
		if ( is_selected )
		{
			gOpenFileDialogData.currentSelectedFilePathData = gOpenFileDialogData.currentAllfiles[i];
			ImGui::SetItemDefaultFocus();
		}

		if ( ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked( 0 ) )
		{
			if ( gOpenFileDialogData.actionData.action == OpenFileDialogAction::OFDA_ImportCubemap )
				OnDoubleClickFileInOpenFileDialog( gOpenFileDialogData.splitPathData[gOpenFileDialogData.splitPathData.size()-1] );
			else
				OnDoubleClickFileInOpenFileDialog( gOpenFileDialogData.currentAllfiles[i] );
		}
	}

	ImGui::EndChild();

	ImGui::End();
}



template <typename T>
void EditorGuiManager::DrawComboDrop(
	const ImVec2& leftMargin, const ImVec2& comboOffset, 
	const float& width,
	const MString& label, const MString& startupSelected, list<string>& choicesList, const MString& selected, 
	T callbackInstance, void (*callbackFunc)( T, MString str ) )
{
	ImGui::Spacing();
	ImGui::Dummy( leftMargin );
	ImGui::SameLine();
	ImGui::Text( label );
	ImGui::SameLine();
	ImGui::Dummy( comboOffset );
	ImGui::SameLine();

	ImGui::PushItemWidth( width );

	MString id( "##hidelabel_%sCombo", label.GetCStr() );

	if ( ImGui::BeginCombo( id, startupSelected ) )
	{
		for ( list<string>::iterator it = choicesList.begin(); it != choicesList.end(); ++it )
		{
			MString str = (*it);
			bool is_selected = ( selected.GetCStr() == str );
			if ( ImGui::Selectable( str, is_selected ) )
				callbackFunc( callbackInstance, str );
			if ( is_selected )
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGui::PopItemWidth();
}

template void EditorGuiManager::DrawComboDrop(
	const ImVec2& leftMargin, const ImVec2& comboOffset, 
	const float& width,
	const MString& label, const MString& startupSelected, list<string>& choicesList, const MString& selected, 
	GameObject* callbackInstance, void (*callbackFunc)( GameObject*, MString str ) );

template void EditorGuiManager::DrawComboDrop( 
	const ImVec2& leftMargin, const ImVec2& comboOffset, 
	const float& width,
	const MString& label, const MString& startupSelected, list<string>& choicesList, const MString& selected, 
	MeshRenderer* callbackInstance, void (*callbackFunc)( MeshRenderer*, MString str ) );





void EditorGuiManager::DrawComboDrop( const float& leftMargin, const float& offset, const MString& label, const MString& startupVal, list<string>& choicesList, void ( *callbackFunc )( MString ) )
{
	ImGui::Spacing();
	ImGui::Dummy( ImVec2( leftMargin, 0 ) );
	ImGui::SameLine();
	ImGui::Text( label );
	ImGui::SameLine();
	ImGui::Dummy( ImVec2( CalcTextOffset( label.GetCStr(), offset ), 0 ) );
	ImGui::SameLine();

	ImGui::PushItemWidth( 200.0f );

	MString id( "##hidelabel_%sCombo", label.GetCStr() );
	if ( ImGui::BeginCombo( id, startupVal.GetCStr() ) )
	{
		for ( list<string>::iterator it = choicesList.begin(); it != choicesList.end(); ++it )
		{
			MString str = (*it);
			bool is_selected = ( startupVal.GetCStr() == str );
			if ( ImGui::Selectable( str, is_selected ) )
				callbackFunc( str );
			if ( is_selected )
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGui::PopItemWidth();
}

void EditorGuiManager::DrawCheckBox( const float& leftMargin, const float& offset, const MString& label, const bool& startupVal, void ( *callbackFunc )( bool ) )
{
	ImGui::Spacing();
	ImGui::Dummy( ImVec2( leftMargin, 0 ) );
	ImGui::SameLine();
	ImGui::Text( label.GetCStr() );
	ImGui::SameLine();
	ImGui::Dummy( ImVec2( CalcTextOffset( label.GetCStr(), offset ), 0 ) );
	ImGui::SameLine();
	bool isEnabled = startupVal;
	MString id( "##hidelabel_%sCheckBox", label.GetCStr() );
	ImGui::Checkbox( id, &isEnabled );
	callbackFunc( isEnabled );
}

void EditorGuiManager::AddLabelWithOffset( const MString& label, const float& leftMargin, const float& targetOffset )
{
	const char* str = label.GetCStr();
	ImGui::Spacing();
	ImGui::Dummy( ImVec2( leftMargin, 0.0f ) );
	ImGui::SameLine();
	ImGui::Text( str );
	ImGui::SameLine();
	ImGui::Dummy( ImVec2( CalcTextOffset( str, targetOffset ), 0.0f ) );
	ImGui::SameLine();
}



bool EditorGuiManager::GetIsSceneWindowFocused()
{
	return guiSceneWindow->GetIsFocused();
}
float EditorGuiManager::GetSceneWindowTopOffsetY()
{
	return guiSceneWindow->GetTopOffsetY();
}
ImVec2 EditorGuiManager::GetSceneWindowSize()
{
	return guiSceneWindow->GetWindSize();
}

ImVec2 EditorGuiManager::GetSceneHierarchyWindowSize()
{
	return guiSceneHierarchyWindow->GetWindSize();
}

ImVec2 EditorGuiManager::GetUtilitiesWindowSize()
{
	return guiUtilitesWindow->GetWindSize();
}


void EditorGuiManager::MakeDragDropSourceTexture( const MString& ddID, const MString& label, const DragDropTexture& data )
{
	if ( ImGui::BeginDragDropSource( ImGuiDragDropFlags_None ) )
	{
		// Set payload to carry the index of our item (could be anything).
		ImGui::SetDragDropPayload( ddID, &data, sizeof(DragDropTexture) );

		// Display preview (could be anything, e.g. when dragging an image we could decide to display the filename and a small preview of the image, etc.).
		ImGui::Text( label );

		ImGui::EndDragDropSource();
	}
}
void EditorGuiManager::MakeDragDropTargetTexture( const ImRect& rect, const int& index, const MString& shaderTexName, MMaterial* mat )
{
	if ( ImGui::BeginDragDropTargetCustom( rect, ImGuiID( "TextureAssetDragDrop" ) ) )
	{
		OnDropTextureOnSlot( rect, index, shaderTexName, mat  );
		ImGui::EndDragDropTarget();
	}
}
void EditorGuiManager::OnDropTextureOnSlot( const ImRect& rect, const int& index, const MString& shaderTexName, MMaterial* mat  )
{
	if ( const ImGuiPayload * payload = ImGui::AcceptDragDropPayload( "TextureAssetDragDrop" ) )
	{
		IM_ASSERT( payload->DataSize == sizeof(DragDropTexture) );
		DragDropTexture payload_n = *(const DragDropTexture*)payload->Data;
		if ( payload_n.texture != nullptr )
			mat->SetTexture( shaderTexName, payload_n.texture );
		else
			Debug::LogWarning( MString( "Failed to find texture: %s\t\n", payload_n.name.GetCStr() ) );
	}
}

void EditorGuiManager::MakeDragDropSourceMaterial( const MString& ddID, const MString& label, const DragDropMaterial& data )
{
	if ( ImGui::BeginDragDropSource( ImGuiDragDropFlags_None ) )
	{
		// Set payload to carry the index of our item (could be anything).
		ImGui::SetDragDropPayload( ddID, &data, sizeof(DragDropMaterial) );

		// Display preview.
		ImGui::Text( label );

		ImGui::EndDragDropSource();
	}
}
void EditorGuiManager::MakeDragDropTargetMaterial( const ImRect& rect, MeshRenderer* renderer )
{
	if ( ImGui::BeginDragDropTargetCustom( rect, ImGuiID( "MaterialAssetDragDrop" ) ) )
	{
		OnDropMaterialOnSlot( rect, renderer );
		ImGui::EndDragDropTarget();
	}
}
void EditorGuiManager::OnDropMaterialOnSlot( const ImRect& rect, MeshRenderer* renderer )
{
	if ( const ImGuiPayload * payload = ImGui::AcceptDragDropPayload( "MaterialAssetDragDrop" ) )
	{
		IM_ASSERT( payload->DataSize == sizeof(DragDropMaterial) );
		DragDropMaterial payload_n = *(const DragDropMaterial*)payload->Data;
		if ( payload_n.material != nullptr )
			renderer->SetMaterial( payload_n.material );
		else
			Debug::LogWarning( MString( "Failed to find material: %s\t\n", payload_n.name.GetCStr() ) );
	}
}

void EditorGuiManager::MakeDragDropSourceMesh( const MString& ddID, const MString& label, const DragDropMesh& data )
{
	if ( ImGui::BeginDragDropSource( ImGuiDragDropFlags_None ) )
	{
		// Set payload to carry the index of our item (could be anything).
		ImGui::SetDragDropPayload( ddID, &data, sizeof(DragDropMesh) );

		// Display preview.
		ImGui::Text( label );

		ImGui::EndDragDropSource();
	}
}
void EditorGuiManager::MakeDragDropTargetMesh( const ImRect& rect, MeshRenderer* renderer )
{
	if ( ImGui::BeginDragDropTargetCustom( rect, ImGuiID( "MeshAssetDragDrop" ) ) )
	{
		OnDropMeshOnSlot( rect, renderer );
		ImGui::EndDragDropTarget();
	}
}
void EditorGuiManager::OnDropMeshOnSlot( const ImRect& rect, MeshRenderer* renderer )
{
	if ( const ImGuiPayload * payload = ImGui::AcceptDragDropPayload( "MeshAssetDragDrop" ) )
	{
		IM_ASSERT( payload->DataSize == sizeof(DragDropMesh) );
		DragDropMesh payload_n = *(const DragDropMesh*)payload->Data;
		if ( payload_n.mesh != nullptr )
			renderer->SetMesh( payload_n.mesh );
		else
			Debug::LogWarning( MString( "Failed to find mesh: %s\t\n", payload_n.name.GetCStr() ) );
	}
}

void EditorGuiManager::OnDropPrefabOnScene()
{
	if ( const ImGuiPayload * payload = ImGui::AcceptDragDropPayload( "PrefabDragDrop" ) )
	{
		IM_ASSERT( payload->DataSize == sizeof(DragDropGameObject) );
		DragDropGameObject payload_n = *(const DragDropGameObject*)payload->Data;
		SceneManager::AddGameObject( payload_n.name, payload_n.go );
	}
}


void EditorGuiManager::StartOpenFileDialogAs( const MString& title, const MString& btnLabel, const OpenFileDialogAction& action )
{
	if ( gOpenFileDialogData.currentPath == "" )
	{
		gOpenFileDialogData.previousPathStack.push( gOpenFileDialogData.driveLetter );
		gOpenFileDialogData.currentPath = gOpenFileDialogData.driveLetter;
	}

	gOpenFileDialogData.actionData.dialogTitle = title;
	gOpenFileDialogData.actionData.btnLabel = btnLabel;
	gOpenFileDialogData.actionData.action = action;

	isOpenFileDialogOpend = true;
}

void EditorGuiManager::AddFolderPathToSplitPath( const MString& name, const MString& path )
{
	AddFolderPathData( gOpenFileDialogData.splitPathData, name, path );
}

void EditorGuiManager::AddFolderPathData( vector<MFile> &container, const MString& name, const MString& path )
{
	MFile file;
	file.name = name;
	file.path = path;
	file.type = 1;
	container.push_back( file );
}

void EditorGuiManager::CreateNewSplitPath( const MString& newPath )
{
	gOpenFileDialogData.splitPathData.clear();

	vector<MString> pathArray = MString::SplitMString( newPath, '\\' );

	MString path;
	for ( size_t i = 0; i < pathArray.size(); i++ )
	{
		path.Append( MString( "%s\\", pathArray[i].GetCStr() ) );
		AddFolderPathToSplitPath( pathArray[i], path );
	}
}

void EditorGuiManager::ClearPreviousPathStack()
{
	while ( !gOpenFileDialogData.previousPathStack.empty() )
		gOpenFileDialogData.previousPathStack.pop();
}

void EditorGuiManager::GetSpecialDirectories()
{
	// Project Folder.
	AddFolderPathData( gOpenFileDialogData.specialDirectories, "Project Folder", Application::GetAppDirctory() );

	TCHAR folder[MAX_PATH];

	// Desktop Folder.
	if ( SUCCEEDED( SHGetFolderPath( nullptr, CSIDL_DESKTOP | CSIDL_FLAG_CREATE, nullptr,  0, folder ) ) )
		AddFolderPathData( gOpenFileDialogData.specialDirectories, "Desktop", MString( "%s\\", folder ) );

	// My Computer Folder.
	if ( SUCCEEDED( SHGetFolderPath( nullptr, CSIDL_DRIVES | CSIDL_FLAG_CREATE, nullptr,  0, folder ) ) )
		AddFolderPathData( gOpenFileDialogData.specialDirectories, "My Computer", MString( "%s\\", folder ) );

	// My Program Files Folder.
	if ( SUCCEEDED( SHGetFolderPath( nullptr, CSIDL_PROGRAM_FILES | CSIDL_FLAG_CREATE, nullptr,  0, folder ) ) )
		AddFolderPathData( gOpenFileDialogData.specialDirectories, "Program Files", MString( "%s\\", folder ) );
	
	// My Program Files Common Folder.
	if ( SUCCEEDED( SHGetFolderPath( nullptr, CSIDL_PROGRAM_FILES_COMMON | CSIDL_FLAG_CREATE, nullptr,  0, folder ) ) )
		AddFolderPathData( gOpenFileDialogData.specialDirectories, "Program Files Common", MString( "%s\\", folder ) );

	// My Documents Folder.
	if ( SUCCEEDED( SHGetFolderPath( nullptr, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, nullptr,  0, folder ) ) )
		AddFolderPathData( gOpenFileDialogData.specialDirectories, "Documents", MString( "%s\\", folder ) );

	// My Favorites Folder.
	if ( SUCCEEDED( SHGetFolderPath( nullptr, CSIDL_FAVORITES | CSIDL_FLAG_CREATE, nullptr,  0, folder ) ) )
		AddFolderPathData( gOpenFileDialogData.specialDirectories, "Favorites", MString( "%s\\", folder ) );

	// My Pictures Folder.
	if ( SUCCEEDED( SHGetFolderPath( nullptr, CSIDL_MYPICTURES | CSIDL_FLAG_CREATE, nullptr,  0, folder ) ) )
		AddFolderPathData( gOpenFileDialogData.specialDirectories, "My Pictures", MString( "%s\\", folder ) );

	// My Music Folder.
	if ( SUCCEEDED( SHGetFolderPath( nullptr, CSIDL_MYMUSIC | CSIDL_FLAG_CREATE, nullptr,  0, folder ) ) )
		AddFolderPathData( gOpenFileDialogData.specialDirectories, "My Music", MString( "%s\\", folder ) );

	// My Videos Folder.
	if ( SUCCEEDED( SHGetFolderPath( nullptr, CSIDL_MYVIDEO | CSIDL_FLAG_CREATE, nullptr,  0, folder ) ) )
		AddFolderPathData( gOpenFileDialogData.specialDirectories, "My Videos", MString( "%s\\", folder ) );

	// Admin Tools.
	if ( SUCCEEDED( SHGetFolderPath( nullptr, CSIDL_ADMINTOOLS | CSIDL_FLAG_CREATE, nullptr,  0, folder ) ) )
		AddFolderPathData( gOpenFileDialogData.specialDirectories, "Admin Tools", MString( "%s\\", folder ) );

	// Common Admin Tools.
	if ( SUCCEEDED( SHGetFolderPath( nullptr, CSIDL_COMMON_ADMINTOOLS | CSIDL_FLAG_CREATE, nullptr,  0, folder ) ) )
		AddFolderPathData( gOpenFileDialogData.specialDirectories, "Common Admin Tools", MString( "%s\\", folder ) );

	// AppData.
	if ( SUCCEEDED( SHGetFolderPath( nullptr, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL,  0, folder ) ) )
		AddFolderPathData( gOpenFileDialogData.specialDirectories, "AppData", MString( "%s\\", folder ) );

	// Common AppData.
	if ( SUCCEEDED( SHGetFolderPath( nullptr, CSIDL_COMMON_APPDATA | CSIDL_FLAG_CREATE, nullptr,  0, folder ) ) )
		AddFolderPathData( gOpenFileDialogData.specialDirectories, "Common AppData", MString( "%s\\", folder ) );

	// Common Documents.
	if ( SUCCEEDED( SHGetFolderPath( nullptr, CSIDL_COMMON_DOCUMENTS | CSIDL_FLAG_CREATE, nullptr,  0, folder ) ) )
		AddFolderPathData( gOpenFileDialogData.specialDirectories, "Common Documents", MString( "%s\\", folder ) );

	// Cookies.
	if ( SUCCEEDED( SHGetFolderPath( nullptr, CSIDL_COOKIES | CSIDL_FLAG_CREATE, nullptr,  0, folder ) ) )
		AddFolderPathData( gOpenFileDialogData.specialDirectories, "Cookies", MString( "%s\\", folder ) );

	// History.
	if ( SUCCEEDED( SHGetFolderPath( nullptr, CSIDL_HISTORY | CSIDL_FLAG_CREATE, nullptr,  0, folder ) ) )
		AddFolderPathData( gOpenFileDialogData.specialDirectories, "History", MString( "%s\\", folder ) );

	// Internet Cache.
	if ( SUCCEEDED( SHGetFolderPath( nullptr, CSIDL_INTERNET_CACHE | CSIDL_FLAG_CREATE, nullptr,  0, folder ) ) )
		AddFolderPathData( gOpenFileDialogData.specialDirectories, "Internet Cache", MString( "%s\\", folder ) );

	// Local AppData.
	if ( SUCCEEDED( SHGetFolderPath( nullptr, CSIDL_LOCAL_APPDATA | CSIDL_FLAG_CREATE, nullptr,  0, folder ) ) )
		AddFolderPathData( gOpenFileDialogData.specialDirectories, "Local AppData", MString( "%s\\", folder ) );

	// System.
	if ( SUCCEEDED( SHGetFolderPath( nullptr, CSIDL_SYSTEM | CSIDL_FLAG_CREATE, nullptr,  0, folder ) ) )
		AddFolderPathData( gOpenFileDialogData.specialDirectories, "System", MString( "%s\\", folder ) );

	// Windows.
	if ( SUCCEEDED( SHGetFolderPath( nullptr, CSIDL_WINDOWS | CSIDL_FLAG_CREATE, nullptr,  0, folder ) ) )
		AddFolderPathData( gOpenFileDialogData.specialDirectories, "Windows", MString( "%s\\", folder ) );
}

void EditorGuiManager::OnDoubleClickFileInOpenFileDialog( const MFile& file, const bool& importAll )
{
	gOpenFileDialogData.currentSelectedFolderIndex = -1;
	gOpenFileDialogData.currentSelectedFileIndex = -1;

	isOpenFileDialogOpend = false;

	switch ( gOpenFileDialogData.actionData.action )
	{
	case OpenFileDialogAction::OFDA_Import3DModel:

		Model3DImporter::Import( MString( "%s%s", file.path.GetCStr(), file.name.GetCStr() ) );
		break;
	case OpenFileDialogAction::OFDA_Import2DTexture:
		if ( importAll )
		{
			vector<MFile> allFiles = Application::GetAllFiles( file.path, MFileFilter::MFF_File );
			for ( vector<MFile>::iterator it = allFiles.begin(); it != allFiles.end(); ++it )
				Texture2DImporter::Import( MString( "%s%s", it->path.GetCStr(), it->name.GetCStr() ) );
		}
		else
			Texture2DImporter::Import( MString( "%s%s", file.path.GetCStr(), file.name.GetCStr() ) );
		break;
	case OpenFileDialogAction::OFDA_ImportCubemap:
		CubemapImporter::Import( file );
		break;
	}
}


void EditorGuiManager::SetEnterGameLoopCallbackFunc( void (*callbackFunc)() )
{
	this->enterGameLoopCallbackFunc = callbackFunc;
}


void EditorGuiManager::CreateNewMaterial()
{
	NulloutCurrentSelectedAssets();
	materialToBeSelectedInAsset = MMaterial::Create( MShader::Get( "Diffuse" ), "New Material" );
	importSettingsFlags = ImGuiTabItemFlags_SetSelected;
}





void EditorGuiManager::StartDisableElement()
{
	ImGui::PushItemFlag( ImGuiItemFlags_Disabled, true );
	ImGui::PushStyleVar( ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f );
}
void EditorGuiManager::EndDisableElement()
{
	ImGui::PopItemFlag();
	ImGui::PopStyleVar();
}

ImVec4 EditorGuiManager::GetImVec4( const Color& color )
{
	return ImVec4( color.r, color.g, color.b, color.a );
}
Color EditorGuiManager::GetColor( const ImVec4& vec4Color )
{
	return Color( vec4Color.x, vec4Color.y, vec4Color.z, vec4Color.w );
}

float EditorGuiManager::GetTextWidth( const char* txt )
{
	ImVec2 textSize = ImGui::CalcTextSize( txt );
	return textSize.x;
}
float EditorGuiManager::CalcTextOffset( const char* txt, float targetOffset )
{
	return targetOffset - GetTextWidth( txt );
}


GameObject* EditorGuiManager::GetCurrentSelectedGameObjectInScene()
{
	return currentSelectedGameObjectInScene;
}

GameObject* EditorGuiManager::GetCurrentGameObjectToBeRemovedFromScene()
{
	return gameObjectToBeRemovedFromScene;
}

void EditorGuiManager::SetCurrentSelectedPrefabInAsset( GameObject* go )
{
	currentSelectedPrefabInAsset = go;
}
GameObject* EditorGuiManager::GetCurrentSelectedPrefabInAsset()
{
	return currentSelectedPrefabInAsset;
}


void EditorGuiManager::AddMeshRendererToCurrentSelectedGameObjectInScene()
{
	if ( currentSelectedGameObjectInScene->GetComponent<MeshRenderer>() == nullptr )
	{
		MeshRenderer* meshRenderer = currentSelectedGameObjectInScene->AddComponent<MeshRenderer>();
		MMaterial* newMat = MMaterial::Create( MShader::Get( "Diffuse" ), MString( "%sMat", currentSelectedGameObjectInScene->name.GetCStr() ) );
		meshRenderer->SetMaterial( newMat );
	}
}

void EditorGuiManager::DeleteCurrentGameObjectToBeRemovedFromScene()
{
	// Delete this object from the sceneGroup.
	SceneManager::DeleteSceneGameObject( gameObjectToBeRemovedFromScene );

	// Nullout the references for the current selected object and the object that need to be destroyed.
	gameObjectToBeRemovedFromScene = nullptr;
	currentSelectedGameObjectInScene = nullptr;
}

void EditorGuiManager::OnClickToolBarPlayBtn()
{
	if ( enterGameLoopCallbackFunc != nullptr )
		enterGameLoopCallbackFunc();
}

void EditorGuiManager::OnCreateNewSceneObjectFromMenuBar()
{
	guiSceneHierarchyWindow->SetSceneHierarchyNode_clicked_ID( SceneManager::GetSceneGameObjectsCount() - 1 );
}

void EditorGuiManager::OnClickItemInSceneHierarchy( Transform* root )
{
	if ( currentSelectedGameObjectInScene != nullptr )
		currentSelectedGameObjectInScene->SetType( GameObjectType::GOT_NORMAL, true );

	if ( root == nullptr )
	{
		currentSelectedGameObjectInScene = nullptr;
		isCurrentSelectedGameObjectActive = false;
		return;
	}

	currentSelectedGameObjectInScene = root->GetGameObject();
	isCurrentSelectedGameObjectActive = currentSelectedGameObjectInScene->GetActive();
	currentSelectedGameObjectInScene->SetType( GameObjectType::GOT_NORMAL_SELECTED, true );

	// Create a new outline object if we don't have one already.
	SceneManager::CreateGameObjectOutline( MString( "%s_Outline", currentSelectedGameObjectInScene->name.GetCStr() ), currentSelectedGameObjectInScene );
}

void EditorGuiManager::SetCurrentGameObjectToBeRemovedFromScene( GameObject* toBeRemoved )
{
	gameObjectToBeRemovedFromScene = toBeRemoved;
}

void EditorGuiManager::SetCurrentSelectedTextureInAsset( MTexture* tex )
{
	currentSelectedTextureInAsset = tex;
	textureToBeSelectedFromAsset = nullptr;
}
MTexture* EditorGuiManager::GetCurrentSelectedTextureInAsset()
{
	return currentSelectedTextureInAsset;
}
MTexture* EditorGuiManager::GetTextureToBeSelectedFromAsset()
{
	return textureToBeSelectedFromAsset;
}

void EditorGuiManager::SetCurrentSelectedCubemapInAsset( MCubemap* cubeMap )
{
	currentSelectedCubemapInAsset = cubeMap;
}
MCubemap* EditorGuiManager::GetCurrentSelectedCubemapInAsset()
{
	return currentSelectedCubemapInAsset;
}

void EditorGuiManager::SetCurrentSelectedMaterialInAsset( MMaterial* mat )
{
	currentSelectedMaterialInAsset = mat;
	materialToBeSelectedInAsset = nullptr;
}
MMaterial* EditorGuiManager::GetCurrentSelectedMaterialInAsset()
{
	return currentSelectedMaterialInAsset;
}
MMaterial* EditorGuiManager::GetMaterialToBeSelectedInAsset()
{
	return materialToBeSelectedInAsset;
}

void EditorGuiManager::SetCurrentSelectedMeshInAsset( MMesh* mesh )
{
	currentSelectedMeshInAsset = mesh;
}
MMesh* EditorGuiManager::GetCurrentSelectedMeshInAsset()
{
	return currentSelectedMeshInAsset;
}


void EditorGuiManager::SetImportSettingsFlags( ImGuiTabItemFlags importSettingsFlags )
{
	this->importSettingsFlags = importSettingsFlags;
}
ImGuiTabItemFlags EditorGuiManager::GetImportSettingsFlags()
{
	return importSettingsFlags;
}

bool EditorGuiManager::GetIsCurrentSelectedGameObjectActive()
{
	return isCurrentSelectedGameObjectActive;
}




// Static Functions

void EditorGuiManager::BrowseDrive( EditorGuiManager* instance, MString newDrive )
{
	instance->gOpenFileDialogData.driveLetter = newDrive;
	instance->gOpenFileDialogData.splitPathData.clear();

	// Remove '\' from drive name we don't want to display it
	instance->AddFolderPathToSplitPath( MString::GetSubStr( newDrive, 0, 2 ), newDrive );
	instance->ClearPreviousPathStack();
	instance->gOpenFileDialogData.currentPath = newDrive;
}

//void MANA3DEditorGUI::DisplaySceneWindow( unsigned int textureColorbuffer, ImVec2 pos )
//{
//	bool isOp = true;
//
//	ImGui::Begin( "Scene Window" );
//
//	// 1st: is texture of the FBO.
//	// 2nd: is the upper left corner for the uvs to be applied at.
//	// 3rd: is the lower right corner.
//	// 4th/5th: are the UVs -> they have to be flipped ( normally they would be (0,0); (1,1) ).
//	ImGui::GetWindowDrawList()->AddImage(
//		(void *)textureColorbuffer,
//		ImVec2( pos ),
//		ImVec2( pos.x + Screen::GetWidth()/2, pos.y + Screen::GetHeight()/2 ), 
//		ImVec2( 0, 1 ), 
//		ImVec2( 1, 0 ) );
//
//	ImGui::End();
//}

