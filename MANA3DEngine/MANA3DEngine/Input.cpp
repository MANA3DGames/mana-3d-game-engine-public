#include "Input.h"
#include "Debug.h"
using namespace MANA3D;


#pragma region -- Static Functions/Members --

GLFWwindow* Input::window;
double Input::scrollXOffset;
double Input::scrollYOffset;
double Input::mouseXPos;
double Input::mouseYPos;
bool Input::isMouseRight;
bool Input::isMouseLeft;
bool Input::isMouseMiddle;


void Input::SetWindow( GLFWwindow* window )
{
	Input::window = window;

	Reset();
	mouseXPos = 0.0;
	mouseYPos = 0.0;
}

void Input::ScrollCallback( GLFWwindow* window, double xoffset, double yoffset )
{
	scrollXOffset = xoffset;
	scrollYOffset = yoffset;
}
void Input::MouseCallback( GLFWwindow* window, double xpos, double ypos )
{
	mouseXPos = xpos;
	mouseYPos = ypos;
}
void Input::MouseButtonCallback( GLFWwindow* window, int button, int action, int mods )
{
	if ( button == KC_MouseRight )
		isMouseRight = action == GLFW_PRESS;

	if ( button == KC_MouseLeft )
		isMouseLeft = action == GLFW_PRESS;

	if ( button == KC_MouseMiddle )
		isMouseMiddle = action == GLFW_PRESS;
}


void Input::Reset()
{
	scrollXOffset = 0.0;
	scrollYOffset = 0.0;

	/*isMouseLeft = false;
	isMouseRight = false;
	isMouseMiddle = false;*/
}




bool Input::GetKeyPressed( KeyCode code )
{
	return glfwGetKey( window, code ) == GLFW_PRESS;
}
bool Input::GetKeyReleased( KeyCode code )
{
	return glfwGetKey( window, code ) == GLFW_RELEASE;
}

double Input::GetScrollX()
{
	return scrollXOffset;
}
double Input::GetScrollY()
{
	return scrollYOffset;
}


double Input::GetMouseXPos()
{
	return mouseXPos;
}
double Input::GetMouseYPos()
{
	return mouseYPos;
}


bool Input::GetMouseRightClick()
{
	return isMouseRight;
}
bool Input::GetMouseLeftClick()
{
	return isMouseLeft;
}
bool Input::GetMouseMiddleClick()
{
	return isMouseMiddle;
}

#pragma endregion