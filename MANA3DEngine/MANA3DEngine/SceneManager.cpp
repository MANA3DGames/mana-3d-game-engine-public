#include "SceneManager.h"
#include "GameObject.h"
#include "Camera.h"
#include "Screen.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "MANA3DEngine.h"
#include "MeshRenderer.h"
#include "SkinnedMeshRenderer.h"
using namespace MANA3D;
using namespace glm;

#include <algorithm>

using std::map;
using std::list;
using std::vector;
using std::string;



SceneManager::SceneManager( MMaterial* outlineMat )
{
	this->outlineMat = outlineMat;
	// Create default objects.
	CreateDefaultObjects();
}

SceneManager::~SceneManager()
{
	while ( !sceneGameObjects.empty() )
	{
		GameObject *temp = *sceneGameObjects.begin();
		sceneGameObjects.erase( sceneGameObjects.begin() );
		DestroyGameObject_Internal( temp, false );
	}
	sceneGameObjects.clear();

	while ( !sceneCameras.empty() )
	{
		Camera *temp = *sceneCameras.begin();
		sceneCameras.erase( sceneCameras.begin() );
		delete temp;
	}
	sceneCameras.clear();

	while ( !scenePointLights.empty() )
	{
		PointLight *temp = *scenePointLights.begin();
		scenePointLights.erase( scenePointLights.begin() );
		delete temp;
	}
	scenePointLights.clear();

	while ( !sceneDirectionalLights.empty() )
	{
		DirectionalLight*temp = *sceneDirectionalLights.begin();
		sceneDirectionalLights.erase( sceneDirectionalLights.begin() );
		delete temp;
	}
	sceneDirectionalLights.clear();

	while ( !sceneSpotLights.empty() )
	{
		SpotLight*temp = *sceneSpotLights.begin();
		sceneSpotLights.erase( sceneSpotLights.begin() );
		delete temp;
	}
	sceneSpotLights.clear();

	while ( !gameObjectOutlinesMap.empty() )
	{
		GameObject* temp = gameObjectOutlinesMap.begin()->second;
		gameObjectOutlinesMap.erase( gameObjectOutlinesMap.begin() );
		DestroyGameObject_Internal( temp, false );
	}
	gameObjectOutlinesMap.clear();

	while ( !prefabs.empty() )
	{
		GameObject *temp = prefabs.begin()->second;
		prefabs.erase( prefabs.begin() );
		DestroyGameObject_Internal( temp, false );
	}
	prefabs.clear();


	ClearRuntimeGameObjects();
}


void SceneManager::CreateDefaultObjects()
{
	GameObject* camGO = AddGameObject( "DefaultCamera" );
	camGO->transform->SetLocalPosition( vec3( 0.0f, 1.0f, 10.0f ) );
	SceneManager::AddCamera( camGO->transform );

	GameObject* dlGo = CreateDirectionalLightGameObject();
	dlGo->transform->RotateY( 180 );
	dlGo->transform->RotateX( -40 );
}

void SceneManager::SetMainCamera()
{
	// Get first gameObject tagged as MainCamera.
	GameObject* mainCameraGO = FindWithTag( "MainCamera" );
	if ( mainCameraGO != nullptr )
	{
		Camera::SetMainCamera( mainCameraGO->GetAssociatedCamera() );
		Camera::SetViewPortSize( Screen::GetWidth(), Screen::GetHeight() );
	}
}



#pragma region

GameObject* SceneManager::CreatePointLightGameObject()
{
	GameObject* plGo = SceneManager::AddGameObject( "plGo" );
	AddPointLight( plGo->transform, 0.1f, 2.0f, 0.5f, 1.0f, 0.1f, 0.1f );
	return plGo;
}
GameObject* SceneManager::CreateSpotLightGameObject()
{
	GameObject* slGo = SceneManager::AddGameObject( "slGo" );
	AddSpotLight( slGo->transform, 0.1f, 1.0f, 1.0f, 1.0f, 0.09, 0.032, glm::cos( glm::radians( 12.5f ) ), glm::cos( glm::radians( 15.0f ) ) );
	return slGo;
}
GameObject* SceneManager::CreateDirectionalLightGameObject()
{
	GameObject* dlGo = SceneManager::AddGameObject( "dlGo" );
	AddDirectionalLight( dlGo->transform, 0.1f, 2.0f, 0.5f );
	return dlGo;
}

