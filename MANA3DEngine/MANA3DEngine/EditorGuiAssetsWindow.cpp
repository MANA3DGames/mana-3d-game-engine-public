#include "EditorGuiAssetsWindow.h"
#include "EditorGuiManager.h"
#include "MTexture.h"
#include "MMaterial.h"
#include "MMesh.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Debug.h"
using namespace MANA3D;

using std::map;
using std::string;

ImGuiTabItemFlags gAssetsWindowFlags;

int displayNow = 0;



EditorGuiAssetsWindow::EditorGuiAssetsWindow( EditorGuiManager* editorGuiManager ) : editorGuiManager( editorGuiManager ) 
{
	gAssetsWindowFlags = ImGuiWindowFlags_NoScrollbar |
						 ImGuiWindowFlags_NoTitleBar |
						 ImGuiWindowFlags_NoResize |
						 ImGuiWindowFlags_AlwaysAutoResize |
						 ImGuiWindowFlags_NoSavedSettings |
						 ImGuiWindowFlags_NoFocusOnAppearing;
}

EditorGuiAssetsWindow::~EditorGuiAssetsWindow() {}


void EditorGuiAssetsWindow::Display( map<string, MTexture*>& privateTexturesMap )
{
	auto utilWindSize = editorGuiManager->GetUtilitiesWindowSize();

	float categoriesWindowWidth = utilWindSize.x * 0.08f;
	if (categoriesWindowWidth < 150)
		categoriesWindowWidth = 150;

	float windowHeight = utilWindSize.y * 0.75f;


	DisplayImportButtons();
	displayNow = DisplayNavigationButoons( categoriesWindowWidth, windowHeight );

	ImGui::SameLine();


	float assetContentBrowserWindowWidth = utilWindSize.x - categoriesWindowWidth - 120.0f;
	float itemWidth = 140.0f;
	float itemWidthExtra = 160.0f;

	ImGui::BeginChild( "AssetContentBrowser", ImVec2( assetContentBrowserWindowWidth, windowHeight ), true );

	// Check if there is a texture selected in the inspector so we need to navigate to that texture in the assets browser.
	auto textureToBeSelectedFromAsset = editorGuiManager->GetTextureToBeSelectedFromAsset();
	if ( textureToBeSelectedFromAsset != nullptr )
	{
		displayNow = 2;
		editorGuiManager->SetCurrentSelectedTextureInAsset( textureToBeSelectedFromAsset );
		auto currentSelectedTextureInAsset = editorGuiManager->GetCurrentSelectedTextureInAsset();
		int texIndex = 0;
		map<string, MTexture*> allTextures = MTexture::GetAllTextures();
		for ( map<string, MTexture*>::iterator it = allTextures.begin(); it != allTextures.end(); ++it )
		{
			if ( currentSelectedTextureInAsset == it->second )
			{
				// Calculate maximum item count in a single row.
				int maxItemsInRow = (int)( ImGui::GetWindowWidth() / itemWidthExtra );
				// Set target scroll value ( TextureIndex / MaxItemCoutInRow x ItemHeight )
				ImGui::SetScrollY( (int)( texIndex / maxItemsInRow ) * itemWidth );
				break;
			}

			texIndex++;
		}
	}

	auto materialToBeSelectedInAsset = editorGuiManager->GetMaterialToBeSelectedInAsset();
	if ( materialToBeSelectedInAsset != nullptr )
	{
		displayNow = 3;
		editorGuiManager->SetCurrentSelectedMaterialInAsset( materialToBeSelectedInAsset );
		
		int matIndex = 0;
		map<string, MMaterial*> allMats = MMaterial::GetAllMaterials();
		for ( map<string, MMaterial*>::iterator it = allMats.begin(); it != allMats.end(); ++it )
		{
			if ( editorGuiManager->GetCurrentSelectedMaterialInAsset() == it->second )
			{
				// Calculate maximum item count in a single row.
				int maxItemsInRow = (int)( ImGui::GetWindowWidth() / itemWidthExtra );
				// Set target scroll value ( matIndex / MaxItemCoutInRow x ItemHeight )
				ImGui::SetScrollY( (int)( matIndex / maxItemsInRow ) * itemWidth );
				break;
			}

			matIndex++;
		}
	}


	editorGuiManager->SetImportSettingsFlags( ImGuiTabItemFlags_None );

	// Display Prefabs.
	if ( displayNow == 0 )
		DisplayPrefabs( privateTexturesMap, itemWidth, itemWidthExtra, assetContentBrowserWindowWidth );
	// Display Meshes.
	else if ( displayNow == 1 )
		DisplayMeshes( privateTexturesMap, itemWidth, itemWidthExtra, assetContentBrowserWindowWidth );
	// Display Textures.
	else if ( displayNow == 2 )
		DisplayTextures( privateTexturesMap, itemWidth, itemWidthExtra, assetContentBrowserWindowWidth );
	// Display Materials.
	else if ( displayNow == 3 )
		DisplayMaterials( privateTexturesMap, itemWidth, itemWidthExtra, assetContentBrowserWindowWidth );
	// Display Cubemaps.
	else if ( displayNow == 4 )
		DisplayCubemaps( privateTexturesMap, itemWidth, itemWidthExtra, assetContentBrowserWindowWidth );

	ImGui::EndChild();
}


