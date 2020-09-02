#include "EditorGuiAssetsOverviewWindow.h"
#include "EditorGuiManager.h"
#include "Screen.h"
#include "MTexture.h"
#include "MCubemap.h"
#include "Texture2DImporter.h"
#include "GraphicSettings.h"
#include "MMaterial.h"
using namespace MANA3D;

#include <list>
using std::list;
using std::string;



EditorGuiAssetsOverviewWindow::EditorGuiAssetsOverviewWindow( EditorGuiManager* editorGuiManager ) : EditorGuiWindow( editorGuiManager )
{
}

EditorGuiAssetsOverviewWindow::~EditorGuiAssetsOverviewWindow() {}


void EditorGuiAssetsOverviewWindow::Display()
{
	bool isSceneOpened = true;

	ImGui::Begin( "AssetPreview", &isSceneOpened, ImVec2( 0 , 0 ), 1.0f,
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove | 
		ImGuiWindowFlags_NoTitleBar );

	ImGui::SetWindowPos( ImVec2( editorGuiManager->GetSceneHierarchyWindowSize().x + editorGuiManager->GetSceneWindowSize().x, editorGuiManager->GetSceneWindowSize().y + editorGuiManager->GetSceneWindowTopOffsetY() ) );

	ImVec2 windowSize = ImVec2( Screen::GetWidth() - editorGuiManager->GetUtilitiesWindowSize().x, editorGuiManager->GetUtilitiesWindowSize().y );
	ImGui::SetWindowSize( windowSize );

	ImGui::BeginTabBar( "AssetPreview#left_tabs_bar" );

	if ( ImGui::BeginTabItem( "Asset Preview" ) )
	{
		ImGui::BeginChild( "AssetPreview_PreviewWindow", ImVec2( windowSize.x - 50, windowSize.y - 50 ), true, ImGuiWindowFlags_HorizontalScrollbar );
		if ( editorGuiManager->GetCurrentSelectedTextureInAsset() != nullptr )
		{
			auto tex = editorGuiManager->GetCurrentSelectedTextureInAsset();
			ImGui::Image( (void*)(intptr_t)tex->GetID(), ImVec2( (float)tex->GetWidth(), (float)tex->GetHeight() ) );
		}
		else if ( editorGuiManager->GetCurrentSelectedCubemapInAsset() != nullptr )
		{
			MTexture* tex = editorGuiManager->GetCurrentSelectedCubemapInAsset()->GetThumbTex();
			ImGui::Image( (void*)(intptr_t)tex->GetID(), ImVec2( (float)tex->GetWidth(), (float)tex->GetHeight() ) );
		}
		ImGui::EndChild();

		ImGui::EndTabItem();
	}

	bool open = true;
	if ( ImGui::BeginTabItem( "Settings", &open, editorGuiManager->GetImportSettingsFlags() ) )
	{
		ImGui::BeginChild( "ImportSettings_SettingsWindow", ImVec2( windowSize.x - 50, windowSize.y - 50 ), true, ImGuiWindowFlags_HorizontalScrollbar );

		auto currentSelectedTextureInAsset = editorGuiManager->GetCurrentSelectedTextureInAsset();
		auto currentSelectedCubemapInAsset = editorGuiManager->GetCurrentSelectedCubemapInAsset();
		auto currentSelectedMaterialInAsset = editorGuiManager->GetCurrentSelectedMaterialInAsset();

		if ( currentSelectedTextureInAsset != NULL )
		{
			ImGui::Image( (void*)(intptr_t)currentSelectedTextureInAsset->GetID(), ImVec2( 100, 100 ), ImVec2( 0, 0 ), ImVec2( 1, 1 ), editorGuiManager->GetImVec4( Color::White ), editorGuiManager->GetImVec4( Color::White ) );
			ImGui::SameLine();
			ImGui::Spacing();
			ImGui::SameLine();
			int chCount = currentSelectedTextureInAsset->GetChannels();
			MString str;
			switch ( chCount )
			{
			case 0:
				str = "None";
				break;
			case 1:
				str = currentSelectedTextureInAsset->GetIsSRGB() ? "sRED" : "RED";
				break;
			case 2:
				str = "Unknown!";
				break;
			case 3:
				str = currentSelectedTextureInAsset->GetIsSRGB() ? "sRGB" : "RGB";
				break;
			case 4:
				str = currentSelectedTextureInAsset->GetIsSRGB() ? "sRGBA" : "RGBA";
				break;
			}

			MString myWrapMode( currentSelectedTextureInAsset->GetWrapModeName() );

			ImGui::Text( "Name: %s\nWidth: %i pixel\nHeight: %i pixel\nChannels: %s\nWrap Mode: %s", 
				currentSelectedTextureInAsset->GetName().GetCStr(), 
				currentSelectedTextureInAsset->GetWidth(), 
				currentSelectedTextureInAsset->GetHeight(), 
				str.GetCStr(),
				myWrapMode.GetCStr() );

			ImGui::Separator();
			ImGui::Spacing();

			list<string> allWrapModes = { "Repeat", "Clamp to Edge" };
			ImGui::Spacing();
			ImGui::Text( "Wrap Mode" );
			ImGui::SameLine();

			ImGui::PushItemWidth( 200.0f );
			MString id( "##hidelabel_ComboWrapModeOptions" );
			if ( ImGui::BeginCombo( id, editorGuiManager->GetTempTargetWrapMode().GetCStr() ) )
			{
				for ( list<string>::iterator it = allWrapModes.begin(); it != allWrapModes.end(); ++it )
				{
					MString str = (*it);
					bool is_selected = ( editorGuiManager->GetTempTargetWrapMode().GetCStr() == str );
					if ( ImGui::Selectable( str, is_selected ) )
						editorGuiManager->SetTempTargetWrapMode( str );

					if ( is_selected )
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();

			static bool flip;
			static bool sRGB;
			static MTexture* lastSelectedTexture;

			ImGui::Spacing();
			if ( lastSelectedTexture != currentSelectedTextureInAsset )
				flip = currentSelectedTextureInAsset->GetIsYFlipped();
			ImGui::Checkbox( "Flip Horizontal", &flip );

			ImGui::Spacing();
			if ( lastSelectedTexture != currentSelectedTextureInAsset )
				sRGB = currentSelectedTextureInAsset->GetIsSRGB();
			ImGui::Checkbox( "Use sRGB", &sRGB );

			ImGui::Dummy( ImVec2( 0.0f, 30.0f ) );
			if ( ImGui::Button( "Update", ImVec2( 100, 30 ) ) )
				Texture2DImporter::ReImport( currentSelectedTextureInAsset, flip, editorGuiManager->GetTempTargetWrapMode() == "Repeat" ? 0 : 1, sRGB );
			

			lastSelectedTexture = currentSelectedTextureInAsset;
		} 
		else if ( currentSelectedCubemapInAsset != nullptr )
		{
			ImGui::Image( (void*)(intptr_t)currentSelectedCubemapInAsset->GetThumbTex()->GetID(), ImVec2( 100, 100 ), ImVec2( 0, 0 ), ImVec2( 1, 1 ), editorGuiManager->GetImVec4( Color::White ), editorGuiManager->GetImVec4( Color::White ) );
			ImGui::SameLine();
			ImGui::Spacing();
			ImGui::SameLine();

			ImGui::Text( "Name: %s", currentSelectedCubemapInAsset->GetName().GetCStr() );

			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Dummy( ImVec2( 0.0f, 30.0f ) );
			if ( ImGui::Button( "Use As Skybox", ImVec2( 100, 30 ) ) )
				GraphicSettings::SetSkyboxCubemap( currentSelectedCubemapInAsset );
		}
		else if ( currentSelectedMaterialInAsset != nullptr )
		{
			ImGui::Spacing();

			MString matName = currentSelectedMaterialInAsset->GetName();

			char* nameStrBuf = matName.GetCharArray();
			ImGui::InputText( "##hidelabel_currentSelectedMaterialInAsset", nameStrBuf, sizeof( MString ) * 2 );

			if ( matName.GetLength() > 50 )
				matName = matName.GetStr().substr( 0, 50 );

			if ( currentSelectedMaterialInAsset->GetName() != matName )
				MMaterial::UpdateName( currentSelectedMaterialInAsset, matName );

			ImGui::Spacing();

			editorGuiManager->DisplayMaterialComponent( currentSelectedMaterialInAsset );
		}
		ImGui::EndChild();

		ImGui::EndTabItem();
	}

	ImGui::EndTabBar();

	ImGui::End();
}