#include "EditorGuiInspectorWindow.h"
#include "EditorGuiManager.h"
#include "MMaterial.h"
#include "MTexture.h"
#include "Screen.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "SkinnedMeshRenderer.h"
#include "MSkinnedMesh.h"
#include "SpotLight.h"
using namespace MANA3D;
using namespace glm;
using std::map;
using std::string;


EditorGuiInspectorWindow::EditorGuiInspectorWindow( EditorGuiManager* editorGuiManager ) : EditorGuiWindow( editorGuiManager )
{
}

EditorGuiInspectorWindow::~EditorGuiInspectorWindow() {}


void EditorGuiInspectorWindow::Display( map<string, MTexture*>& privateTexturesMap )
{
	bool isSceneOpened = true;

	ImGui::Begin( "InspectorView", &isSceneOpened, ImVec2( 0 , 0 ), 1.0f,
					ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_NoMove | 
					ImGuiWindowFlags_NoTitleBar );

	ImVec2 sceneWindSize = editorGuiManager->GetSceneWindowSize();
	ImVec2 sceneHierarchyWindSize = editorGuiManager->GetSceneHierarchyWindowSize();
	ImGui::SetWindowPos( ImVec2( sceneHierarchyWindSize.x + sceneWindSize.x, editorGuiManager->GetSceneWindowTopOffsetY() ) );
	ImVec2 windowSize = ImVec2( Screen::GetWidth() - sceneHierarchyWindSize.x - sceneWindSize.x, sceneWindSize.y );
	ImGui::SetWindowSize( windowSize );

	UpdateWindProperties();

	ImGui::BeginTabBar( "InspectorView#left_tabs_bar" );

	if ( ImGui::BeginTabItem( "Inspector" ) )
	{
		ImGui::BeginChild( "Inspector_MainChildWindow", ImVec2( windowSize.x - 30, windowSize.y - 50 ) );

		// Check if we have a gameObject selected in the scene hierarchy.
		if ( editorGuiManager->GetCurrentSelectedGameObjectInScene() != nullptr )
		{
			ImGui::Spacing();
			ImGui::SameLine();

			DisplayInspectorWindowActiveCheckbox();

			ImGui::SameLine();

			DisplayInspectorWindowGameObjectName();
			
			ImGui::Separator();

			DisplayInspectorWindowGameObjectTag( privateTexturesMap );

			ImGui::Separator();

			if ( editorGuiManager->GetCurrentSelectedGameObjectInScene()->GetAssociatedCamera() != nullptr )
				DisplayInspectorWindowCameraInstance();
			else if ( editorGuiManager->GetCurrentSelectedGameObjectInScene()->GetAssociatedLight() != nullptr )
				DisplayInspectorWindowLightInstance();

			ImGui::Dummy( ImVec2( 0, 10 ) );
			ImGui::Separator();
			ImGui::Spacing();

			DisplayInspectorWindowTransfrom();
			DisplayInspectorWindowComponents();

			ImGui::Spacing();
			ImGui::Separator();
		}

		ImGui::EndChild();

		ImGui::EndTabItem();
	}

	ImGui::EndTabBar();

	ImGui::End();
}

void EditorGuiInspectorWindow::DisplayInspectorWindowActiveCheckbox()
{
	bool isActive = editorGuiManager->GetIsCurrentSelectedGameObjectActive();
	ImGui::Checkbox( "##hidelabel_IsActiveCheckBox", &isActive );
	editorGuiManager->GetCurrentSelectedGameObjectInScene()->SetActive( isActive );
}

void EditorGuiInspectorWindow::DisplayInspectorWindowGameObjectName()
{
	char* nameStrBuf = editorGuiManager->GetCurrentSelectedGameObjectInScene()->name.GetCharArray();
	ImGui::InputText( "##hidelabel_currentSelectedGameObjectName", nameStrBuf, sizeof( MString ) * 2 );

	if ( editorGuiManager->GetCurrentSelectedGameObjectInScene()->name.GetLength() > 50 )
		editorGuiManager->GetCurrentSelectedGameObjectInScene()->name = editorGuiManager->GetCurrentSelectedGameObjectInScene()->name.GetStr().substr( 0, 50 );
}

