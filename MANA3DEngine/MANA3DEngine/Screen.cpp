#include "Screen.h"
using namespace MANA3D;

#include <Windows.h>
#include <glad/glad.h>


Screen::Screen() :
	height( 0 ),
	width( 0 ),
	window( nullptr ),
	monitor( nullptr )
{
}

Screen::~Screen()
{
}



Screen*	Screen::instance;

void Screen::Create( GLFWwindow* window, const int& width, const int& height )
{
	if ( instance != 0 )
		return;

	instance = new Screen;
	instance->width = width;
	instance->height = height;
	instance->window = window;

	instance->monitor =  glfwGetPrimaryMonitor();
}
void Screen::Destroy()
{
	delete instance;
}


void Screen::SetSize( const int& width, const int& height )
{
	instance->width = width;
	instance->height = height;
}


int Screen::GetWidth()
{
	return instance->width;
}
int Screen::GetHeight()
{
	return instance->height;
}

void Screen::Maximize()
{
	// Get the resolution of the primary monitor
	const GLFWvidmode* vidmode = glfwGetVideoMode( glfwGetPrimaryMonitor() );
	SetSize( vidmode->width, vidmode->height );
	int nCaptionHeight = ::GetSystemMetrics( SM_CYCAPTION );
	glfwSetWindowSize( instance->window, Screen::GetWidth(), Screen::GetHeight() - ( nCaptionHeight * 3 ) );
	glfwSetWindowPos( instance->window, 0, nCaptionHeight + 3 );
}


bool Screen::IsFullscreen( void )
{
	return glfwGetWindowMonitor( instance->window ) != nullptr;
}

void Screen::SetFullScreen( const bool& fullscreen )
{
    if ( instance->IsFullscreen() == fullscreen )
        return;

    if ( fullscreen )
    {
        // backup window position and window size
        glfwGetWindowPos( instance->window, &instance->_wndPos[0], &instance->_wndPos[1] );
        glfwGetWindowSize( instance->window, &instance->_wndSize[0], &instance->_wndSize[1] );

        // get resolution of monitor
        const GLFWvidmode * mode = glfwGetVideoMode( instance->monitor );

        // switch to full screen
        glfwSetWindowMonitor( instance->window, instance->monitor, 0, 0, mode->width, mode->height, 0 );
    }
    else
    {
        // restore last window size and position
        glfwSetWindowMonitor( instance->window, nullptr, instance->_wndPos[0], instance->_wndPos[1], instance->_wndSize[0], instance->_wndSize[1], 0 );
    }

	glfwGetFramebufferSize( instance->window, &instance->_vpSize[0], &instance->_vpSize[1] );
	glViewport( 0, 0, instance->_vpSize[0], instance->_vpSize[1] );
}