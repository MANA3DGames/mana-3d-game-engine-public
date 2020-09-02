#ifndef SKYBOX_H
#define SKYBOX_H

#include "MCubemap.h"

#include <string>

namespace MANA3D
{
	class MShader;

	class Skybox
	{
		friend class GraphicSettings;

	private:
		Skybox( MCubemap* cubemap, MShader* skyboxShader );
		~Skybox();

		void SetCubmap( MCubemap* cubemap );
		MCubemap* GetCubmap();

		void Render();

	private:

		unsigned int skyboxVAO, skyboxVBO;
		MCubemap* cubemap;
		MShader* skyboxShader;
	};
}

#endif // !SKYBOX_H