void EditorGuiInspectorWindow::DisplayInspectorWindowGameObjectTag( map<string, MTexture*>& privateTexturesMap )
{
	editorGuiManager->DrawComboDrop(
		ImVec2( 30.0f, 0.0f ), ImVec2( 0.0f, 0.0f ), 200.0f,
		"Tag", 
		editorGuiManager->GetCurrentSelectedGameObjectInScene()->GetTag(), 
		SceneManager::GetTagList(), 
		editorGuiManager->GetCurrentSelectedGameObjectInScene()->GetTag(),
		editorGuiManager->GetCurrentSelectedGameObjectInScene(), &SceneManager::SetGameObjectTagFromEditor );
	
	ImGui::SameLine();
	static char strBuf[128] = "";
	ImGui::PushItemWidth( 100.0f );
	ImGui::InputText( "##hidelabel_currentSelectedGameObjectAddNewTag", strBuf, sizeof( MString ) * 2 );
	ImGui::PopItemWidth();


	ImVec2 btnSize = ImVec2( 20, 20 );
	ImVec2 uv0 = ImVec2( 0, 0 );
	ImVec2 uv1 = ImVec2( 1, 1 );
	ImVec4 bgColor = editorGuiManager->GetImVec4( Color::Black );

	ImGui::SameLine();
	if ( ImGui::ImageButton( (void*)(intptr_t)privateTexturesMap.find( "Add.png" )->second->GetID(), btnSize, uv0, uv1, -1, bgColor ) )
	{
		MString newTag( strBuf );
		newTag.Trim();
		if ( newTag.GetLength() > 0 )
		{
			// Add the new tag
			SceneManager::AddNewTag( newTag );

			// Clear input field.
			memset( strBuf, 0, sizeof( strBuf ) );
		}
	}
	if ( ImGui::IsItemHovered() )
		ImGui::SetTooltip( "Add New Tag" );

	ImGui::SameLine();
	if ( ImGui::ImageButton( (void*)(intptr_t)privateTexturesMap.find( "Settings.png" )->second->GetID(), btnSize, uv0, uv1, -1, bgColor ) )
	{

	}
	if ( ImGui::IsItemHovered() )
		ImGui::SetTooltip( "Tag Settings" );
}

void EditorGuiInspectorWindow::DisplayInspectorWindowTransfrom()
{
	Transform* transform = editorGuiManager->GetCurrentSelectedGameObjectInScene()->transform;
	//ImGui::SetNextTreeNodeOpen( true );

	editorGuiManager->StartDisableElement();
	DisplayInspectorWindowComponentActiveCheckbox( "Transform", transform, true );
	editorGuiManager->EndDisableElement();

	ImGui::SameLine();

	if ( ImGui::CollapsingHeader( "Transform", ImGuiTreeNodeFlags_DefaultOpen ) )
	{
		DisplayInspectorWindowTransfromElement( transform->GetPosition(), "Position" );
		DisplayInspectorWindowTransfromElement( transform->GetEulerAngles(), "Rotation" );
		DisplayInspectorWindowTransfromElement( transform->GetScale(), "Scale   " );
	}
}

