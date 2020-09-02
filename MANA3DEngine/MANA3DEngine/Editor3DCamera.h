#ifndef EDITOR_CAMERA_H
#define EDITOR_CAMERA_H

#include "Component.h"
#include "CameraEnum.h"

namespace MANA3D
{
	class Camera;


	class Editor3DCamera : public Component
	{
		friend class GameObject;
		friend class Editor3DSceneManager;

	private:
		// Euler Angles
		double Yaw;
		double Pitch;
		// Camera options
		float MovementSpeed;
		float MouseSensitivity;

		double lastX;
		double lastY;
		bool firstMouse;

		Camera* camera;
		EditorGuiManager* editor;
		


		// Calculates the front vector from the Camera's (updated) Euler Angles
		void updateCameraVectors();

		void SetCamera( Camera* camera );
		void SetMANA3DEditorGUI( EditorGuiManager* editor );

		Camera* GetCamera();


	protected:
		Editor3DCamera();
		virtual ~Editor3DCamera();

		void Update() override;

		void CopyFrom( Component* ) override;


		// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
		void ProcessKeyboard( Camera_Movement direction, float deltaTime );

		// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
		void ProcessMouseMovement( double xoffset, double yoffset, bool constrainPitch = true );

		// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
		void ProcessMouseScroll( double yoffset, float deltaTime );

		void ProcessMousePin( double xoffset, double yoffset, float deltaTime );
	};
}

#endif // !EDITOR_CAMERA_H
