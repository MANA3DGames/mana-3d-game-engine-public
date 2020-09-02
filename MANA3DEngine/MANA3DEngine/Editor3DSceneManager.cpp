#include "Editor3DSceneManager.h"
#include "EditorGuiManager.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Camera.h"
#include "Screen.h"
#include "MMesh.h"
#include "Editor3DCamera.h"
#include "Editor3DGrid.h"
using namespace MANA3D;


Editor3DSceneManager::Editor3DSceneManager( MMaterial* gridMat, MShader* gridShader, MMesh* gridMesh, MMesh* crossGridMesh ) :
	mana3DEditorGUI( nullptr )
{
	// Create default camera.
	GameObject* defaultCamGO = SceneManager::CreateGameObject( "EditorCamera", GameObjectType::GOT_HIDDEN, editorGameObjects );
	Camera* camera = SceneManager::AddCamera( defaultCamGO->transform );
	Camera::SetMainCamera( camera );
	Camera::SetViewPortSize( Screen::GetWidth(), Screen::GetHeight() );

	editorCam = defaultCamGO->AddComponent<Editor3DCamera>();
	editorCam->SetCamera( camera );
	
	GameObject* grid = SceneManager::CreateGameObject( "Editor3DGrid", GameObjectType::GOT_HIDDEN, editorGameObjects );
	GameObject* cross = SceneManager::CreateGameObject( "CrossGrid", GameObjectType::GOT_HIDDEN, editorGameObjects );
	editor3DGrid = new Editor3DGrid( grid, cross, gridMat, gridShader, gridMesh, crossGridMesh );
}
Editor3DSceneManager::~Editor3DSceneManager()
{
	delete editor3DGrid;

	while ( !editorGameObjects.empty() )
	{
		GameObject *temp = *editorGameObjects.begin();
		editorGameObjects.erase( editorGameObjects.begin() );
		SceneManager::DestroyGameObject_Internal( temp, false );
	}
	editorGameObjects.clear();
}



void Editor3DSceneManager::SetEditorGuiManager( EditorGuiManager* mana3DEditorGUI )
{
	this->mana3DEditorGUI = mana3DEditorGUI;
	editorCam->SetMANA3DEditorGUI( mana3DEditorGUI );
}


void Editor3DSceneManager::SetEditorCamAsMain()
{
	Camera::SetMainCamera( editorCam->GetCamera() );
}


void Editor3DSceneManager::SetMainGridColor()
{
	editor3DGrid->SetMainGridColor();
}

void Editor3DSceneManager::SetCrossGridColor()
{
	editor3DGrid->SetCrossGridColor();
}


GameObject* Editor3DSceneManager::GetMainGridGO()
{
	return editor3DGrid->mainGridGameObject;
}
GameObject* Editor3DSceneManager::GetCrossGridGO()
{
	return editor3DGrid->crossGameObject;
}




vector<GameObject*> Editor3DSceneManager::editorGameObjects;


void Editor3DSceneManager::Update()
{
	for ( vector<GameObject*>::iterator it = editorGameObjects.begin(); it != editorGameObjects.end(); ++it )
		(*it)->Update();
}