void EditorGuiInspectorWindow::DisplayInspectorWindowTransfromElement( vec3 elementVec, const MString& elementID )
{
	ImGui::PushItemWidth( 100 );

	ImGui::Spacing();
	ImGui::Dummy( ImVec2( 10, 0 ) );
	ImGui::SameLine();
	ImGui::Text( elementID );
	ImGui::SameLine();
	ImGui::Dummy( ImVec2( 30, 10 ) );

	char* components[] = { "X", "Y", "Z" };

	for ( size_t i = 0; i < 3; i++ )
	{
		ImGui::SameLine();
		ImGui::Text( components[i] );
		ImGui::SameLine();

		MString id( "##hidelabel_currentSelectedGameObjectTransform%s%s", elementID.GetCStr(), components[i] );

		if ( ImGui::InputFloat( id, &elementVec[i], 0.0f, 0.0f, "%3f", ImGuiInputTextFlags_EnterReturnsTrue ) )
		{
			if ( elementID == "Position" )
				editorGuiManager->GetCurrentSelectedGameObjectInScene()->transform->SetLocalPosition( elementVec );
			else if ( elementID == "Rotation" )
				editorGuiManager->GetCurrentSelectedGameObjectInScene()->transform->SetLocalEulerAngles( elementVec );
			else if ( elementID == "Scale   " )
				editorGuiManager->GetCurrentSelectedGameObjectInScene()->transform->SetLocalScale( elementVec );
		}
	}

	ImGui::PopItemWidth();
}

void EditorGuiInspectorWindow::DisplayInspectorWindowComponents()
{
	map<string, Component*>& allComponents = editorGuiManager->GetCurrentSelectedGameObjectInScene()->GetAllComponents();
	for ( map<string, Component*>::iterator it = allComponents.begin(); it != allComponents.end(); ++it )
	{
		// Get string after "class MANA3D::"
		MString typeName = it->first.substr( 14, it->first.length() - 14 );

		if ( typeName == "Transform" )
			continue;

		if ( editorGuiManager->GetCurrentSelectedGameObjectInScene()->GetAssociatedCamera() != nullptr ||
			 editorGuiManager->GetCurrentSelectedGameObjectInScene()->GetAssociatedLight() != nullptr )
			continue;

		DisplayInspectorWindowComponentActiveCheckbox( typeName, it->second, it->second->GetEnable() );
		ImGui::SameLine();

		if ( ImGui::CollapsingHeader( typeName, ImGuiTreeNodeFlags_DefaultOpen ) )
		{
			if ( typeName == "MeshRenderer" )
			{
				DisplayInspectorWindowMeshRenderer( it->second );
				continue;
			}
			else if ( typeName == "SkinnedMeshRenderer" )
			{
				DisplayInspectorWindowSkinnedMeshRenderer( it->second );
				continue;
			}
		}
	}
}

void EditorGuiInspectorWindow::DisplayInspectorWindowComponentActiveCheckbox( const MString& id, Component* component, bool isEnabled )
{
	MString finalID( "##hidelabel_ComponentIsActiveCheckbox%s", id.GetCStr() );
	ImGui::Checkbox( finalID, &isEnabled );
	component->SetEnable( isEnabled );
}


