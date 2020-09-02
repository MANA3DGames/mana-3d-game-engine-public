#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include "Light.h"

namespace MANA3D
{
	class SpotLight : public Light
	{
		friend class SceneManager;

	protected:
		SpotLight(  Transform* transform,
					const float& ambient, const float& diffuse, const float& specular,
					const float& constant,
					const double& linear, const double& quadratic,
					const float& cutOff, const float& outerCutOff );
		virtual ~SpotLight();


	private:
		float cutOff;
		float outerCutOff;

	public:
		void SetCutOff( float val );
		float GetCutOff();

		void SetOuterCutOff( float val );
		float GetOuterCutOff();


		void CopyPropertiesFrom( Light* source ) override;
	};
}

#endif // !SPOT_LIGHT_H

