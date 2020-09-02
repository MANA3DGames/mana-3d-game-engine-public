#include "Light.h"
#include "MCube.h"
#include "Math3D.h"
#include "Transform.h"
#include "SceneManager.h"
#include "MANA3DEngine.h"
using namespace MANA3D;


Light::Light( Transform* transform, LightType type, const float& ambient, const float& diffuse, const float& specular )
{
	this->type = type;

	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;

	this->constant = 0.0f;
	this->linear = 0.0;
	this->quadratic = 0.0;

	this->transform = transform;
	gameObject = transform->GetGameObject();
	gameObject->SetTag( "Light" );
	gameObject->SetAssociatedLight( this );

	MString typeName;
	switch ( type )
	{
	case MANA3D::LightType::LT_Point_Light:
		typeName = "PointLight";
		break;
	case MANA3D::LightType::LT_Spot_Light:
		typeName = "SpotLight";
		break;
	case MANA3D::LightType::LT_Directional_Light:
		typeName = "DirectionalLight";
		break;
	}
	if ( MANA3DEngine::GetIsEditorLoop() )
		MANA3DEngine::CreateGizmo( gameObject, typeName );
}

Light::~Light()
{
}


GameObject* Light::GetGameObject()
{
	return gameObject;
}

Transform* Light::GetTransform()
{
	return transform;
}

LightType Light::GetType()
{
	return type;
}


void Light::SetAmbient( const float& val )
{
	ambient = val;
}
const float& Light::GetAmbient()
{
	return ambient;
}

void Light::SetDiffuse( const float& val )
{
	diffuse = val;
}
const float& Light::GetDiffuse()
{
	return diffuse;
}

void Light::SetSpecular( const float& val )
{
	specular = val;
}
const float& Light::GetSpecular()
{
	return specular;
}



const float& Light::GetConstant()
{
	return constant;
}
void Light::SetConstant( const float& val )
{
	constant = val;
}

const double& Light::GetLinear()
{
	return linear;
}
void Light::SetLinear( const double& val )
{
	linear = val;
}

const double& Light::GetQuadratic()
{
	return quadratic;
}
void Light::SetQuadratic( const double& val )
{
	quadratic = val;
}


void Light::CopyPropertiesFrom( Light* source )
{
	this->ambient = source->ambient;
	this->diffuse = source->diffuse;
	this->specular = source->specular;

	this->constant = source->constant;
	this->linear = source->linear;
	this->quadratic = source->quadratic;
}
