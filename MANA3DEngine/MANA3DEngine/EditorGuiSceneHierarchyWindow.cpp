#include "EditorGuiSceneHierarchyWindow.h"
#include "EditorGuiManager.h"
#include "SceneManager.h"
#include "Transform.h"
#include "MString.h"
#include "CommandManager.h"
using namespace MANA3D;

#include <vector>
using std::vector;

#include <list>
using std::list;

#include <functional>



EditorGuiSceneHierarchyWindow::EditorGuiSceneHierarchyWindow( EditorGuiManager* editorGuiManager )
	: EditorGuiWindow( editorGuiManager ), sceneHierarchyNode_clicked_ID( -1 )
{
	isFocused = false;
	topOffsetY = 70.0f;
}

EditorGuiSceneHierarchyWindow::~EditorGuiSceneHierarchyWindow() { }



void EditorGuiSceneHierarchyWindow::Display()
{
	bool isSceneOpened = true;

	ImGui::Begin( "Scene Hierarchy", &isSceneOpened, ImVec2( 300, 576 ), 1.0f, 
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove );

	ImGui::SetWindowPos( ImVec2( 0, editorGuiManager->GetSceneWindowTopOffsetY() ) );
	ImGui::SetWindowSize( ImVec2( 300, editorGuiManager->GetSceneWindowSize().y ) );

	UpdateWindProperties();
	windSize = ImGui::GetWindowSize();


	// Make this window drop target for prefab assets.
	MakeDragDropTargetForPrefabAssets(); 
	// Also make the whole window as a drop target for unparenting item.
	MakeDragDropTargetForItemUnparenting();


	ImGui::Text( "Scene Hierarchy" );
	
	ImGui::Separator();
	ImGui::Spacing();


	if ( isFirstRun )
	{
		ImGui::SetNextTreeNodeOpen( true );
		isFirstRun = false;
	}


	if ( ImGui::IsWindowFocused() &&
		 ImGui::IsMouseHoveringWindow() && 
		 ImGui::IsMouseClicked( 0 ) )
	{
		editorGuiManager->OnClickItemInSceneHierarchy( nullptr );
		sceneHierarchyNode_clicked_ID = -1;
	}


	if ( ImGui::TreeNode( "Scene" ) )
	{
		// Also make Scene-treeNode a drap target for object parenting in the hierarchy.
		ImRect rect = ImRect( ImGui::GetItemRectMin(), ImGui::GetItemRectMax() );
		if ( ImGui::BeginDragDropTargetCustom( rect, ImGuiID( "GameObjectDragDrop" ) ) )
		{
			OnDropItemOnSlot( rect, nullptr );
			ImGui::EndDragDropTarget();
		}


		vector<GameObject*>& allNodes = SceneManager::GetSceneGameObjects();

		int id = 0;
		for ( vector<GameObject*>::iterator it = allNodes.begin(); it != allNodes.end(); ++it )
		{
			if ( (*it)->transform->GetParent() == nullptr )
				DrawSceneHierarchyNodeChildren( (*it)->transform, id );
			id++;
		}

		ImGui::TreePop();
	}


	ImGui::End();

	// Check if we have any GameObject that need to be deleted.
	if ( editorGuiManager->GetCurrentGameObjectToBeRemovedFromScene() != nullptr )
		editorGuiManager->DeleteCurrentGameObjectToBeRemovedFromScene();
}

