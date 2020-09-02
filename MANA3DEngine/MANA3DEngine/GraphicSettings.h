#ifndef GRAPHIC_SETTINGS_H
#define GRAPHIC_SETTINGS_H

#include <list>
using std::list;

#include "MString.h"


namespace MANA3D
{
	class MCubemap;
	class Skybox;
	class MShader;

	class GraphicSettings
	{
		friend class MANA3DEngine;
		friend class MeshRenderer;

	public:
		static void Init();

		static void SetFaceCulling( int type );
		static int GetFaceCulling();

		static void SetSkyboxCubemap( MCubemap* );
		static void SetSkyboxCubemap( MString );
		static MString GetSkyboxCubemapName();

		static void EnableBlending( bool );
		static bool GetIsBlendingEnabled();

		static void EnableMSAA( bool );
		static bool GetIsMSAAEnabled();

		static void EnableGammaCorrection( bool );
		static bool GetIsGammaCorrectionEnabled();

		static void EnablesRGB( bool );
		static bool GetIssRGBEnabled();


		static std::list<std::string> GetPostProcessesList();
		static MString GetPostProcessName();

		static void SetCurrentSelectedPostProcessName( MString ppName );

	private:

		static void SetFaceCullingWithoutSave( int type );
		
		static void RenderSkybox();
		
		static void UsePPShader();

		static void DestroyAll();


		static std::list<std::string> PostProcessesList;
		static MString currentSelectedPostProcessName;

		static int faceCullingType;

		static Skybox* skybox;

		static MShader* postprocessShader;

		static bool isBlendingEnabled;
		static bool isMSAAEnabled;
		static bool isGammaCorrectionEnabled;
		static bool issRGBEnabled;
	};
}

#endif // !GRAPHIC_SETTINGS_H