void EditorGuiAssetsWindow::DisplayImportButtons()
{
	ImGui::Spacing();
	if ( ImGui::Button( "Import 3D Model" ) )
		editorGuiManager->StartOpenFileDialogAs( "Import Asset : 3D Model", "Import", OpenFileDialogAction::OFDA_Import3DModel );

	ImGui::SameLine();
	if ( ImGui::Button( "Import Texture" ) )
		editorGuiManager->StartOpenFileDialogAs( "Import Asset : 2D Texture", "Import", OpenFileDialogAction::OFDA_Import2DTexture );

	ImGui::SameLine();
	if ( ImGui::Button( "Import Cubemap" ) )
		editorGuiManager->StartOpenFileDialogAs( "Import Asset : Cubemap", "Import", OpenFileDialogAction::OFDA_ImportCubemap );

	ImGui::Separator();
	ImGui::Dummy( ImVec2( 0.0f, 5.0f ) );
}
int EditorGuiAssetsWindow::DisplayNavigationButoons( float categoriesWindowWidth, float windowHeight )
{
	ImGui::BeginChild( "AssetCategoriesWindow", ImVec2( categoriesWindowWidth, windowHeight ), true );

	ImVec4 selectedBtnColor = ImVec4( 0.4f, 0.5f, 0.7f, 1.0f );
	ImVec4 defaultBtnColor = ImGui::GetStyleColorVec4( ImGuiCol_Button );

	float btnWidth = categoriesWindowWidth * 0.9f;
	if ( btnWidth < 100 )
		btnWidth = 100;
	ImVec2 btnSize = ImVec2( btnWidth, 20.0f );


	ImGui::PushStyleColor( ImGuiCol_Button, displayNow == 0 ? selectedBtnColor : defaultBtnColor );
	if ( ImGui::Button( "Prefabs", btnSize ) )
	{
		displayNow = 0;
	}
	ImGui::PopStyleColor();

	ImGui::Separator();
	ImGui::Spacing();


	ImGui::PushStyleColor( ImGuiCol_Button, displayNow == 1 ? selectedBtnColor : defaultBtnColor );
	if ( ImGui::Button( "Meshes", btnSize ) )
	{
		displayNow = 1;
	}
	ImGui::PopStyleColor();


	ImGui::PushStyleColor( ImGuiCol_Button, displayNow == 2 ? selectedBtnColor : defaultBtnColor );
	if ( ImGui::Button( "Textures", btnSize ) )
	{
		displayNow = 2;
	}
	ImGui::PopStyleColor();


	ImGui::PushStyleColor( ImGuiCol_Button, displayNow == 3 ? selectedBtnColor : defaultBtnColor );
	if ( ImGui::Button( "Materials", btnSize ) )
	{
		displayNow = 3;
	}
	ImGui::PopStyleColor();


	ImGui::PushStyleColor( ImGuiCol_Button, displayNow == 4 ? selectedBtnColor : defaultBtnColor );
	if ( ImGui::Button( "Cubemaps", btnSize ) )
	{
		displayNow = 4;
	}
	ImGui::PopStyleColor();


	ImGui::PushStyleColor( ImGuiCol_Button, displayNow == 5 ? selectedBtnColor : defaultBtnColor );
	if ( ImGui::Button( "Skinned Meshes", btnSize ) )
	{
		displayNow = 5;
	}
	ImGui::PopStyleColor();


	ImGui::PushStyleColor( ImGuiCol_Button, displayNow == 6 ? selectedBtnColor : defaultBtnColor );
	if ( ImGui::Button( "Skeleton", btnSize ) )
	{
		displayNow = 6;
	}
	ImGui::PopStyleColor();



	ImGui::EndChild();

	return displayNow;
}