#pragma endregion [Create Menu - Static Functions]


#pragma region
vector<GameObject*> SceneManager::sceneGameObjects;
vector<GameObject*> SceneManager::runtimeGameObjects;


void SceneManager::UpdateScene()
{
	for ( vector<GameObject*>::iterator it = sceneGameObjects.begin(); it != sceneGameObjects.end(); ++it )
		(*it)->Update();
}
void SceneManager::UpdateRuntime()
{
	for ( vector<GameObject*>::iterator it = runtimeGameObjects.begin(); it != runtimeGameObjects.end(); ++it )
		(*it)->Update();
}


vector<GameObject*>	SceneManager::GetSceneGameObjects()
{
	return sceneGameObjects;
}

int SceneManager::GetSceneGameObjectsCount()
{
	return sceneGameObjects.size();
}


GameObject* SceneManager::AddGameObject( const MString& name )
{
	return CreateGameObject( name, GameObjectType::GOT_NORMAL, MANA3DEngine::GetIsEditorLoop() ? sceneGameObjects : runtimeGameObjects );
}
GameObject* SceneManager::AddGameObject( const MString& name, GameObject* prefab )
{
	return CloneGameObject( prefab, GameObjectType::GOT_NORMAL, MANA3DEngine::GetIsEditorLoop() ? sceneGameObjects : runtimeGameObjects );
}


void SceneManager::EraseGameObjectFromGroup( GameObject* toBeRemoved, vector<GameObject*>& container )
{
	for ( vector<GameObject*>::iterator it = container.begin(); it != container.end(); ++it )
	{
		if ( *it == toBeRemoved )
		{
			// Erase parent.
			container.erase( it );
			break;
		}
	}

	// Erase chidren.
	int childrenCount = toBeRemoved->transform->GetChildCount();
	for ( int i = 0; i < childrenCount; i++ )
		EraseGameObjectFromGroup( toBeRemoved->transform->GetChild( i )->GetGameObject(), container );
}

void SceneManager::DeleteGameObject( GameObject* toBeRemoved )
{
	EraseGameObjectFromGroup( toBeRemoved, runtimeGameObjects );
	DestroyGameObject_Internal( toBeRemoved, true );
}
void SceneManager::DeleteSceneGameObject( GameObject* toBeRemoved )
{
	EraseGameObjectFromGroup( toBeRemoved, sceneGameObjects );
	DestroyGameObject_Internal( toBeRemoved, true );
}

GameObject* SceneManager::Find( const MString& name )
{
	for ( vector<GameObject*>::iterator it = runtimeGameObjects.begin(); it != runtimeGameObjects.end(); ++it )
	{
		if ( name == (*it)->name )
			return *it;
	}

	return nullptr;
}

void SceneManager::CloneSceneGameObjects()
{
	for ( vector<GameObject*>::iterator it = sceneGameObjects.begin(); it != sceneGameObjects.end(); ++it )
	{
		// Check if this is a child object so we don't want to clone int since it was already cloned when its parent was cloned.
		if ( (*it)->transform->GetParent() != nullptr )
			continue;

		Debug::Log( MString( "Add %s", (*it)->name.GetCStr() ) );
		AddGameObject( (*it)->name, *it );
	}
}

void SceneManager::ClearRuntimeGameObjects()
{
	ClearTagMap();

	while ( !runtimeGameObjects.empty() )
	{
		GameObject *temp = *runtimeGameObjects.begin();
		runtimeGameObjects.erase( runtimeGameObjects.begin() );
		DestroyGameObject_Internal( temp, false );
	}
	runtimeGameObjects.clear();

	while ( !runtimePointLights.empty() )
	{
		PointLight *temp = *runtimePointLights.begin();
		runtimePointLights.erase( runtimePointLights.begin() );
		delete temp;
	}
	runtimePointLights.clear();

	while ( !runtimeDirectionalLights.empty() )
	{
		DirectionalLight*temp = *runtimeDirectionalLights.begin();
		runtimeDirectionalLights.erase( runtimeDirectionalLights.begin() );
		delete temp;
	}
	runtimeDirectionalLights.clear();

	while ( !runtimeSpotLights.empty() )
	{
		SpotLight *temp = *runtimeSpotLights.begin();
		runtimeSpotLights.erase( runtimeSpotLights.begin() );
		delete temp;
	}
	runtimeSpotLights.clear();

	while ( !runtimeCameras.empty() )
	{
		Camera *temp = *runtimeCameras.begin();
		runtimeCameras.erase( runtimeCameras.begin() );
		delete temp;
	}
	runtimeCameras.clear();
}

