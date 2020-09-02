#define _CRT_SECURE_NO_WARNINGS
#include "GameObject.h"
#include "Transform.h"
#include "SceneManager.h"
#include "Camera.h"
using namespace MANA3D;

using std::map;
using std::list;
using std::vector;
using std::string;


GameObject::GameObject( const MString& name ) : 
	name( name ),
	isActive( true ),
	tag( "" )
{
	// Attach transform component.
	transform = AddComponent<Transform>();	
}

GameObject::~GameObject()
{
	DestroyAllComponents();
}


void GameObject::Update()
{
	for ( map<string, Component*>::iterator it = components.begin(); it != components.end(); ++it )
	{
		if ( it->second != nullptr && it->second->GetEnable() )
			it->second->Update();
	}
}


void GameObject::SetActive( const bool& active )
{
	isActive = active;
}

const bool& GameObject::GetActive()
{
	return isActive;
}


MString GetTypeName( GameObjectType type )
{
	switch ( type )
	{
	case GameObjectType::GOT_NORMAL:
		return "GO_NORMAL";
	case GameObjectType::GOT_NORMAL_SELECTED:
		return "GO_NORMAL_SELECTED";
	case GameObjectType::GOT_PREFAB:
		return "GO_PREFAB";
	case GameObjectType::GOT_OUTLINE:
		return "GO_OUTLINE";
	case GameObjectType::GOT_HIDDEN:
		return "GO_HIDDEN";
	default:
		return "";
	}

	return "";
}

void GameObject::SetType( GameObjectType type, const bool& setChildrenAsWell )
{
	this->type = type;

	if ( setChildrenAsWell )
	{
		size_t childCount = transform->GetChildCount();
		for ( size_t i = 0; i < childCount; i++ )
			transform->GetChild( i )->GetGameObject()->SetType( type, true );
	}
}

const GameObjectType& GameObject::GetType()
{
	return type;
}


void GameObject::SetTag( const MString& tag )
{
	SceneManager::RegisterGameObjectTag( this, tag );
}

const MString& GameObject::GetTag()
{
	return tag;
}


void GameObject::SetAssociatedLight( Light* light )
{
	this->associatedLight = light;
}
Light* GameObject::GetAssociatedLight()
{
	return associatedLight;
}

void GameObject::SetAssociatedCamera( Camera* cam )
{
	this->associatedCamera = cam;
}
Camera* GameObject::GetAssociatedCamera()
{
	return associatedCamera;
}


map<string, Component*>& GameObject::GetAllComponents()
{
	return components;
}


void GameObject::DestroyAllComponents()
{
	for ( map<string, Component*>::iterator it = components.begin(); it != components.end(); ++it )
	{
		if ( it->second != nullptr )
		{
			delete it->second;
			it->second = nullptr;
		}
	}

	components.clear();

	transform = nullptr;
}


void GameObject::AddComponent( const string& name, Component* com )
{
	com->gameObject = this;
	com->transform = transform;
	components.insert( std::pair<string, Component*>( name, com ) );
}


template <class T>
void GameObject::RemoveComponent()
{
	map<string, Component*>::iterator it = components.find( typeid(T).name() );
	if ( it->second != nullptr )
	{
		if ( it->second == transform )
			return;

		delete it->second;
		it->second = nullptr;
	}
	components.erase( typeid(T).name() );
}