void EditorGuiSceneHierarchyWindow::DrawSceneHierarchyNodeChildren( Transform* root, int& id )
{
	int currentIDBeforeClick = sceneHierarchyNode_clicked_ID;

	ImGuiTreeNodeFlags node_flags = ( root->GetChildren().size() > 0 ? ( ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick ) : ImGuiTreeNodeFlags_Leaf ) | 
									( sceneHierarchyNode_clicked_ID == id ? ImGuiTreeNodeFlags_Selected : 0 );

	auto go = root->GetGameObject();

	ImGui::PushStyleColor( ImGuiCol_Text, go->GetActive() ? ImVec4( 1.0f, 1.0f, 1.0f, 1.0f ) : ImVec4( 0.4f, 0.4f, 0.4f, 1.0f ) );
	bool node_open = ImGui::TreeNodeEx( (void*)(intptr_t)id, node_flags, go->name );
	ImGui::PopStyleColor();

	if ( ImGui::IsItemClicked( 0 ) || ImGui::IsItemClicked( 1 ) )
	{
		sceneHierarchyNode_clicked_ID = id;
		editorGuiManager->OnClickItemInSceneHierarchy( root );

		/*CommandManager::RegisterCommand( 
			[=]() {
				sceneHierarchyNode_clicked_ID = id;
				editorGuiManager->OnClickItemInSceneHierarchy( root );
			},
			[=]() {
				if ( currentIDBeforeClick != -1 )
				{
					sceneHierarchyNode_clicked_ID = id;
					editorGuiManager->OnClickItemInSceneHierarchy( root );
				}
				else
				{
					sceneHierarchyNode_clicked_ID = -1;
					editorGuiManager->OnClickItemInSceneHierarchy( nullptr );
				}
		} );*/
	}


	// Make this item dragable.
	DragDropGameObject dDPA;
	dDPA.name = go->name;
	dDPA.go = go;
	MakeDragDropSourceItem( "GameObjectDragDrop", go->name, dDPA );
	// Make this item drag target.
	ImRect rect = ImRect( ImGui::GetItemRectMin(), ImGui::GetItemRectMax() );
	MakeDragDropTargetItem( rect, go->transform );



	auto currentSelectedGameObjectInScene = editorGuiManager->GetCurrentSelectedGameObjectInScene();
	if ( currentSelectedGameObjectInScene != nullptr && ImGui::BeginPopupContextWindow() )
	{
		ImGui::TextColored( editorGuiManager->GetImVec4( Color::Green ), currentSelectedGameObjectInScene->name.GetCStr() );
		ImGui::Separator();
		ImGui::Spacing();

		if ( ImGui::MenuItem( "Delete", nullptr, false, true ) )
		{
			editorGuiManager->SetCurrentGameObjectToBeRemovedFromScene( currentSelectedGameObjectInScene );
		}

		ImGui::EndPopup();
	}

    if ( node_open )
    {
		list<Transform*> children = root->GetChildren();
        for ( list<Transform*>::iterator it = children.begin(); it != children.end(); ++it )
		{
			id++;
			DrawSceneHierarchyNodeChildren( *it, id );
		}

		ImGui::TreePop();
    }
}

void EditorGuiSceneHierarchyWindow::SetSceneHierarchyNode_clicked_ID( int id )
{
	sceneHierarchyNode_clicked_ID = id;
}





void EditorGuiSceneHierarchyWindow::MakeDragDropSourceItem( const MString& ddID, const MString& label, const DragDropGameObject& data )
{
	if ( ImGui::BeginDragDropSource( ImGuiDragDropFlags_None ) )
	{
		// Set payload to carry the index of our item (could be anything).
		ImGui::SetDragDropPayload( ddID, &data, sizeof(DragDropGameObject) );

		// Display preview.
		ImGui::Text( label );

		ImGui::EndDragDropSource();
	}
}
void EditorGuiSceneHierarchyWindow::MakeDragDropTargetItem( const ImRect& rect, Transform* newParent )
{
	if ( ImGui::BeginDragDropTargetCustom( rect, ImGuiID( "GameObjectDragDrop" ) ) )
	{
		OnDropItemOnSlot( rect, newParent );
		ImGui::EndDragDropTarget();
	}
}
void EditorGuiSceneHierarchyWindow::OnDropItemOnSlot( const ImRect& rect, Transform* newParent )
{
	if ( const ImGuiPayload * payload = ImGui::AcceptDragDropPayload( "GameObjectDragDrop" ) )
	{
		IM_ASSERT( payload->DataSize == sizeof(DragDropGameObject) );
		DragDropGameObject payload_n = *(const DragDropGameObject*)payload->Data;

		if ( payload_n.go != nullptr )
			payload_n.go->transform->SetParent( newParent );	
	}
}

void EditorGuiSceneHierarchyWindow::MakeDragDropTargetForItemUnparenting()
{
	ImRect rect = ImRect( ImVec2( 5, topOffsetY + 5 ), ImVec2( windSize.x - 10, topOffsetY + windSize.y - 10 ) );
	if ( ImGui::BeginDragDropTargetCustom( rect, ImGuiID( "GameObjectDragDrop" ) ) )
	{
		OnDropItemOnSlot( rect, nullptr );
		ImGui::EndDragDropTarget();
	}
}

void EditorGuiSceneHierarchyWindow::MakeDragDropTargetForPrefabAssets()
{
	ImRect rect = ImRect( ImVec2( 5, topOffsetY + 5 ), ImVec2( windSize.x - 10, topOffsetY + windSize.y - 10 ) );
	if ( ImGui::BeginDragDropTargetCustom( rect, ImGuiID( "ScenePrefabDragDrop" ) ) )
	{
		editorGuiManager->OnDropPrefabOnScene();
		ImGui::EndDragDropTarget();
	}
}