#pragma endregion [GameObjects - Static Functions]

#pragma region 

map<string, GameObject*> SceneManager::prefabs;
map<GameObject*, GameObject*> SceneManager::gameObjectOutlinesMap;
map<string, list<GameObject*>> SceneManager::tagMap;
list<string> SceneManager::tagList;
MMaterial* SceneManager::outlineMat;


void SceneManager::ConstructTagMap( list<string>& tags )
{
	for ( list<string>::iterator it = tags.begin(); it != tags.end(); ++it )
		AddNewTag( *it );
}

void SceneManager::ClearTagMap()
{
	for ( map<string, list<GameObject*>>::iterator it = tagMap.begin(); it != tagMap.end(); ++it )
		it->second.clear();
}

void SceneManager::AddNewTag( const MString& newTag )
{
	// First, check if we already have a tag with this name.
	map<string, list<GameObject*>>::iterator it = tagMap.find( newTag.GetStr() );
	if ( it != tagMap.end() )
	{
		Debug::LogWarning( "There is already a tag with this name." );

		// We already have a tag with this name so we don't proceed.
		return;
	}

	list<GameObject*> temp;
	tagMap.insert( std::pair<string, list<GameObject*>>( newTag, temp ) );
	tagList.push_back( newTag.GetStr() );
}

void SceneManager::RegisterGameObjectTag( GameObject* go, const MString& tag )
{
	if ( MANA3DEngine::GetIsEditorLoop() )
	{
		go->tag = tag;
		return;
	}

	// Remove old tag.
	if ( go->tag != "" )
	{
		map<string, list<GameObject*>>::iterator oldIt = tagMap.find( go->tag.GetCStr() );
		if ( oldIt != tagMap.end() )
		{
			oldIt->second.remove( go );
			go->tag = "";
		}
	}

	// Add new tag.
	map<string, list<GameObject*>>::iterator newIt = tagMap.find( tag.GetCStr() );
	if ( newIt != tagMap.end() )
	{
		newIt->second.push_back( go );
		go->tag = tag;
	}
	else
	{
		// Add the given tag as a new one.
		AddNewTag( tag );

		// Now tag the given gameObject as the given tag.
		RegisterGameObjectTag( go, tag );
	}
}

list<string> SceneManager::GetTagList()
{
	return tagList;
}

GameObject* SceneManager::FindWithTag( const MString& tag )
{
	map<string, list<GameObject*>>::iterator it = tagMap.find( tag.GetStr() );
	if ( it != tagMap.end() )
	{
		if ( it->second.size() > 0 )
			return it->second.front();
		else
			return nullptr;
	}
	else
		return nullptr;
}

list<GameObject*> SceneManager::FindAllWithTag( const MString& tag )
{
	map<string, list<GameObject*>>::iterator it = tagMap.find( tag.GetCStr() );
	if ( it != tagMap.end() )
		return it->second;
	else
	{
		list<GameObject*> emptyList;
		return emptyList;
	}
}



// Only for Editor
void SceneManager::SetGameObjectTagFromEditor( GameObject* go, MString tag )
{
	go->SetTag( tag );
}
// Only for Editor
void SceneManager::SetMeshRendererMesh( GameObject* go, MString name )
{
	go->GetComponent<MeshRenderer>()->SetMesh( MMesh::Get( name ) );

	// This is only for editor
	//if ( GetOutline( go ) )
	DestroyGameObjectOutline( go );
	CreateGameObjectOutline( MString( "%s_Outline", go->name ), go );
}
// Only for Editor
void SceneManager::SetMeshRendererMaterial( GameObject* go, MString name )
{
	go->GetComponent<MeshRenderer>()->SetMaterial( MMaterial::Get( name ) );
}
// Only for Editor
void SceneManager::SetMeshRendererMaterialShader( GameObject* go, const MString& name )
{
	go->GetComponent<MeshRenderer>()->SetMaterialShader( MShader::Get( name ) );
}




