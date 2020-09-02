#ifndef MANA3D_GAMEOBJECT_H
#define MANA3D_GAMEOBJECT_H

#include <map>
#include <list>
#include <vector>
#include <type_traits>		// to use std::is_same
#include <typeinfo>

#include "MString.h"


namespace MANA3D
{
	enum class GameObjectType
	{
		GOT_NORMAL,
		GOT_NORMAL_SELECTED,
		GOT_PREFAB,
		GOT_OUTLINE,
		GOT_HIDDEN,
		GOT_GIZMO
	};

	// Forward decleration
	class Component;
	class Transform;
	class Light;
	class Camera;


	class GameObject
	{
		friend class MANA3DEngine;
		friend class EditorGuiManager;
		friend class Model3DImporter;
		friend class SceneManager;

	public:

		// Activates/Deactivates the GameObject.
		void SetActive( const bool& );		
		// Return the state of isActive value.
		const bool& GetActive();													

		const GameObjectType& GetType();

		// Set a tag for this GameObject.
		void SetTag( const MString& );	
		// Return the tag for this GameObject.
		const MString& GetTag();													

		// Called every frame.
		void Update();												

		void SetAssociatedLight( Light* light );
		Light* GetAssociatedLight();

		void SetAssociatedCamera( Camera* cam );
		Camera* GetAssociatedCamera();

		std::map<std::string, Component*>& GetAllComponents();


		// The name of this GameObject.
		MString name;	
		// The Transform component attached to this GameObject.	
		Transform* transform;														


	private:
		// Default Constructor.
		GameObject( const MString& );												
		// Default Destructor.
		~GameObject();												

		void DestroyAllComponents();

		void AddComponent( const std::string&, Component* );


		void SetType( GameObjectType, const bool& setChildrenAsWell );

		// Active state of this GameObject.
		bool isActive;				
		// The tag of this GameObject.
		MString tag;														

		GameObjectType type;

		// A map of attached components with this GameObject.
		std::map<std::string, Component*> components;


		// Associated Light instance (Only for light object)
		Light* associatedLight;
		// Associated Camera instance (Only for camera object)
		Camera* associatedCamera;


	#pragma region
	public:

		// Adds a component class named className to the game object.
		template <class T>
		T* AddComponent()												
		{
			if ( std::is_same<T, Component>::value || 
				 std::is_base_of<Component, T>::value )
			{
				T* t = new T;
				( (Component*)t )->gameObject = this;
				( (Component*)t )->transform = transform;
				components.insert( std::pair<std::string, Component*>( typeid(T).name(), t ) );
				return t;
			}
			else
				return nullptr;
		}

		// Returns the component of Type type if the game object has one attached, null if it doesn't.
		template <class T>
		T* GetComponent()													
		{
			std::map<std::string, Component*>::iterator it = components.find( typeid(T).name() );
			if ( it != components.end() )
				return (T*)( it->second );
			else
				return nullptr;
		}

		template <class T>
		void RemoveComponent();


	#pragma endregion [Template]

	};
}

#endif