void EditorGuiInspectorWindow::DisplayInspectorWindowMeshRenderer( Component* component )
{
	MeshRenderer* meshRenderer = (MeshRenderer*)component;
	MMesh* mesh = meshRenderer->GetMesh();

	// Mesh
	if ( mesh != nullptr )
	{
		editorGuiManager->DrawComboDrop(
			ImVec2( 10.0f, 0.0f ), ImVec2( 60, 0.0f ), 200.0f,
			"Mesh", mesh->GetName(), MMesh::GetAllMeshNamesList(), mesh->GetName(),
			editorGuiManager->GetCurrentSelectedGameObjectInScene(), &SceneManager::SetMeshRendererMesh );

		ImRect rect = ImRect( ImGui::GetItemRectMin(), ImGui::GetItemRectMax() );
		editorGuiManager->MakeDragDropTargetMesh( rect, meshRenderer );
	}
	else
	{
		editorGuiManager->DrawComboDrop(
			ImVec2( 10.0f, 0.0f ), ImVec2( 60, 0.0f ), 200.0f,
			"Mesh", "none", MMesh::GetAllMeshNamesList(), "none",
			editorGuiManager->GetCurrentSelectedGameObjectInScene(), &SceneManager::SetMeshRendererMesh );

		ImRect rect = ImRect( ImGui::GetItemRectMin(), ImGui::GetItemRectMax() );
		editorGuiManager->MakeDragDropTargetMesh( rect, meshRenderer );
	}

	// FaceCulling
	editorGuiManager->DrawComboDrop(
		ImVec2( 10.0f, 0.0f ), ImVec2( 4, 0.0f ), 200.0f,
		"Face Culling", meshRenderer->GetFaceCullingTypeName(), MeshRenderer::GetFaceCullingOptionNamesList(), meshRenderer->GetFaceCullingTypeName(),
		meshRenderer, &MeshRenderer::SetFaceCullingOption );

	// Material
	MMaterial* mat = meshRenderer->GetMaterial();
	if ( mat != nullptr )
	{
		editorGuiManager->DrawComboDrop(
			ImVec2( 10.0f, 0.0f ), ImVec2( 31, 0.0f ), 200.0f,
			"Material", mat->GetName(), MMaterial::GetAllMaterialNamesList(), mat->GetName(),
			editorGuiManager->GetCurrentSelectedGameObjectInScene(), &SceneManager::SetMeshRendererMaterial );

		ImRect rect = ImRect( ImGui::GetItemRectMin(), ImGui::GetItemRectMax() );
		editorGuiManager->MakeDragDropTargetMaterial( rect, meshRenderer );

		editorGuiManager->DisplayMaterialComponent( mat );
	}
	else
	{
		editorGuiManager->DrawComboDrop(
			ImVec2( 10.0f, 0.0f ), ImVec2( 31, 0.0f ), 200.0f,
			"Material", "none", MMaterial::GetAllMaterialNamesList(), "none",
			editorGuiManager->GetCurrentSelectedGameObjectInScene(), &SceneManager::SetMeshRendererMaterial );

		ImRect rect = ImRect( ImGui::GetItemRectMin(), ImGui::GetItemRectMax() );
		editorGuiManager->MakeDragDropTargetMaterial( rect, meshRenderer );
	}
}
void EditorGuiInspectorWindow::DisplayInspectorWindowSkinnedMeshRenderer( Component* component )
{
	SkinnedMeshRenderer* sMeshRenderer = (SkinnedMeshRenderer*)component;
	MSkinnedMesh* sMesh = sMeshRenderer->GetSkinnedMesh();

	//// Mesh
	//if ( mesh != nullptr )
	//{
	//	DrawComboDrop( 
	//		ImVec2( 10.0f, 0.0f ), ImVec2( 60, 0.0f ), 200.0f,
	//		"Mesh", mesh->GetName(), MMesh::GetAllMeshNamesList(), mesh->GetName(),
	//		currentSelectedGameObjectInScene, &SceneManager::SetMeshRendererMesh );
	//}
	//else
	//{
	//	DrawComboDrop( 
	//		ImVec2( 10.0f, 0.0f ), ImVec2( 60, 0.0f ), 200.0f,
	//		"Mesh", "none", MMesh::GetAllMeshNamesList(), "none",
	//		currentSelectedGameObjectInScene, &SceneManager::SetMeshRendererMesh );
	//}

	//// FaceCulling
	//DrawComboDrop( 
	//	ImVec2( 10.0f, 0.0f ), ImVec2( 4, 0.0f ), 200.0f,
	//	"Face Culling", meshRenderer->GetFaceCullingTypeName(), MeshRenderer::GetFaceCullingOptionNamesList(), meshRenderer->GetFaceCullingTypeName(),
	//	meshRenderer, &MeshRenderer::SetFaceCullingOption );

	// Material
	MMaterial* mat = sMeshRenderer->GetMaterial();
	if ( mat != nullptr )
	{
		editorGuiManager->DrawComboDrop(
			ImVec2( 10.0f, 0.0f ), ImVec2( 31, 0.0f ), 200.0f,
			"Material", mat->GetName(), MMaterial::GetAllMaterialNamesList(), mat->GetName(),
			editorGuiManager->GetCurrentSelectedGameObjectInScene(), &SceneManager::SetMeshRendererMaterial );

		editorGuiManager->DisplayMaterialComponent( mat );
	}
	else
	{
		editorGuiManager->DrawComboDrop(
			ImVec2( 10.0f, 0.0f ), ImVec2( 31, 0.0f ), 200.0f,
			"Material", "none", MMaterial::GetAllMaterialNamesList(), "none",
			editorGuiManager->GetCurrentSelectedGameObjectInScene(), &SceneManager::SetMeshRendererMaterial );
	}
}


