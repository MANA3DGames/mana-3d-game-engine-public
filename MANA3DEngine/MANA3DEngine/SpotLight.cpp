#include "SpotLight.h"
using namespace MANA3D;


SpotLight::SpotLight(	Transform* transform,
						const float& ambient, const float& diffuse, const float& specular,
						const float& constant,
						const double& linear, const double& quadratic,
						const float& cutOff, const float& outerCutOff )
	: Light( transform, LightType::LT_Spot_Light, ambient, diffuse, specular )
{
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
	this->cutOff = cutOff;
	this->outerCutOff = outerCutOff;
}

SpotLight::~SpotLight()
{
}


void SpotLight::SetCutOff( float val )
{
	cutOff = val;
}
float SpotLight::GetCutOff()
{
	return cutOff;
}

void SpotLight::SetOuterCutOff( float val )
{
	outerCutOff = val;
}
float SpotLight::GetOuterCutOff()
{
	return outerCutOff;
}




void SpotLight::CopyPropertiesFrom( Light* source )
{
	Light::CopyPropertiesFrom( source );
	SpotLight* spotLight = (SpotLight*)source;
	this->cutOff = spotLight->cutOff;
	this->outerCutOff = spotLight->outerCutOff;
}
