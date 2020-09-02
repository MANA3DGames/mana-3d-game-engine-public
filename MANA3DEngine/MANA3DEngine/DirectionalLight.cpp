#include "DirectionalLight.h"
using namespace MANA3D;

DirectionalLight::DirectionalLight( Transform* transform, const float& ambient, const float& diffuse, const float& specular )
	: Light( transform, LightType::LT_Directional_Light, ambient, diffuse, specular )
{
}

DirectionalLight::~DirectionalLight()
{
}