void EditorGuiAssetsWindow::DisplayPrefabs( map<string, MTexture*>& privateTexturesMap, float itemWidth, float itemWidthExtra, float assetContentBrowserWindowWidth )
{
	MakeDragDropTargetToCreateNewPrefabAsset();


	float totalItemsWidth = 0.0f;

	map<string, GameObject*> prefabs = SceneManager::GetAllPrefabs();
	for ( map<string, GameObject*>::iterator it = prefabs.begin(); it != prefabs.end(); ++it )
	{
		if ( ImGui::BeginChild( it->second->name.GetCStr(), ImVec2( itemWidth, itemWidth ), false, gAssetsWindowFlags ) )
		{
			ImGui::Dummy( ImVec2( 2.0f, 0.0f ) );
			ImGui::SameLine();

			bool isSelected = editorGuiManager->GetCurrentSelectedPrefabInAsset() == it->second;
			int padding = 0;
			if ( isSelected )
			{
				padding = -1;
				ImGui::PushStyleColor( ImGuiCol_Border, editorGuiManager->GetImVec4( Color::Green ) );
			}
			if ( ImGui::ImageButton( (void*)(intptr_t)privateTexturesMap.find( "Skeleton.png" )->second->GetID(), 
										ImVec2( 100, 100 ), ImVec2( 0, 0 ), ImVec2( 1, 1 ), padding, editorGuiManager->GetImVec4( Color::Black ) ) )
			{
				editorGuiManager->NulloutCurrentSelectedAssets();
				editorGuiManager->SetCurrentSelectedPrefabInAsset( it->second );
			}
			if ( isSelected )
				ImGui::PopStyleColor();

			if ( ImGui::IsItemHovered() )
				ImGui::SetTooltip( "Prefab: %s", it->first.c_str() );

			// Make this asset dragable.
			DragDropGameObject dDPA;
			dDPA.name = it->first;
			dDPA.go = it->second;
			MakeDragDropSourcePrefab( "PrefabDragDrop", it->first, dDPA );

			MString titleText = editorGuiManager->AddAssetElementTitle( it->first );

			if ( ImGui::BeginPopupContextWindow())
			{
				ImGui::TextColored( ImVec4( 0.0f, 1.0f, 0.0f, 1.0f ), MString( "Prefab: %s", titleText.GetCStr() ).GetCStr() );
				ImGui::Separator();
				ImGui::Spacing();

				if ( ImGui::MenuItem( "Settings", nullptr, false, true ) )
				{
					editorGuiManager->NulloutCurrentSelectedAssets();
					editorGuiManager->SetCurrentSelectedPrefabInAsset( it->second );
					editorGuiManager->SetImportSettingsFlags( ImGuiTabItemFlags_SetSelected );
				}

				ImGui::Spacing();

				if ( ImGui::MenuItem( "Delete", nullptr, false, true ) )
				{
					SceneManager::DestroyPrefabAsset( it->second );
					editorGuiManager->SetCurrentSelectedPrefabInAsset( nullptr );
				}

				ImGui::EndPopup();
			}
		}
		ImGui::EndChild();

		totalItemsWidth += itemWidthExtra;

		if ( totalItemsWidth >= assetContentBrowserWindowWidth - itemWidthExtra )
			totalItemsWidth = 0.0f;
		else
			ImGui::SameLine();
	}
}
void EditorGuiAssetsWindow::DisplayMeshes( map<string, MTexture*>& privateTexturesMap, float itemWidth, float itemWidthExtra, float assetContentBrowserWindowWidth )
{
	float totalItemsWidth = 0.0f;

	map<string, MMesh*> allMeshes = MMesh::GetAllMeshes();
	for ( map<string, MMesh*>::iterator it = allMeshes.begin(); it != allMeshes.end(); ++it )
	{
		if ( ImGui::BeginChild( it->second->GetName().GetCStr(), ImVec2( itemWidth, itemWidth ), false, gAssetsWindowFlags ) )
		{
			ImGui::Dummy( ImVec2( 2.0f, 0.0f ) );
			ImGui::SameLine();

			bool isSelected = editorGuiManager->GetCurrentSelectedMeshInAsset() == it->second;
			int padding = 0;
			if ( isSelected )
			{
				padding = -1;
				ImGui::PushStyleColor( ImGuiCol_Border, editorGuiManager->GetImVec4( Color::Green ) );
			}
			if ( ImGui::ImageButton( (void*)(intptr_t)privateTexturesMap.find( "Mesh.png" )->second->GetID(), 
										ImVec2( 100, 100 ), ImVec2( 0, 0 ), ImVec2( 1, 1 ), padding, editorGuiManager->GetImVec4( Color::Black ) ) )
			{
				editorGuiManager->NulloutCurrentSelectedAssets();
				editorGuiManager->SetCurrentSelectedMeshInAsset( it->second );
			}
			if ( isSelected )
				ImGui::PopStyleColor();

			if ( ImGui::IsItemHovered() )
				ImGui::SetTooltip( "Mesh: %s", it->first.c_str() );

			// Make this asset dragable.
			DragDropMesh dDM;
			dDM.name = it->first;
			dDM.mesh = it->second;
			editorGuiManager->MakeDragDropSourceMesh( "MeshAssetDragDrop", it->first, dDM );

			MString titleText = editorGuiManager->AddAssetElementTitle( it->first );

			if ( ImGui::BeginPopupContextWindow())
			{
				ImGui::TextColored( ImVec4( 0.0f, 1.0f, 0.0f, 1.0f ), MString( "Mesh: %s", titleText.GetCStr() ).GetCStr() );
				ImGui::Separator();
				ImGui::Spacing();

				if ( ImGui::MenuItem( "Settings", nullptr, false, true ) )
				{
					editorGuiManager->NulloutCurrentSelectedAssets();
					editorGuiManager->SetCurrentSelectedMeshInAsset( it->second );
					editorGuiManager->SetImportSettingsFlags( ImGuiTabItemFlags_SetSelected );
				}

				ImGui::Spacing();

				if ( ImGui::MenuItem( "Delete", nullptr, false, true ) )
				{
					MMesh::Delete( it->second );
					editorGuiManager->SetCurrentSelectedMeshInAsset( nullptr );
				}

				ImGui::EndPopup();
			}
		}
		ImGui::EndChild();

		totalItemsWidth += itemWidthExtra;

		if ( totalItemsWidth >= assetContentBrowserWindowWidth - itemWidthExtra )
			totalItemsWidth = 0.0f;
		else
			ImGui::SameLine();
	}
}
void EditorGuiAssetsWindow::DisplayTextures( map<string, MTexture*>& privateTexturesMap, float itemWidth, float itemWidthExtra, float assetContentBrowserWindowWidth )
{
	float totalItemsWidth = 0.0f;

	map<string, MTexture*> allTextures = MTexture::GetAllTextures();
	for ( map<string, MTexture*>::iterator it = allTextures.begin(); it != allTextures.end(); ++it )
	{
		if ( ImGui::BeginChild( it->second->GetName().GetCStr(), ImVec2( itemWidth, itemWidth ), false, gAssetsWindowFlags ) )
		{
			ImGui::Dummy( ImVec2( 2.0f, 0.0f ) );
			ImGui::SameLine();

			bool isSelectedTexture = editorGuiManager->GetCurrentSelectedTextureInAsset() == it->second;
			int padding = 0;
			if ( isSelectedTexture )
			{
				padding = -1;
				ImGui::PushStyleColor( ImGuiCol_Border, editorGuiManager->GetImVec4( Color::Green ) );
			}
			if ( ImGui::ImageButton( (void*)(intptr_t)it->second->GetID(), ImVec2( 100, 100 ), ImVec2( 0, 0 ), ImVec2( 1, 1 ), padding, editorGuiManager->GetImVec4( Color::Black ) ) )
			{
				editorGuiManager->NulloutCurrentSelectedAssets();
				editorGuiManager->SetCurrentSelectedTextureInAsset( it->second );
				editorGuiManager->SetTempTargetWrapMode( editorGuiManager->GetCurrentSelectedTextureInAsset()->GetWrapModeName() );
			}
			if ( isSelectedTexture )
				ImGui::PopStyleColor();

			if ( ImGui::IsItemHovered() )
				ImGui::SetTooltip( "Tex2D: %s", it->second->GetName().GetCStr() );

			// Make drag drop target for texture.
			DragDropTexture dDT;
			dDT.name = it->first;
			dDT.texture = it->second;
			editorGuiManager->MakeDragDropSourceTexture( "TextureAssetDragDrop", it->first, dDT );

			MString titleText = editorGuiManager->AddAssetElementTitle( it->first );

			if ( ImGui::BeginPopupContextWindow())
			{
				ImGui::TextColored( ImVec4( 0.0f, 1.0f, 0.0f, 1.0f ), MString( "Tex2D: %s", titleText.GetCStr() ).GetCStr() );
				ImGui::Separator();
				ImGui::Spacing();

				if ( ImGui::MenuItem( "Settings", nullptr, false, true ) )
				{
					editorGuiManager->NulloutCurrentSelectedAssets();
					editorGuiManager->SetCurrentSelectedTextureInAsset( it->second );
					editorGuiManager->SetTempTargetWrapMode( editorGuiManager->GetCurrentSelectedTextureInAsset()->GetWrapModeName() );
					editorGuiManager->SetImportSettingsFlags( ImGuiTabItemFlags_SetSelected );
				}

				ImGui::Spacing();

				if ( ImGui::MenuItem( "Delete", nullptr, false, true ) )
				{
					MTexture::Delete( it->second );
					editorGuiManager->SetCurrentSelectedTextureInAsset( nullptr );
				}

				ImGui::EndPopup();
			}
		}
		ImGui::EndChild();

		totalItemsWidth += itemWidthExtra;

		if ( totalItemsWidth >= assetContentBrowserWindowWidth - itemWidthExtra )
			totalItemsWidth = 0.0f;
		else
			ImGui::SameLine();
	}
}
void EditorGuiAssetsWindow::DisplayMaterials( map<string, MTexture*>& privateTexturesMap, float itemWidth, float itemWidthExtra, float assetContentBrowserWindowWidth )
{
	float totalItemsWidth = 0.0f;

	map<string, MMaterial*> allMaterials = MMaterial::GetAllMaterials();
	for ( map<string, MMaterial*>::iterator it = allMaterials.begin(); it != allMaterials.end(); ++it )
	{
		if ( ImGui::BeginChild( it->second->GetName().GetCStr(), ImVec2( itemWidth, itemWidth ), false, gAssetsWindowFlags ) )
		{
			ImGui::Dummy( ImVec2( 2.0f, 0.0f ) );
			ImGui::SameLine();

			bool isSelected = editorGuiManager->GetCurrentSelectedMaterialInAsset() == it->second;
			int padding = 0;
			if ( isSelected )
			{
				padding = -1;
				ImGui::PushStyleColor( ImGuiCol_Border, editorGuiManager->GetImVec4( Color::Green ) );
			}

			ImTextureID texID = nullptr;
			map<string, MTextureIndexPair> allTextureIndexPairs = it->second->GetTextureIndexPairsMap();
			if ( allTextureIndexPairs.size() > 0 )
			{
				map<string, MTextureIndexPair>::iterator matIt = allTextureIndexPairs.begin();
				if ( matIt != allTextureIndexPairs.end() )
				{
					if ( matIt->second.tex != nullptr )
						texID = (void*)(intptr_t)( matIt->second.tex->GetID() );
				}
			}
			if ( ImGui::ImageButton( texID, ImVec2( 100, 100 ), ImVec2( 0, 0 ), ImVec2( 1, 1 ), padding, editorGuiManager->GetImVec4( Color::Black ) ) )
			{
				editorGuiManager->NulloutCurrentSelectedAssets();
				editorGuiManager->SetCurrentSelectedMaterialInAsset( it->second );
			}
			if ( isSelected )
				ImGui::PopStyleColor();

			if ( ImGui::IsItemHovered() )
				ImGui::SetTooltip( "Mat: %s", it->first.c_str() );

			// Make this asset dragable.
			DragDropMaterial dDM;
			dDM.name = it->first;
			dDM.material = it->second;
			editorGuiManager->MakeDragDropSourceMaterial( "MaterialAssetDragDrop", it->first, dDM );

			MString titleText = editorGuiManager->AddAssetElementTitle( it->first );

			if ( ImGui::BeginPopupContextWindow())
			{
				ImGui::TextColored( ImVec4( 0.0f, 1.0f, 0.0f, 1.0f ), MString( "Mat: %s", titleText.GetCStr() ).GetCStr() );
				ImGui::Separator();
				ImGui::Spacing();

				if ( ImGui::MenuItem( "Settings", nullptr, false, true ) )
				{
					editorGuiManager->NulloutCurrentSelectedAssets();
					editorGuiManager->SetCurrentSelectedMaterialInAsset( it->second );
					editorGuiManager->SetImportSettingsFlags( ImGuiTabItemFlags_SetSelected );
				}

				ImGui::Spacing();

				if ( ImGui::MenuItem( "Delete", nullptr, false, true ) )
				{
					MMaterial::Delete( it->second );
					editorGuiManager->SetCurrentSelectedMaterialInAsset( nullptr );
				}

				ImGui::EndPopup();
			}
		}
		ImGui::EndChild();

		totalItemsWidth += itemWidthExtra;

		if ( totalItemsWidth >= assetContentBrowserWindowWidth - itemWidthExtra )
			totalItemsWidth = 0.0f;
		else
			ImGui::SameLine();
	}

	if ( ImGui::BeginPopupContextWindow() )
	{
		ImGui::TextColored( ImVec4( 0.0f, 1.0f, 0.0f, 1.0f ), "Material Options" );
		ImGui::Separator();
		ImGui::Spacing();

		if ( ImGui::MenuItem( "Create New Material", nullptr, false, true ) )
		{
			editorGuiManager->CreateNewMaterial();
		}

		ImGui::EndPopup();
	}
}
void EditorGuiAssetsWindow::DisplayCubemaps( map<string, MTexture*>& privateTexturesMap, float itemWidth, float itemWidthExtra, float assetContentBrowserWindowWidth )
{
	float totalItemsWidth = 0.0f;

	map<string, MCubemap*> allCubemaps = MCubemap::GetAllCubemaps();
	for ( map<string, MCubemap*>::iterator it = allCubemaps.begin(); it != allCubemaps.end(); ++it )
	{
		if ( ImGui::BeginChild( it->second->GetName().GetCStr(), ImVec2( itemWidth, itemWidth ), false, gAssetsWindowFlags ) )
		{
			ImGui::Dummy( ImVec2( 2.0f, 0.0f ) );
			ImGui::SameLine();

			bool isSelected = editorGuiManager->GetCurrentSelectedCubemapInAsset() == it->second;
			int padding = 0;
			if ( isSelected )
			{
				padding = -1;
				ImGui::PushStyleColor( ImGuiCol_Border, editorGuiManager->GetImVec4( Color::Green ) );
			}
			if ( ImGui::ImageButton( (void*)(intptr_t)it->second->GetThumbTex()->GetID(), 
										ImVec2( 100, 100 ), ImVec2( 0, 0 ), ImVec2( 1, 1 ), padding, editorGuiManager->GetImVec4( Color::Black ) ) )
			{
				editorGuiManager->NulloutCurrentSelectedAssets();
				editorGuiManager->SetCurrentSelectedCubemapInAsset( it->second );
			}
			if ( isSelected )
				ImGui::PopStyleColor();

			if ( ImGui::IsItemHovered() )
				ImGui::SetTooltip( "Cubemap: %s", it->second->GetName().GetCStr() );

			MString titleText = editorGuiManager->AddAssetElementTitle( it->first );

			if ( ImGui::BeginPopupContextWindow())
			{
				ImGui::TextColored( ImVec4( 0.0f, 1.0f, 0.0f, 1.0f ), MString( "Cubemap: %s", titleText.GetCStr() ).GetCStr() );
				ImGui::Separator();
				ImGui::Spacing();

				if ( ImGui::MenuItem( "Import Settings", nullptr, false, true ) )
				{
					editorGuiManager->NulloutCurrentSelectedAssets();
					editorGuiManager->SetCurrentSelectedCubemapInAsset( it->second );
					editorGuiManager->SetImportSettingsFlags( ImGuiTabItemFlags_SetSelected );
				}

				ImGui::Spacing();

				if ( ImGui::MenuItem( "Delete", nullptr, false, true ) )
				{
					MCubemap::Destroy( it->second );
					editorGuiManager->SetCurrentSelectedCubemapInAsset( nullptr );
				}

				ImGui::EndPopup();
			}
		}
		ImGui::EndChild();

		totalItemsWidth += itemWidthExtra;

		if ( totalItemsWidth >= assetContentBrowserWindowWidth - itemWidthExtra )
			totalItemsWidth = 0.0f;
		else
			ImGui::SameLine();
	}
}
void EditorGuiAssetsWindow::DisplaySkinnedMeshes( map<string, MTexture*>& privateTexturesMap, float itemWidth, float itemWidthExtra, float assetContentBrowserWindowWidth )
{
	float totalItemsWidth = 0.0f;
}
void EditorGuiAssetsWindow::DisplaySkeletons( map<string, MTexture*>& privateTexturesMap, float itemWidth, float itemWidthExtra, float assetContentBrowserWindowWidth )
{
	float totalItemsWidth = 0.0f;
}


