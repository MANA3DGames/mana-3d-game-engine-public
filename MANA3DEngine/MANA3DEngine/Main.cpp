#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glfw3.lib" )

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "MANA3DEngine.h"
using namespace MANA3D;

// Main entrance of the applicaiton.
int main()
{
	// Create an instance of MANA 3D Engine.
	// --> This will enter the main loop for the enigne.
	MANA3DEngine::Create();
	// Destroy current instance of the engine Once we exit its main loop.
	MANA3DEngine::Destroy();

	// End of the applicaiton.
	return 0;
}