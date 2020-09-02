#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Light.h"

namespace MANA3D
{
	class DirectionalLight : public Light
	{
		friend class SceneManager;

	protected:
		DirectionalLight( Transform* transform, const float& ambient, const float& diffuse, const float& specular );
		virtual ~DirectionalLight();
	};
}

#endif // !DIRECTIONAL_LIGHT_H