void EditorGuiInspectorWindow::DrawSimpleSpecialInstanceIcon( const unsigned int& id, const float& leftMargin, const float& topMargin, const float& size )
{
	ImGui::Dummy( ImVec2( 0, topMargin ) );
	ImGui::Dummy( ImVec2( leftMargin, 0 ) );
	ImGui::SameLine();
	ImGui::Image( (void*)(intptr_t)id, ImVec2( size, size ) );
}
void EditorGuiInspectorWindow::DrawSimpleSpecialInstanceIcon( const float& leftMargin, const float& topMargin, const float& size )
{
	DrawSimpleSpecialInstanceIcon( ( *editorGuiManager->GetCurrentSelectedGameObjectInScene()->GetComponent<MeshRenderer>()->GetMaterial()->GetTextureIndexPairsMap().begin() ).second.tex->GetID(), leftMargin, topMargin, size );
}
void EditorGuiInspectorWindow::DisplayInspectorWindowCameraInstance()
{
	if ( ImGui::CollapsingHeader( "Camera", ImGuiTreeNodeFlags_DefaultOpen ) )
	{
		float leftMargin = 10.0f;
		float targetOffset = 100.0f;

		DrawSimpleSpecialInstanceIcon( 10, 10, 30 );

		auto camera = editorGuiManager->GetCurrentSelectedGameObjectInScene()->GetAssociatedCamera();
		editorGuiManager->AddLabelWithOffset( "Field of View", leftMargin, targetOffset );
		float fov = camera->GetFieldOfView();
		ImGui::PushItemWidth( 100.0f );
		if ( ImGui::InputFloat( "##hidelabel_currentSelectedGameObjectCameraFOV", &fov, 0.0f, 0.0f, "%3f", ImGuiInputTextFlags_EnterReturnsTrue ) )
			camera->SetFieldOfView( fov );
		ImGui::PopItemWidth();

		editorGuiManager->AddLabelWithOffset( "Near Clip Plane", leftMargin, targetOffset );
		float nearPlane = camera->GetNearPlane();
		ImGui::PushItemWidth( 100.0f );
		if ( ImGui::InputFloat( "##hidelabel_currentSelectedGameObjectCameraNearPlane", &nearPlane, 0.0f, 0.0f, "%3f", ImGuiInputTextFlags_EnterReturnsTrue ) )
			camera->SetNearPlane( nearPlane );
		ImGui::PopItemWidth();

		editorGuiManager->AddLabelWithOffset( "Far Clip Plane", leftMargin, targetOffset );
		float farPlane = camera->GetFarPlane();
		ImGui::PushItemWidth( 100.0f );
		if ( ImGui::InputFloat( "##hidelabel_currentSelectedGameObjectCameraFarPlane", &farPlane, 0.0f, 0.0f, "%3f", ImGuiInputTextFlags_EnterReturnsTrue ) )
			camera->SetFarPlane( farPlane );
		ImGui::PopItemWidth();

		ImGui::Spacing();

		editorGuiManager->AddLabelWithOffset( "BG Color", leftMargin, targetOffset );
		static ImVec4 color = editorGuiManager->GetImVec4( camera->GetBGColor() );
		ImGui::SameLine();
		int misc_flags = ImGuiColorEditFlags_HDR | 
						 ImGuiColorEditFlags_NoDragDrop | 
						 ImGuiColorEditFlags_AlphaPreviewHalf | 
						 ImGuiColorEditFlags_AlphaPreview | 
						 ImGuiColorEditFlags_NoOptions;
		if ( ImGui::ColorEdit3( "##hidenlabel_CameraInstanceBGColor", (float*)&color, misc_flags ) )
			camera->SetBGColor( editorGuiManager->GetColor( color ) );
	}
}

