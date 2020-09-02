#ifndef MANA3D_COMPONENT_H
#define MANA3D_COMPONENT_H

#include "GameObject.h"

namespace MANA3D
{
	class Component
	{
		friend class GameObject;

	public:
		void SetEnable( bool );
		const bool& GetEnable();

		GameObject* GetGameObject();

	protected:
		Component();						// Default Constructor.
		virtual ~Component();				// Default Destructor.

		virtual void Update();				// Called every frame.

		virtual void OnEnable();
		virtual void OnDisable();

		GameObject* gameObject;				// The game object this component is attached to.
		Transform* transform;

		virtual void CopyFrom( Component* compoment ) = 0;

	private:
		bool isEnable;
	};
}

#endif