void EditorGuiAssetsWindow::MakeDragDropSourcePrefab( const MString& ddID, const MString& label, const DragDropGameObject& data )
{
	if ( ImGui::BeginDragDropSource( ImGuiDragDropFlags_None ) )
	{
		// Set payload to carry the index of our item (could be anything).
		ImGui::SetDragDropPayload( ddID, &data, sizeof(DragDropGameObject) );

		// Display preview (could be anything, e.g. when dragging an image we could decide to display the filename and a small preview of the image, etc.).
		ImGui::Text( label );

		ImGui::EndDragDropSource();
	}
}

void EditorGuiAssetsWindow::MakeDragDropTargetToCreateNewPrefabAsset()
{
	auto size = ImGui::GetWindowSize();
	auto pos = ImGui::GetWindowPos();

	ImRect rect = ImRect( ImVec2( pos.x, pos.y ), ImVec2( pos.x + size.x, pos.y + size.y ) );

	if ( ImGui::BeginDragDropTargetCustom( rect, ImGuiID( "GameObjectDragDrop" ) ) )
	{
		OnDropGameObjectOnPrefabsWindow();
		ImGui::EndDragDropTarget();
	}
}
void EditorGuiAssetsWindow::OnDropGameObjectOnPrefabsWindow()
{
	if ( const ImGuiPayload* payload = ImGui::AcceptDragDropPayload( "GameObjectDragDrop" ) )
	{
		IM_ASSERT( payload->DataSize == sizeof(DragDropGameObject) );
		DragDropGameObject payload_n = *(const DragDropGameObject*)payload->Data;
		SceneManager::CreatePrefabAsset( payload_n.go );
	}
}