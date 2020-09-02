#include "Component.h"
using namespace MANA3D;

Component::Component() : isEnable( true ), transform( nullptr ), gameObject( nullptr )
{
}

Component::~Component()
{

}

void Component::Update()
{
	
}


void Component::SetEnable( bool enable )
{
	isEnable = enable;
	if ( isEnable )
		OnEnable();
	else
		OnDisable();
}

const bool& Component::GetEnable()
{
	return isEnable;
}

void Component::OnEnable()
{

}

void Component::OnDisable()
{

}

GameObject* Component::GetGameObject()
{
	return gameObject;
}