void EditorGuiInspectorWindow::DisplayInspectorWindowLightInstance()
{
	LightType type = editorGuiManager->GetCurrentSelectedGameObjectInScene()->GetAssociatedLight()->GetType();
	switch ( type )
	{
	case MANA3D::LightType::LT_Point_Light:
		DisplayInspectorWindowPointLightInstance();
		break;
	case MANA3D::LightType::LT_Spot_Light:
		DisplayInspectorWindowSpotLightInstance();
		break;
	case MANA3D::LightType::LT_Directional_Light:
		DisplayInspectorWindowDirectionalLightInstance();
		break;
	}
}
void EditorGuiInspectorWindow::DisplayInspectorWindowCommonLightProperties( const float& leftMargin, const float& targetOffset )
{
	auto light = editorGuiManager->GetCurrentSelectedGameObjectInScene()->GetAssociatedLight();
	editorGuiManager->AddLabelWithOffset( "Ambient", leftMargin, targetOffset );
	float ambient = light->GetAmbient();
	ImGui::PushItemWidth( 100.0f );
	if ( ImGui::InputFloat( "##hidelabel_currentSelectedGameObjectLightAmbient", &ambient, 0.0f, 0.0f, "%3f", ImGuiInputTextFlags_EnterReturnsTrue ) )
		light->SetAmbient( ambient );
	ImGui::PopItemWidth();

	editorGuiManager->AddLabelWithOffset( "Diffuse", leftMargin, targetOffset );
	float diffuse = light->GetDiffuse();
	ImGui::PushItemWidth( 100.0f );
	if ( ImGui::InputFloat( "##hidelabel_currentSelectedGameObjectLightDiffuse", &diffuse, 0.0f, 0.0f, "%3f", ImGuiInputTextFlags_EnterReturnsTrue ) )
		light->SetDiffuse( diffuse );
	ImGui::PopItemWidth();

	editorGuiManager->AddLabelWithOffset( "Specular", leftMargin, targetOffset );
	float specular = light->GetSpecular();
	ImGui::PushItemWidth( 100.0f );
	if ( ImGui::InputFloat( "##hidelabel_currentSelectedGameObjectLightSpecular", &specular, 0.0f, 0.0f, "%3f", ImGuiInputTextFlags_EnterReturnsTrue ) )
		light->SetSpecular( specular );
	ImGui::PopItemWidth();
}
void EditorGuiInspectorWindow::DisplayInspectorWindowCommonLightProperties2( const float& leftMargin, const float& targetOffset )
{
	auto light = editorGuiManager->GetCurrentSelectedGameObjectInScene()->GetAssociatedLight();
	editorGuiManager->AddLabelWithOffset( "Constant", leftMargin, targetOffset );
	float constant = light->GetConstant();
	ImGui::PushItemWidth( 100.0f );
	if ( ImGui::InputFloat( "##hidelabel_currentSelectedGameObjectLightConstant", &constant, 0.0f, 0.0f, "%3f", ImGuiInputTextFlags_EnterReturnsTrue ) )
		light->SetConstant( constant );
	ImGui::PopItemWidth();

	editorGuiManager->AddLabelWithOffset( "Linear", leftMargin, targetOffset );
	double linear = light->GetLinear();
	ImGui::PushItemWidth( 100.0f );
	if ( ImGui::InputDouble( "##hidelabel_currentSelectedGameObjectDirLightLinear", &linear, 0.0f, 0.0f, "%3f", ImGuiInputTextFlags_EnterReturnsTrue ) )
		light->SetLinear( linear );
	ImGui::PopItemWidth();

	editorGuiManager->AddLabelWithOffset( "Quadratic", leftMargin, targetOffset );
	double quadratic = light->GetQuadratic();
	ImGui::PushItemWidth( 100.0f );
	if ( ImGui::InputDouble( "##hidelabel_currentSelectedGameObjectDirLightQuadratic", &quadratic, 0.0f, 0.0f, "%3f", ImGuiInputTextFlags_EnterReturnsTrue ) )
		light->SetQuadratic( quadratic );
	ImGui::PopItemWidth();
}
void EditorGuiInspectorWindow::DisplayInspectorWindowPointLightInstance()
{
	if ( ImGui::CollapsingHeader( "Point Light", ImGuiTreeNodeFlags_DefaultOpen ) )
	{
		float leftMargin = 10.0f;
		float targetOffset = 100.0f;

		DrawSimpleSpecialInstanceIcon( 10, 10, 30 );

		DisplayInspectorWindowCommonLightProperties( leftMargin, targetOffset );
		ImGui::Dummy( ImVec2( 0, 10.0f ) );
		DisplayInspectorWindowCommonLightProperties2( leftMargin, targetOffset );
	}
}
void EditorGuiInspectorWindow::DisplayInspectorWindowSpotLightInstance()
{
	if ( ImGui::CollapsingHeader( "Spot Light", ImGuiTreeNodeFlags_DefaultOpen ) )
	{
		float leftMargin = 10.0f;
		float targetOffset = 100.0f;

		DrawSimpleSpecialInstanceIcon( 10, 10, 30 );

		DisplayInspectorWindowCommonLightProperties( leftMargin, targetOffset );
		ImGui::Dummy( ImVec2( 0, 10.0f ) );
		DisplayInspectorWindowCommonLightProperties2( leftMargin, targetOffset );
		ImGui::Dummy( ImVec2( 0, 10.0f ) );

		SpotLight* spotLight = (SpotLight*)( editorGuiManager->GetCurrentSelectedGameObjectInScene()->GetAssociatedLight() );

		editorGuiManager->AddLabelWithOffset( "Cut Off", leftMargin, targetOffset );
		float cutOff = spotLight->GetCutOff();
		ImGui::PushItemWidth( 100.0f );
		if ( ImGui::InputFloat( "##hidelabel_currentSelectedGameObjectLightCutOff", &cutOff, 0.0f, 0.0f, "%3f", ImGuiInputTextFlags_EnterReturnsTrue ) )
			spotLight->SetCutOff( cutOff );
		ImGui::PopItemWidth();

		editorGuiManager->AddLabelWithOffset( "Outer Cut Off", leftMargin, targetOffset );
		float outerCutOff = spotLight->GetOuterCutOff();
		ImGui::PushItemWidth( 100.0f );
		if ( ImGui::InputFloat( "##hidelabel_currentSelectedGameObjectLightOuterCutOff", &outerCutOff, 0.0f, 0.0f, "%3f", ImGuiInputTextFlags_EnterReturnsTrue ) )
			spotLight->SetOuterCutOff( outerCutOff );
		ImGui::PopItemWidth();
	}
}
void EditorGuiInspectorWindow::DisplayInspectorWindowDirectionalLightInstance()
{
	if ( ImGui::CollapsingHeader( "Directional Light", ImGuiTreeNodeFlags_DefaultOpen ) )
	{
		float leftMargin = 10.0f;
		float targetOffset = 100.0f;

		DrawSimpleSpecialInstanceIcon( 10, 10, 30 );

		DisplayInspectorWindowCommonLightProperties( leftMargin, targetOffset );
	}
}