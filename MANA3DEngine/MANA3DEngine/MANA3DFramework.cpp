#include "MANA3DFramework.h"
#include "Application.h"
#include "Input.h"
#include "SkinnedMeshRenderer.h"
#include "MRectangle.h"
#include "MCube.h"
#include "Transform.h"
#include "MRectangle.h"
#include "Editor3DCamera.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Model3DImporter.h"
#include "Texture2DImporter.h"
#include "Screen.h"
#include "MCubemap.h"
#include "GraphicSettings.h"
#include "Time.h"
#include "SceneManager.h"
#include "FPSCameraController.h"
#include "Camera.h"
using namespace MANA3D;
using namespace glm;


Transform* nano;

Transform* cameraTrans;
SpotLight* spotLight;





void CreateCube()
{
	// Create a cube object.
	MCube* cube = MCube::Create();
	cube->GetGameObject()->name = "cube";
	cube->GetGameObject()->transform->TranslateZ( -3.0f );

	// Load and create a texture
	//MTexture* texture1 = Texture2DImporter::Import( MString( "%sTextures/container2.png", Application::GetAppDirctory().GetCStr() ) );
	MTexture* texture1 = MTexture::Get( "container2.png" );
	//MTexture* texture2 = Texture2DImporter::Import( MString( "%sTextures/container2_specular.png", Application::GetAppDirctory().GetCStr() ) );
	MTexture* texture2 = MTexture::Get( "container2_specular.png" );

	MMaterial* specular = MMaterial::Create( MShader::Get( "Specular" ), "mySpecularMat" );
	specular->SetTexture( "diffuseMap", texture1 );
	specular->SetTexture( "specularMap", texture2 );

	// Material properties
	MeshRenderer* cMeshRenderer = cube->GetGameObject()->GetComponent<MeshRenderer>();
	cMeshRenderer->SetMaterial( specular );
}
void CreateGrass( vec3 pos )
{
	MRectangle* rect = MRectangle::Create();
	rect->GetGameObject()->name = "Grass";
	rect->GetGameObject()->transform->SetLocalPosition( pos );
	rect->GetGameObject()->transform->Rotate( 0.0f, 180.0f, 0.0f );

	//MTexture* texture1 = Texture2DImporter::Import( MString( "%sTextures/grass.png", Application::GetAppDirctory().GetCStr() ) );
	MTexture* texture1 = MTexture::Get( "grass.png" );

	if ( texture1 == nullptr )
	{
		Debug::LogError( "Couldn't find the target grass image" );
		return;
	}
	
	MMaterial* cutoutMat = MMaterial::Create( MShader::Get( "TransparentCutout" ), "myTransparentCutoutMat" );
	//cutoutMat->SetTexture( "diffuseMap", texture1, 0 );
	cutoutMat->SetTexture( "diffuseMap", texture1 );

	MeshRenderer* cMeshRenderer = rect->GetGameObject()->GetComponent<MeshRenderer>();
	cMeshRenderer->SetMaterial( cutoutMat );
}
void CreateTWindow( vec3 pos )
{
	MRectangle* rect = MRectangle::Create();
	rect->GetGameObject()->transform->SetLocalPosition( pos );
	rect->GetGameObject()->transform->Rotate( 0.0f, 180.0f, 0.0f );
	//MTexture* texture1 = Texture2DImporter::Import( MString( "%sTextures/window.png", Application::GetAppDirctory().GetCStr() ) );
	MTexture* texture1 = MTexture::Get( "window.png" );
	MMaterial* cutoutMat = MMaterial::Create( MShader::Get( "Transparent" ), "myTransparentMat" );
	//cutoutMat->SetTexture( "diffuseMap", texture1, 0 );
	cutoutMat->SetTexture( "diffuseMap", texture1 );
	MeshRenderer* cMeshRenderer = rect->GetGameObject()->GetComponent<MeshRenderer>();
	cMeshRenderer->SetMaterial( cutoutMat );
}
void PrintChildName( GameObject* go, int offset )
{
	/*for ( size_t i = 0; i < offset; i++ )
	{
		cout << "\t";
	}
	cout << go->name << endl;*/

	//go->transform->SetLocalPosition( vec3( 0, -10, -10 ) );

	list<Transform*> children = go->transform->GetChildren();
	int newoff = offset + 1;
	for ( list<Transform*>::iterator it = children.begin(); it != children.end(); ++it )
	{
		PrintChildName( (*it)->GetGameObject(), newoff );
	}
}
GameObject* Load_Nanosuit()
{
	//GameObject* model = Model3DImporter::Import( MString( "%sModels/nanosuit/nanosuit.obj", Application::GetAppDirctory().GetCStr() ) );
	GameObject* model = SceneManager::GetPrefab( "nanosuit.obj" );
	GameObject* myNano = SceneManager::AddGameObject( "MyNano", model );
	myNano->transform->SetLocalScale( Math3D::VECTOR3_ONE * 0.1f );
	return myNano;
}
void ImportModelShanna()
{
	//GameObject* prefab = Model3DImporter::Import( MString( "%sModels/Shanna_All_Anims/Shanna_All_Anims.fbx", Application::GetAppDirctory().GetCStr() ) );

	//MTexture* headTexture = Texture2DImporter::Import( MString( "%sModels/Shanna_All_Anims/HeadTexTest.psd", Application::GetAppDirctory().GetCStr() ) );
	//MTexture* bodyTexture = Texture2DImporter::Import( MString( "%sModels/Shanna_All_Anims/BodyTexTest.psd", Application::GetAppDirctory().GetCStr() ) );

	//prefab->transform->GetChild( 0 )->GetGameObject()->GetComponent<SkinnedMeshRenderer>()->SetMaterialTexture( "diffuseMap", headTexture, 0 );
	//prefab->transform->GetChild( 1 )->GetGameObject()->GetComponent<SkinnedMeshRenderer>()->SetMaterialTexture( "diffuseMap", bodyTexture, 0 );
	//prefab->transform->GetChild( 2 )->GetGameObject()->GetComponent<SkinnedMeshRenderer>()->SetMaterial( MMaterial::GetReflection() );


	//GameObject* go = GameObject::Clone( prefab );
	////go->transform->SetLocalScale( Math3D::VECTOR3_ONE * 0.05f );
	//go->transform->SetLocalPosition( Math3D::VECTOR3_BACKWARD * 300.0f );

	//gAllGOs.push_back( prefab );
	//gAllGOs.push_back( go );
}