GameObject* SceneManager::CreateGameObject( const MString& name, GameObjectType type, vector<GameObject*>& container )
{
	GameObject* go = new GameObject( name );
	go->type = type;
	if ( type != GameObjectType::GOT_PREFAB )
		container.push_back( go );
	return go;
}

GameObject* SceneManager::CreatePrefabAsset( const MString& name, const bool& isRoot )
{
	GameObject* go = new GameObject( name );
	go->type = GameObjectType::GOT_PREFAB;

	// Add this gameObject to the static list of prefabs only if it's the root of prefab.
	if ( isRoot )
		prefabs[name.GetStr()] = go;

	return go;
}

GameObject* SceneManager::CreatePrefabAsset( GameObject* source )
{
	map<string, GameObject*>::iterator it = prefabs.find( source->name.GetStr() );
	if ( it != prefabs.end() )
	{
		Debug::LogError( MString( "There is already a prefab with the name %s ", source->name.GetCStr() ) );
		return nullptr;
	}

	vector<GameObject*> tempContainer;
	auto prefab = CloneGameObject( source, GameObjectType::GOT_PREFAB, tempContainer );
	tempContainer.clear();	// There is no need for clear since we don't add any prefab to the container.
	prefabs[prefab->name.GetStr()] = prefab;
	return prefab;
}

void SceneManager::DestroyPrefabAsset( GameObject* prefab )
{
	for ( map<string, GameObject*>::iterator it = prefabs.begin(); it != prefabs.end(); ++it )
	{
		if ( it->second == prefab )
		{
			prefabs.erase( it );
			break;
		}
	}

	DestroyGameObject_Internal( prefab, true );
}



GameObject* SceneManager::CreateGameObjectOutline( const MString& name, GameObject* selectedGO )
{
	if ( selectedGO->GetAssociatedCamera() != nullptr || selectedGO->associatedLight != nullptr )
		return nullptr;

	GameObject* outlineGO = GetGameObjectOutline( selectedGO );
	if ( outlineGO != nullptr )
		return outlineGO;

	outlineGO = CreateGameObjectOutlineNode( name, selectedGO, nullptr );

	// Add this outline object to outlinesMap
	gameObjectOutlinesMap.insert( std::pair<GameObject*, GameObject*>( selectedGO, outlineGO ) );

	return outlineGO;
}
GameObject* SceneManager::CreateGameObjectOutlineNode( const MString& name, GameObject* selectedGONode, Transform* parent )
{
	GameObject* outlineGO = new GameObject( name );
	outlineGO->type = GameObjectType::GOT_OUTLINE;

	MeshRenderer* selectedGOMeshRenderer = selectedGONode->GetComponent<MeshRenderer>();

	if ( selectedGOMeshRenderer != NULL )
	{
		// Copy mesh shape to our new outline
		MeshRenderer* meshRenderer = outlineGO->AddComponent<MeshRenderer>();
		meshRenderer->SetMesh( selectedGOMeshRenderer->GetMesh() );
		meshRenderer->SetMaterial( outlineMat );
	}

	outlineGO->transform->SetParent( parent );
	outlineGO->transform->SetLocalPosition( selectedGONode->transform->GetLocalPosition() );

	size_t childCount = selectedGONode->transform->GetChildCount();
	for ( size_t i = 0; i < childCount; i++ )
	{
		GameObject* nextNode = selectedGONode->transform->GetChild( i )->GetGameObject();
		CreateGameObjectOutlineNode( nextNode->name, nextNode, outlineGO->transform );
	}

	return outlineGO;
}

