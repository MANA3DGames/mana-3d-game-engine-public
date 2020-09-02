#include "GraphicSettings.h"
#include "MShader.h"
#include "MCubemap.h"
#include "Skybox.h"
using namespace MANA3D;

#include <GL3W/gl3w.h>

using std::string;
using std::map;



list<string> GraphicSettings::PostProcessesList = {  
	"None",
	"Inversion",
	"Grayscale",
	"Sharpen",
	"Blur",
	"EdgeDetection"
};

MString GraphicSettings::currentSelectedPostProcessName = "None";
MShader* GraphicSettings::postprocessShader;

int GraphicSettings::faceCullingType;

Skybox* GraphicSettings::skybox;

bool GraphicSettings::isBlendingEnabled;
bool GraphicSettings::isMSAAEnabled;
bool GraphicSettings::isGammaCorrectionEnabled;
bool GraphicSettings::issRGBEnabled;


void GraphicSettings::Init()
{
	skybox = new Skybox( nullptr, MShader::GetPrivate("Skybox") );
	SetCurrentSelectedPostProcessName( "None" );
}

void GraphicSettings::SetFaceCullingWithoutSave( int type )
{
	switch ( type )
	{
	case 0:
		glEnable( GL_CULL_FACE );
		glCullFace( GL_BACK ); 
		break;
	case 1:
		glEnable( GL_CULL_FACE );
		glCullFace( GL_FRONT );
		break;
	case 2:
		glDisable( GL_CULL_FACE );
		break;
	}
}

void GraphicSettings::SetFaceCulling( int type )
{
	SetFaceCullingWithoutSave( type );
	faceCullingType = type;
}

int GraphicSettings::GetFaceCulling()
{
	return faceCullingType;
}


void GraphicSettings::EnableBlending( bool enable )
{
	isBlendingEnabled = enable;
}
bool GraphicSettings::GetIsBlendingEnabled()
{
	return isBlendingEnabled;
}

void GraphicSettings::EnableMSAA( bool enable )
{
	isMSAAEnabled = enable;
}
bool GraphicSettings::GetIsMSAAEnabled()
{
	return isMSAAEnabled;
}

void GraphicSettings::EnableGammaCorrection( bool enable )
{
	isGammaCorrectionEnabled = enable;
}
bool GraphicSettings::GetIsGammaCorrectionEnabled()
{
	return isGammaCorrectionEnabled;
}

void GraphicSettings::EnablesRGB( bool enable )
{
	issRGBEnabled = enable;
}
bool GraphicSettings::GetIssRGBEnabled()
{
	return issRGBEnabled;
}


void GraphicSettings::SetSkyboxCubemap( MCubemap* cubemap )
{
	skybox->SetCubmap( cubemap );
}
void GraphicSettings::SetSkyboxCubemap( MString name )
{
	string str = name.GetStr();
	map<string, MCubemap*> allCubemaps = MCubemap::GetAllCubemaps();
	for ( map<string, MCubemap*>::iterator it = allCubemaps.begin(); it != allCubemaps.end(); ++it )
	{
		if ( it->first == str )
		{
			SetSkyboxCubemap( it->second );
			return;
		}
	}

	SetSkyboxCubemap( nullptr );
}

MString GraphicSettings::GetSkyboxCubemapName()
{
	if ( skybox != nullptr && skybox->GetCubmap() != nullptr )
		return skybox->GetCubmap()->GetName();

	return "Clear Color";
}

void GraphicSettings::RenderSkybox()
{
	SetFaceCullingWithoutSave( 0 );
	skybox->Render();
	SetFaceCulling( faceCullingType );
}


void GraphicSettings::DestroyAll()
{
	delete skybox;
}



list<string> GraphicSettings::GetPostProcessesList()
{
	return PostProcessesList;
}

MString GraphicSettings::GetPostProcessName()
{
	return currentSelectedPostProcessName;
}

void GraphicSettings::SetCurrentSelectedPostProcessName( MString ppName )
{
	currentSelectedPostProcessName = ppName;
	postprocessShader = MShader::GetPrivate( MString( "PP_%s", ppName.GetCStr() ) );
}


void GraphicSettings::UsePPShader()
{
	if ( postprocessShader != nullptr )
		postprocessShader->Use();
}