#include "PointLight.h"
using namespace MANA3D;


PointLight::PointLight( Transform* transform, const float& ambient, const float& diffuse, const float& specular, const float& constant, const double& linear, const double& quadratic )
	: Light( transform, LightType::LT_Point_Light, ambient, diffuse, specular )
{
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
}

PointLight::~PointLight()
{
}