GameObject* SceneManager::CloneGameObjectRecursion( GameObject* source, GameObjectType type, Transform* parent, vector<GameObject*>& container )
{
	GameObject* clone = CreateGameObject( source->name, type, container );
	clone->SetActive( source->GetActive() );
	clone->SetTag( source->GetTag() );

	// Clone Associated Camera.
	Camera* cam = source->GetAssociatedCamera();
	if ( cam != nullptr )
	{
		Camera* camClone = SceneManager::AddCamera( clone->transform );
		camClone->CopyPropertiesFrom( cam );
	}

	// Clone Associated Light.
	Light* light = source->associatedLight;
	SpotLight* sourceSpotLight = (SpotLight*)light;
	if ( light != nullptr )
	{
		switch ( light->GetType() )
		{
		case LightType::LT_Point_Light:
			SceneManager::AddPointLight( clone->transform, light->GetAmbient(), light->GetDiffuse(), light->GetSpecular(), light->GetConstant(), light->GetLinear(), light->GetQuadratic() );
			break;
		case LightType::LT_Spot_Light:
			SceneManager::AddSpotLight( clone->transform, light->GetAmbient(), light->GetDiffuse(), light->GetSpecular(), light->GetConstant(), light->GetLinear(), light->GetQuadratic(), sourceSpotLight->GetCutOff(), sourceSpotLight->GetOuterCutOff() );
			break;
		case LightType::LT_Directional_Light:
			SceneManager::AddDirectionalLight( clone->transform, light->GetAmbient(), light->GetDiffuse(), light->GetSpecular() );
			break;
		}
	}

	for ( std::map<string, Component*>::iterator it = source->components.begin(); it != source->components.end(); ++it )
	{
		if ( typeid( Transform ).name() == it->first )
		{
			continue;
		}
		else if ( typeid( MeshRenderer ).name() == it->first )
		{
			if ( cam != nullptr || light != nullptr )
			{
				// We don't want to clone the meshRenderer for the Camera or Light as it is only for editor use.
				continue;
			}

			MeshRenderer* com = clone->AddComponent<MeshRenderer>();
			com->CopyFrom( it->second );
		}
		else if ( typeid( SkinnedMeshRenderer ).name() == it->first )
		{
			SkinnedMeshRenderer* com = clone->AddComponent<SkinnedMeshRenderer>();
			com->CopyFrom( it->second );
		}
	}

	clone->transform->SetParent( parent );
	clone->transform->CopyFrom( source->transform );
	list<Transform*> children = source->transform->GetChildren();
	for ( list<Transform*>::iterator it = children.begin(); it != children.end(); ++it )
		CloneGameObjectRecursion( (*it)->gameObject, type, clone->transform, container );

	return clone;
}

GameObject* SceneManager::CloneGameObject( GameObject* source, GameObjectType type, vector<GameObject*>& container )
{
	return CloneGameObjectRecursion( source, type, nullptr, container );
}

GameObject* SceneManager::GetPrefab( const MString& name )
{
	map<string, GameObject*>::iterator it = prefabs.find( name.GetStr() );
	if ( it != prefabs.end() )
		return it->second;
	else
		return nullptr;
}

map<string, GameObject*>& SceneManager::GetAllPrefabs()
{
	return prefabs;
}


GameObject* SceneManager::GetGameObjectOutline( GameObject* go )
{
	map<GameObject*, GameObject*>::iterator it = gameObjectOutlinesMap.find( go );
	if ( it != gameObjectOutlinesMap.end() )
		return it->second;
	else
		return nullptr;
}

void SceneManager::DestroyGameObjectOutline( GameObject* selectedGO )
{
	map<GameObject*, GameObject*>::iterator it = gameObjectOutlinesMap.find( selectedGO );
	if ( it->second != nullptr )
	{
		DestroyGameObject_Internal( it->second, true );
		it->second = nullptr;
		gameObjectOutlinesMap.erase( it );
	}
}


