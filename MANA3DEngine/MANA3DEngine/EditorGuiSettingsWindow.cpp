#include "EditorGuiSettingsWindow.h"
#include "EditorGuiManager.h"
#include "GraphicSettings.h"
#include "MCubemap.h"
using namespace MANA3D;

#include <list>
using std::list;
#include <string>
using std::string;


EditorGuiSettingsWindow::EditorGuiSettingsWindow( EditorGuiManager* editorGuiManager ) : editorGuiManager( editorGuiManager )
{

}

EditorGuiSettingsWindow::~EditorGuiSettingsWindow() {}


void EditorGuiSettingsWindow::Display()
{
	ImVec2 ySpace( 0.0f, 20.0f );
	float leftMargin = 10.0f;
	float checkboxOffset = 200.0f;

	ImGui::Dummy( ySpace );
	editorGuiManager->DrawCheckBox( leftMargin, checkboxOffset, "Enable GL Blending", GraphicSettings::GetIsBlendingEnabled(), GraphicSettings::EnableBlending );
	editorGuiManager->DrawCheckBox( leftMargin, checkboxOffset, "Enable GL MSAA", GraphicSettings::GetIsMSAAEnabled(), GraphicSettings::EnableMSAA );
	editorGuiManager->DrawCheckBox( leftMargin, checkboxOffset, "Enable Gamma Correction", GraphicSettings::GetIsGammaCorrectionEnabled(), GraphicSettings::EnableGammaCorrection );
	editorGuiManager->DrawCheckBox( leftMargin, checkboxOffset, "Enable sRBG (Hardware) (*Beta)", GraphicSettings::GetIssRGBEnabled(), GraphicSettings::EnablesRGB );

	ImGui::Dummy( ySpace );
	editorGuiManager->DrawComboDrop( leftMargin, checkboxOffset, "Post-processing", GraphicSettings::GetPostProcessName(), GraphicSettings::GetPostProcessesList(), GraphicSettings::SetCurrentSelectedPostProcessName );

	ImGui::Dummy( ySpace );
	list<string> cubeNamesList = MCubemap::GetAllCubemapNamesList();
	cubeNamesList.push_front( "Clear Color" );
	editorGuiManager->DrawComboDrop( leftMargin, checkboxOffset, "Skybox Cubemap", GraphicSettings::GetSkyboxCubemapName(), cubeNamesList, GraphicSettings::SetSkyboxCubemap );
}