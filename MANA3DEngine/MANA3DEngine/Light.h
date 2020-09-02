#ifndef LIGHT_H
#define LIGHT_H

#include "Transform.h"

namespace MANA3D
{
	enum class LightType
	{
		LT_Point_Light,
		LT_Spot_Light,
		LT_Directional_Light
	};

	class Light 
	{
		friend class GameObject;

	protected:
		Light( Transform* transform, LightType type, const float& ambient, const float& diffuse, const float& specular );
		virtual ~Light();


		LightType type;

		GameObject* gameObject;
		Transform* transform;

		float ambient;
		float diffuse;
		float specular;

		float constant;

		double linear;
		double quadratic;

	public:
		GameObject* GetGameObject();
		Transform* GetTransform();	
		LightType GetType();

		void SetAmbient( const float& val );
		const float& GetAmbient();

		void SetDiffuse( const float& val );
		const float& GetDiffuse();

		void SetSpecular( const float& val );
		const float& GetSpecular();


		void SetConstant( const float& val );
		const float& GetConstant();
		
		void SetLinear( const double& val );
		const double& GetLinear();
		
		void SetQuadratic( const double& val );
		const double& GetQuadratic();
		


		virtual void CopyPropertiesFrom( Light* source );

	};
}

#endif // !LIGHT_H
