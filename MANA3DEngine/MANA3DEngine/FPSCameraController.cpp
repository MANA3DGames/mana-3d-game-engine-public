#include "FPSCameraController.h"
#include "Camera.h"
#include "Application.h"
#include "Screen.h"
#include "Input.h"
#include "Time.h"
using namespace MANA3D;
using namespace glm;


// Constructor with vectors
FPSCameraController::FPSCameraController() : 
	Component(), 
	yaw( -90.0f ),
	pitch( 0.0f ),
	movementSpeed( 2.5f ),
	mouseSensitivity( 0.1f ),
	lastX( 0.0f ),
	lastY( 0.0f ),
	firstMouse( false ),
	camera( nullptr )
{
	Application::LockCursor();
}

FPSCameraController::~FPSCameraController()
{

}


void FPSCameraController::Update()
{
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
		ProcessMouseScroll( (float)scrollY );


	double xpos = Input::GetMouseXPos();
	double ypos = Input::GetMouseYPos();

	// Just for the first time.
	if ( firstMouse && xpos == 0 && ypos == 0 )
		return;

	if ( firstMouse )
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos; // reversed since y-coordinates go from bottom to top

	lastX = (float)xpos;
	lastY = (float)ypos;
	
	ProcessMouseMovement( xoffset, yoffset );
}


// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void FPSCameraController::ProcessKeyboard( Camera_Movement direction, float deltaTime )
{
	float velocity = movementSpeed * deltaTime;
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
void FPSCameraController::ProcessMouseMovement( float xoffset, float yoffset, bool constrainPitch )
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if ( constrainPitch )
	{
		if ( pitch > 89.0f )
			pitch = 89.0f;
		if ( pitch < -89.0f )
			pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void FPSCameraController::ProcessMouseScroll( float yoffset )
{
	// Get current field of value.
	float fov = camera->GetFieldOfView();

	// Adjust field of view value.
	if ( fov >= 1.0f && fov <= 45.0f )
		fov -= yoffset;
	if ( fov <= 1.0f )
		fov = 1.0f;
	if ( fov >= 45.0f )
		fov = 45.0f;

	// Set new field of view value.
	camera->SetFieldOfView( fov );
}



// Calculates the front vector from the Camera's (updated) Euler Angles
void FPSCameraController::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos( glm::radians( yaw ) ) * cos( glm::radians( pitch ) );
	front.y = sin( glm::radians( pitch ) );
	front.z = sin( glm::radians( yaw ) ) * cos( glm::radians( pitch ) );
	transform->SetForward( Math3D::Normalize( front ) );

	// Also re-calculate the Right and Up vector
	// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	transform->SetRight( Math3D::Normalize( Math3D::Cross( transform->GetForward(), Math3D::VECTOR3_UP ) ) );
	transform->SetUp( Math3D::Cross( transform->GetRight(), transform->GetForward() ) );
}


void FPSCameraController::SetCamera( Camera* camera )
{
	this->camera = camera;

	lastX = Screen::GetWidth() / 2.0f;
	lastY = Screen::GetHeight() / 2.0f;
	firstMouse = true;

	updateCameraVectors();
}



void FPSCameraController::CopyFrom( Component* trans )
{
	Debug::Log( "Copy EditorCamera" );
}