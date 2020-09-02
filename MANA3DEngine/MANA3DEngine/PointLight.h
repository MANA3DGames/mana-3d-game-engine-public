#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Light.h"

namespace MANA3D
{
	class PointLight : public Light
	{
		friend class SceneManager;

	protected:
		PointLight( Transform* transform, const float& ambient, const float& diffuse, const float& specular, const float& constant, const double& linear, const double& quadratic );
		virtual ~PointLight();
	};
}

#endif // !POINT_LIGHT_H

