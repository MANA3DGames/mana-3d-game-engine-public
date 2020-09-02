#ifndef CAMERA_H
#define CAMERA_H

#include "Transform.h"
#include "Color.h"


namespace MANA3D
{
	class GameObject;

	class Camera
	{
		friend class MANA3DEngine;
		friend class SceneManager;

	private:
		Camera( Transform* transform );
		virtual ~Camera();

	private:
		GameObject* gameObject;
		Transform* transform;

		float fieldOfView;

		float nearPlane;
		float farPlane;

		Color bgColor;

		int viewWidth;
		int viewHeight;

	public:
		Transform* GetTransform();

		// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectionMatrix();

		void SetFieldOfView(float);
		float GetFieldOfView();
		
		void SetNearPlane( float val );
		float GetNearPlane();

		void SetFarPlane( float val );
		float GetFarPlane();

		void SetBGColor( Color bgColor );
		Color GetBGColor();


		void CopyPropertiesFrom( Camera* source );




	public: // Static
		static void SetMainCamera( Camera* cam );
		static void SetViewPortSize( int width, int height );

		static Camera* GetCurrentMainCamera();


	private: // Static 
		static Camera* mainCamera;

		static void RenderFromEditorCamera();
		static void RenderFromRuntimeCamera();
		static void RenderFromMainCamera( GameObject* toBeRendered );

		static void Render( void ( *RenderFunc )(), void ( *BlendingRenderFunc )() );

		static void ConfigGLForSelectedObjectPhase1();
		static void ConfigGLForSelectedObjectPhase2( GameObject* selectedGO, GameObject* selectedOutline );
	};
}

#endif // !CAMERA_H