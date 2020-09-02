#include "Time.h"
using namespace MANA3D;

#include <glfw3.h>
#include <stdio.h>


float Time::deltaTime;
float Time::startTime;
float Time::lastTime;
int Time::totalFrames;
int Time::fpsCount;



void Time::Init()
{
	startTime = (float)glfwGetTime();
	lastTime = 0.0f;
	deltaTime = 0.0f;
	totalFrames = 0;
}

bool Time::Update()
{
	totalFrames++;

	lastTime = (float)glfwGetTime();
	deltaTime = lastTime - startTime;

	if ( deltaTime > 0 )
	{
		fpsCount = (int)( totalFrames / deltaTime );
		totalFrames = 0;
		startTime = lastTime;

		return true;
	}

	return false;
}


float Time::GetTime()
{
	return (float)glfwGetTime();
}

float Time::GetDeltaTime()
{
	return deltaTime;
}

int Time::GetFPS()
{
	return fpsCount;
}

float Time::GetTimeNeededToDrawFrame()
{
	return  1000.0f / float( fpsCount );
}