void SceneManager::DestroyGameObject_Internal( GameObject *gameObject, bool destroyChildren )
{
	// Check if the gameObject is not NULL.
	if ( gameObject == nullptr )
	{
		Debug::LogError( "GameObject to be destroyed is NULL" );
		return;
	}

	// Check if we want to destroy children of this gameObject or just detach them before destroying.
	if ( destroyChildren )
	{
		//gameObject->transform->RemoveChildren();

		list<Transform*>& children = gameObject->transform->GetChildren();
		while ( !children.empty() )
			DestroyGameObject_Internal( ( *children.begin() )->gameObject, true );

		children.clear();
	}
	else
		gameObject->transform->DetachChildren();

	// Check if we have an associated light instance.
	Light* associatedLight = gameObject->associatedLight;
	if ( associatedLight != nullptr )
	{
		if ( MANA3DEngine::GetIsEditorLoop() )
		{
			// Check which type of a light so we can call the proper destructor.
			switch ( associatedLight->GetType() )
			{
			case LightType::LT_Point_Light:
				SceneManager::DeleteScenePointLight( (PointLight*)associatedLight );
				break;
			case LightType::LT_Spot_Light:
				SceneManager::DeleteSceneSpotLight( (SpotLight*)associatedLight );
				break;
			case LightType::LT_Directional_Light:
				SceneManager::DeleteSceneDirectionalLight( (DirectionalLight*)associatedLight );
				break;
			}
		}
		else
		{
			// Check which type of a light so we can call the proper destructor.
			switch ( associatedLight->GetType() )
			{
			case LightType::LT_Point_Light:
				SceneManager::DeletePointLight( (PointLight*)associatedLight );
				break;
			case LightType::LT_Spot_Light:
				SceneManager::DeleteSpotLight( (SpotLight*)associatedLight );
				break;
			case LightType::LT_Directional_Light:
				SceneManager::DeleteDirectionalLight( (DirectionalLight*)associatedLight );
				break;
			}
		}
	}

	// Check if we have an associated camera instance.
	Camera* associatedCamera = gameObject->GetAssociatedCamera();
	if ( associatedCamera != nullptr )
	{
		if ( gameObject->GetType() == GameObjectType::GOT_PREFAB )
			delete associatedCamera;
		else if ( MANA3DEngine::GetIsEditorLoop() )
			// Destroy associated camera instance before delete this gameObject.
			DeleteSceneCamera( associatedCamera );
		else
			DeleteCamera( associatedCamera );
	}

	// Free memory and delete this gameObject.
	delete gameObject;
	gameObject = nullptr;
}


#pragma endregion [GameObjects - Static Functions Internal]


#pragma region 

vector<Camera*> SceneManager::sceneCameras;
vector<Camera*> SceneManager::runtimeCameras;


Camera* SceneManager::AddCamera( Transform* transform )
{
	Camera *cam = new Camera( transform );

	if ( transform->GetGameObject()->GetType() != GameObjectType::GOT_PREFAB )
	{
		if ( MANA3DEngine::GetIsEditorLoop() )
			sceneCameras.push_back( cam );
		else
			runtimeCameras.push_back( cam );
	}
	
	return cam;
}
Camera* SceneManager::AddCamera()
{
	GameObject* camGO = AddGameObject( "Camera" );
	camGO->transform->SetLocalPosition( vec3( 0.0f, 0.0f, 0.0f ) );
	return AddCamera( camGO->transform );
}

void SceneManager::DeleteSceneCamera( Camera* cam )
{
	sceneCameras.erase( std::remove( sceneCameras.begin(), sceneCameras.end(), cam ), sceneCameras.end() );
	delete cam;
}
void SceneManager::DeleteCamera( Camera* cam )
{
	runtimeCameras.erase( std::remove( runtimeCameras.begin(), runtimeCameras.end(), cam ), runtimeCameras.end() );
	delete cam;
}

#pragma endregion [Camera - Static Functions]


#pragma region

vector<PointLight*> SceneManager::scenePointLights;
vector<PointLight*> SceneManager::runtimePointLights;


PointLight* SceneManager::AddPointLight( Transform* transform, const float& ambient, const float& diffuse, const float& specular, const float& constant, const double& linear, const double& quadratic )
{
	PointLight *light = new PointLight( transform, ambient, diffuse, specular, constant, linear, quadratic );
	light->gameObject->name = "PointLight";

	if ( transform->GetGameObject()->GetType() != GameObjectType::GOT_PREFAB )
	{
		if ( MANA3DEngine::GetIsEditorLoop() )
			scenePointLights.push_back( light );
		else
			runtimePointLights.push_back( light );
	}
		
	return light;
}

void SceneManager::DeleteScenePointLight( PointLight* light )
{
	scenePointLights.erase( std::remove( scenePointLights.begin(), scenePointLights.end(), light ), scenePointLights.end() );
	delete light;
}
void SceneManager::DeletePointLight( PointLight* light )
{
	runtimePointLights.erase( std::remove( runtimePointLights.begin(), runtimePointLights.end(), light ), runtimePointLights.end() );
	delete light;
}

