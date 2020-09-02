#ifndef FPS_CAMERA_CONTROLLER_H
#define FPS_CAMERA_CONTROLLER_H

#include "Component.h"
#include "CameraEnum.h"


namespace MANA3D
{
	class Camera;

	class FPSCameraController : public Component
	{
		friend class GameObject;

	private:
		// Euler Angles
		float yaw;
		float pitch;

		// Camera options
		float movementSpeed;
		float mouseSensitivity;

		float lastX;
		float lastY;
		bool firstMouse;

		Camera* camera;


		// Calculates the front vector from the Camera's (updated) Euler Angles
		void updateCameraVectors();


	protected:
		FPSCameraController();
		virtual ~FPSCameraController();

		void Update() override;

		void CopyFrom( Component* ) override;


		// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
		void ProcessKeyboard( Camera_Movement direction, float deltaTime );

		// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
		void ProcessMouseMovement( float xoffset, float yoffset, bool constrainPitch = true );

		// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
		void ProcessMouseScroll( float yoffset );


	public:
		void SetCamera( Camera* camera );
		
	};
}
#endif

#pragma once
