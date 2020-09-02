#include "Editor3DCamera.h"
#include "Screen.h"
#include "Input.h"
#include "Time.h"
#include "EditorGuiManager.h"
#include "Camera.h"
#include "Component.h"
#include "SpotLight.h"
using namespace MANA3D;
using namespace glm;


// Constructor with vectors
Editor3DCamera::Editor3DCamera() :
	Component(),
	Yaw( NULL ),
	Pitch( NULL ),
	MovementSpeed( NULL ),
	MouseSensitivity( NULL ),
	lastX( NULL ),
	lastY( NULL ),
	firstMouse( NULL ),
	camera( nullptr ),
	editor( nullptr )
{
}

Editor3DCamera::~Editor3DCamera()
{
}


void Editor3DCamera::Update()
{
	double xpos = Input::GetMouseXPos();
	double ypos = Input::GetMouseYPos();

	if ( !editor->GetIsSceneWindowFocused() )
	{
		lastX = xpos;
		lastY = ypos;
		return;
	}
		

	if ( Input::GetKeyPressed( KeyCode::KC_W ) )
		ProcessKeyboard( Camera_Movement::CM_FORWARD, Time::GetDeltaTime() );
	if ( Input::GetKeyPressed( KeyCode::KC_S ) )
		ProcessKeyboard( Camera_Movement::CM_BACKWARD, Time::GetDeltaTime() );
	if ( Input::GetKeyPressed( KeyCode::KC_A ) )
		ProcessKeyboard( Camera_Movement::CM_LEFT, Time::GetDeltaTime() );
	if ( Input::GetKeyPressed( KeyCode::KC_D ) )
		ProcessKeyboard( Camera_Movement::CM_RIGHT, Time::GetDeltaTime() );

	double scrollY = Input::GetScrollY();
	if ( scrollY != 0.0 )
		ProcessMouseScroll( scrollY, Time::GetDeltaTime() );

	// Just for the first time.
	if ( firstMouse && xpos == 0 && ypos == 0 )
		return;

	if ( firstMouse )
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	if ( Input::GetMouseRightClick() )
		ProcessMouseMovement( xoffset, yoffset );

	if ( Input::GetMouseMiddleClick() || ( Input::GetKeyPressed( KeyCode::KC_LeftShift ) && Input::GetMouseLeftClick() ) )
		ProcessMousePin( xoffset, yoffset, Time::GetDeltaTime() );


	lastX = xpos;
	lastY = ypos;
}


// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Editor3DCamera::ProcessKeyboard( Camera_Movement direction, float deltaTime )
{
	float velocity = MovementSpeed * deltaTime;
	vec3 position = transform->GetLocalPosition();

	if ( direction == Camera_Movement::CM_FORWARD )
		transform->SetLocalPosition( position + transform->GetForward() * velocity );
	if ( direction == Camera_Movement::CM_BACKWARD )
		transform->SetLocalPosition( position - transform->GetForward() * velocity );
	if ( direction == Camera_Movement::CM_LEFT )
		transform->SetLocalPosition( position - transform->GetRight() * velocity );
	if ( direction == Camera_Movement::CM_RIGHT )
		transform->SetLocalPosition( position + transform->GetRight() * velocity );
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Editor3DCamera::ProcessMouseMovement( double xoffset, double yoffset, bool constrainPitch )
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if ( constrainPitch )
	{
		if ( Pitch > 89.0f )
			Pitch = 89.0f;
		if ( Pitch < -89.0f )
			Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Editor3DCamera::ProcessMouseScroll( double yoffset, float deltaTime )
{
	float velocity = MovementSpeed * deltaTime * (float)abs( yoffset ) * 5.0f;
	vec3 position = transform->GetLocalPosition();
	
	if ( yoffset > 0.0f )
		transform->SetLocalPosition( position + transform->GetForward() * velocity );
	else if ( yoffset < 0.0f )
		transform->SetLocalPosition( position - transform->GetForward() * velocity );
}


void Editor3DCamera::ProcessMousePin( double xoffset, double yoffset, float deltaTime )
{
	vec3 position = transform->GetLocalPosition();

	float velocityX = (float)xoffset * deltaTime * 2.0f;
	float velocityY = (float)yoffset * deltaTime * 2.0f;

	transform->SetLocalPosition( position - ( transform->GetUp() * velocityY ) - ( transform->GetRight() * velocityX ) );
}



// Calculates the front vector from the Camera's (updated) Euler Angles
void Editor3DCamera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = (float)( cos( glm::radians( Yaw ) ) * cos( glm::radians( Pitch ) ) );
	front.y = (float)( sin( glm::radians( Pitch ) ) );
	front.z = (float)( sin( glm::radians( Yaw ) ) * cos( glm::radians( Pitch ) ) );
	transform->SetForward( Math3D::Normalize( front ) );

	// Also re-calculate the Right and Up vector
	// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	transform->SetRight( Math3D::Normalize( Math3D::Cross( transform->GetForward(), Math3D::VECTOR3_UP ) ) );
	transform->SetUp( Math3D::Cross( transform->GetRight(), transform->GetForward() ) );
}



void Editor3DCamera::SetCamera( Camera* camera )
{
	this->camera = camera;

	transform->SetLocalPosition( vec3( 5.0f, 8.0f, 15.0f ) );

	this->Yaw = -110.0f;
	this->Pitch = -30.0;

	// Camera options
	this->MovementSpeed = 10.0f;
	this->MouseSensitivity = 0.1f;


	lastX = Screen::GetWidth() / 2.0f;
	lastY = Screen::GetHeight() / 2.0f;
	firstMouse = true;

	updateCameraVectors();
}

Camera* Editor3DCamera::GetCamera()
{
	return camera;
}

void Editor3DCamera::SetMANA3DEditorGUI( EditorGuiManager* editor )
{
	this->editor = editor;
}



void Editor3DCamera::CopyFrom( Component* trans )
{
	Debug::Log( "Copy EditorCamera" );
}