vector<PointLight*> SceneManager::GetScenePointLights()
{
	return scenePointLights;
}
vector<PointLight*> SceneManager::GetRuntimePointLights()
{
	return runtimePointLights;
}

vector<PointLight*> SceneManager::GetAllPointLights()
{
	return MANA3DEngine::GetIsEditorLoop() ? scenePointLights : runtimePointLights;
}

#pragma endregion [PointLights - Static Functions]

#pragma region

vector<DirectionalLight*> SceneManager::sceneDirectionalLights;
vector<DirectionalLight*> SceneManager::runtimeDirectionalLights;


DirectionalLight* SceneManager::AddDirectionalLight( Transform* transform, const float& ambient, const float& diffuse, const float& specular )
{
	DirectionalLight*light = new DirectionalLight( transform, ambient, diffuse, specular );
	light->gameObject->name = "DirectionalLight";

	if ( transform->GetGameObject()->GetType() != GameObjectType::GOT_PREFAB )
	{
		if ( MANA3DEngine::GetIsEditorLoop() )
			sceneDirectionalLights.push_back( light );
		else
			runtimeDirectionalLights.push_back( light );
	}
	
	return light;
}

void SceneManager::DeleteSceneDirectionalLight( DirectionalLight* light )
{
	sceneDirectionalLights.erase( std::remove( sceneDirectionalLights.begin(), sceneDirectionalLights.end(), light ), sceneDirectionalLights.end() );
	delete light;
}
void SceneManager::DeleteDirectionalLight( DirectionalLight* light )
{
	runtimeDirectionalLights.erase( std::remove( runtimeDirectionalLights.begin(), runtimeDirectionalLights.end(), light ), runtimeDirectionalLights.end() );
	delete light;
}

vector<DirectionalLight*> SceneManager::GetSceneDirectionalLights()
{
	return sceneDirectionalLights;
}
vector<DirectionalLight*> SceneManager::GetRuntimeDirectionalLights()
{
	return runtimeDirectionalLights;
}

vector<DirectionalLight*> SceneManager::GetAllDirectionalLights()
{
	return MANA3DEngine::GetIsEditorLoop() ? sceneDirectionalLights : runtimeDirectionalLights;
}

#pragma endregion [DirectionalLights - Static Functions]

#pragma region

vector<SpotLight*> SceneManager::sceneSpotLights;
vector<SpotLight*> SceneManager::runtimeSpotLights;


SpotLight* SceneManager::AddSpotLight( Transform* transform, const float& ambient, const float& diffuse, const float& specular,
	const float& constant, const double& linear, const double& quadratic, const float& cutOff, const float& outerCutOff )
{
	SpotLight*light = new SpotLight( transform, ambient, diffuse, specular, constant, linear, quadratic, cutOff, outerCutOff );
	light->gameObject->name = "SpotLight";

	if ( transform->GetGameObject()->GetType() != GameObjectType::GOT_PREFAB )
	{
		if ( MANA3DEngine::GetIsEditorLoop() )
			sceneSpotLights.push_back( light );
		else
			runtimeSpotLights.push_back( light );
	}
	return light;
}

void SceneManager::DeleteSceneSpotLight( SpotLight* light )
{
	sceneSpotLights.erase( std::remove( sceneSpotLights.begin(), sceneSpotLights.end(), light ), sceneSpotLights.end() );
	delete light;
}
void SceneManager::DeleteSpotLight( SpotLight* light )
{
	runtimeSpotLights.erase( std::remove( runtimeSpotLights.begin(), runtimeSpotLights.end(), light ), runtimeSpotLights.end() );
	delete light;
}

vector<SpotLight*> SceneManager::GetSceneSpotLights()
{
	return sceneSpotLights;
}
vector<SpotLight*> SceneManager::GetRuntimeSpotLights()
{
	return runtimeSpotLights;
}

vector<SpotLight*> SceneManager::GetAllSpotLights()
{
	return MANA3DEngine::GetIsEditorLoop() ? sceneSpotLights : runtimeSpotLights;
}

#pragma endregion [SpotLights - Static Functions]