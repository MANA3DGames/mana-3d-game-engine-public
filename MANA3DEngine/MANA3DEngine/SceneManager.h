#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "MString.h"

#include <map>
#include <list>
#include <vector>


namespace MANA3D
{
	class GameObject;
	class Transform;
	class Camera;
	class PointLight;
	class DirectionalLight;
	class SpotLight;
	class MMaterial;
	enum class GameObjectType;


	class SceneManager
	{
		friend class MANA3DEngine;
		friend class EditorGuiManager;
		friend class Editor3DSceneManager;

	private:
		SceneManager( MMaterial* outlineMat );
		~SceneManager();

		void CreateDefaultObjects();
		void SetMainCamera();




#pragma region
	private:

		static std::vector<GameObject*> sceneGameObjects;
		static std::vector<GameObject*> runtimeGameObjects;


		static void UpdateScene();
		static void UpdateRuntime();


		static void CloneSceneGameObjects();

		static void ClearRuntimeGameObjects();

		static void DeleteSceneGameObject( GameObject* toBeRemoved );

		static void EraseGameObjectFromGroup( GameObject* toBeRemoved, std::vector<GameObject*>& container );

	public:
		static GameObject* AddGameObject( const MString& name );
		static GameObject* AddGameObject( const MString& name, GameObject* prefab );

		static void DeleteGameObject( GameObject* toBeRemoved );

		// Finds a game object by name and returns it.
		static GameObject* Find( const MString& name );

		static GameObject* CreatePointLightGameObject();
		static GameObject* CreateSpotLightGameObject();
		static GameObject* CreateDirectionalLightGameObject();

		static std::vector<GameObject*>	GetSceneGameObjects();
		static int GetSceneGameObjectsCount();

#pragma endregion [Static GameObjects]

#pragma region
	private:
		static std::map<std::string, GameObject*> prefabs;
		static std::map<GameObject*, GameObject*> gameObjectOutlinesMap;
		
		static MMaterial* outlineMat;

		// Map for all tags.
		static std::map<std::string, std::list<GameObject*>> tagMap;
		static std::list<std::string> tagList;

		static void ConstructTagMap( std::list<std::string>& tags );
		static void ClearTagMap();


		// DON'T call this, internal use only!
		static GameObject* CloneGameObjectRecursion( GameObject* source, GameObjectType type , Transform* parent, std::vector<GameObject*>& container );
		static GameObject* CloneGameObject( GameObject* source, GameObjectType type, std::vector<GameObject*>& container );


		static GameObject* CreateGameObjectOutline( const MString& name, GameObject* selectedGO );
		static GameObject* CreateGameObjectOutlineNode( const MString& name, GameObject* go, Transform* parent );
		static void DestroyGameObjectOutline( GameObject* selectedGO );
		static GameObject* GetGameObjectOutline( GameObject* );


		static GameObject* CreateGameObject( const MString& name, GameObjectType type, std::vector<GameObject*>& container );

		static void DestroyGameObject_Internal( GameObject*, bool destroyChildren );

	public:

		// Returns one active GameObject tagged tag. Returns null if no GameObject was found.
		static GameObject* FindWithTag( const MString& tag );	
		// Returns a list of active GameObjects tagged tag.Returns empty array if no GameObject was found.
		static std::list<GameObject*> FindAllWithTag( const MString& tag );

		static GameObject* GetPrefab( const MString& name );
		static GameObject* CreatePrefabAsset( const MString& name, const bool& isRoot  );
		static GameObject* CreatePrefabAsset( GameObject* source );
		static void DestroyPrefabAsset( GameObject* prefab );

		static void RegisterGameObjectTag( GameObject* go, const MString& tag );

		static std::map<std::string, GameObject*>& GetAllPrefabs();

		static std::list<std::string> GetTagList();

		static void AddNewTag( const MString& newTag );

		static void SetGameObjectTagFromEditor( GameObject* go, MString tag );
		static void SetMeshRendererMesh( GameObject* go, MString name );
		static void SetMeshRendererMaterial( GameObject* go, MString name );
		static void SetMeshRendererMaterialShader( GameObject* go, const MString& name );

	#pragma endregion [Static GameObjects Internal]


#pragma region
	private:
		static std::vector<Camera*> sceneCameras;
		static std::vector<Camera*> runtimeCameras;

		static void DeleteSceneCamera( Camera* cam );

	public:
		static Camera* AddCamera( Transform* transform );

		static Camera* AddCamera();

		static void DeleteCamera( Camera* cam );

#pragma endregion [Static Camera]

#pragma region
	private:

		static std::vector<PointLight*> scenePointLights;
		static std::vector<PointLight*> runtimePointLights;

		// Returns all scene point lights.
		static std::vector<PointLight*> GetScenePointLights();
		// Returns all runtime point lights.
		static std::vector<PointLight*> GetRuntimePointLights();

		static void DeleteScenePointLight( PointLight* light );

	public:
		static PointLight* AddPointLight( Transform* transform, const float& ambient, const float& diffuse, const float& specular, const float& constant, const double& linear, const double& quadratic );

		// Delete point light added at runtime.
		static void DeletePointLight( PointLight* light );

		static std::vector<PointLight*> GetAllPointLights();

#pragma endregion [Static PointLights]

#pragma region
	private:

		static std::vector<DirectionalLight*> sceneDirectionalLights;
		static std::vector<DirectionalLight*> runtimeDirectionalLights;

		// Returns all scene Directional Lights.
		static std::vector<DirectionalLight*> GetSceneDirectionalLights();
		// Returns all runtime Directional Lights.
		static std::vector<DirectionalLight*> GetRuntimeDirectionalLights();

		static void DeleteSceneDirectionalLight( DirectionalLight* light );

	public:
		static DirectionalLight* AddDirectionalLight( Transform* transform, const float& ambient, const float& diffuse, const float& specular );

		// Delete directional light added at runtime.
		static void DeleteDirectionalLight( DirectionalLight* light );

		static std::vector<DirectionalLight*> GetAllDirectionalLights();

#pragma endregion [Static DirectionalLights]

#pragma region
	private:

		static std::vector<SpotLight*> sceneSpotLights;
		static std::vector<SpotLight*> runtimeSpotLights;

		// Returns all scene spot lights.
		static std::vector<SpotLight*> GetSceneSpotLights();
		// Returns all runtime spot lights.
		static std::vector<SpotLight*> GetRuntimeSpotLights();

		static void DeleteSceneSpotLight( SpotLight* light );

	public:
		static SpotLight* AddSpotLight( Transform* transform, const float& ambient, const float& diffuse, const float& specular,
			const float& constant, const double& linear, const double& quadratic, const float& cutOff, const float& outerCutOff );

		// Delete spot light added at runtime.
		static void DeleteSpotLight( SpotLight* light );

		static std::vector<SpotLight*> GetAllSpotLights();

#pragma endregion [Static SpotLights]

	};
}

#endif // !SCENE_MANAGER_H