void CreateSpotLight()
{
	GameObject* go = SceneManager::AddGameObject( "" );
	spotLight = SceneManager::AddSpotLight(
		go->transform,
		0.1f, 1.0f, 1.0f, 1.0f, 0.09, 0.032, 
		glm::cos( glm::radians( 12.5f ) ), 
		glm::cos( glm::radians( 15.0f ) ) );
}


void CreateFPSCamera()
{
	//GameObject* camGO = SceneManager::Find( "DefaultCamera" );
	GameObject* camGO = SceneManager::FindWithTag( "MainCamera" );

	if ( camGO != nullptr )
	{
		Camera* camera = camGO->GetAssociatedCamera();
		if ( camera != nullptr )
		{
			cameraTrans = camGO->transform;
			FPSCameraController* fpsCam = camGO->AddComponent<FPSCameraController>();
			fpsCam->SetCamera( camera );
		}
	}
}








void MANA3DFramework::OnStart()
{
	CreateFPSCamera();

	/*
	CreateCube();
	

	CreateTWindow( Math3D::VECTOR3_ZERO );
	
	CreateTWindow( vec3( -2, 0, 4 ) );
	CreateTWindow( vec3( -1, 0, 3 ) );
	*/

	//CreateGrass( vec3( 1, 0, 4 ) );

	//GraphicSettings::SetIsBlendingEnabled( true );

	//nano = Load_Nanosuit()->transform;

	//CreateSpotLight();

	GameObject* go = SceneManager::Find( "nanosuit.obj" );
	if ( go != nullptr )
	{
		nano = go->transform->FindChild( "Helmet" );
		if ( nano != nullptr )
			Debug::Log( "Found helmet" );
	}
}

void MANA3DFramework::OnUpdate()
{
	if ( nano != nullptr )
		nano->RotateYLocally( 10.0f * Time::GetDeltaTime() );



	if ( cameraTrans != nullptr && spotLight != nullptr )
	{
		// Update spotLight position & direction according to main camera.
		spotLight->GetTransform()->SetLocalPosition( cameraTrans->GetLocalPosition() );
		spotLight->GetTransform()->SetLocalRotation( cameraTrans->GetLocalRotation() );
	}


	if ( Input::GetKeyPressed( KeyCode::KC_F ) )
		Screen::SetFullScreen( false );

	if ( Input::GetKeyPressed( KeyCode::KC_Delete ) )
	{
		GameObject* go = SceneManager::Find( "nanosuit.obj" );
		if ( go != nullptr )
		{
			SceneManager::DeleteGameObject( go );
		}
	}
}

void MANA3DFramework::OnTerminate()
{
}