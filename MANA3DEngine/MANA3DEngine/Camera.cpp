#include "Camera.h"
#include "SkinnedMeshRenderer.h"
#include "MString.h"
#include "GraphicSettings.h"
#include "MANA3DEngine.h"
using namespace MANA3D;

#include <glad/glad.h>
#include <algorithm>

using namespace glm;


Camera::Camera( Transform* transform ) :
	transform( transform )
{
	this->gameObject = this->transform->GetGameObject();
	this->gameObject->SetTag( "MainCamera" );
	this->gameObject->SetAssociatedCamera( this );
	this->transform->SetForward( vec3( 0.0f, 0.0f, -1.0f ) );
	this->fieldOfView = 45.0f;
	this->nearPlane = 0.1f;
	this->farPlane = 1000.0f;
	this->bgColor = Color( 0.1f, 0.1f, 0.1f, 1.0f );
	this->viewWidth = 0;
	this->viewHeight = 0;

	if ( MANA3DEngine::GetIsEditorLoop() )
		MANA3DEngine::CreateGizmo( gameObject, "Camera" );
}

Camera::~Camera()
{
}



mat4 Camera::GetProjectionMatrix()
{
	return glm::perspective( glm::radians( mainCamera->fieldOfView ), (float)mainCamera->viewWidth / (float)mainCamera->viewHeight, mainCamera->nearPlane, mainCamera->farPlane );
}

mat4 Camera::GetViewMatrix()
{
	vec3 position = transform->GetLocalPosition();
	vec3 front = glm::normalize( transform->GetForward() );
	vec3 up = glm::normalize( transform->GetUp() );
	return glm::lookAt( position, position + front, up );
}


Transform* Camera::GetTransform()
{
	return transform;
}


void Camera::SetFieldOfView( float fov )
{
	fieldOfView = fov;
}
float Camera::GetFieldOfView()
{
	return fieldOfView;
}


void Camera::SetNearPlane( float val )
{
	nearPlane = val;
}
float Camera::GetNearPlane()
{
	return nearPlane;
}

void Camera::SetFarPlane( float val )
{
	farPlane = val;
}
float Camera::GetFarPlane()
{
	return farPlane;
}


void Camera::SetBGColor( Color bgColor )
{
	this->bgColor = bgColor;
}
Color Camera::GetBGColor()
{
	return bgColor;
}



void Camera::CopyPropertiesFrom( Camera* source )
{
	fieldOfView = source->fieldOfView;

	nearPlane = source->nearPlane;
	farPlane = source->farPlane;

	bgColor = source->bgColor;

	viewWidth = source->viewWidth;
	viewHeight = source->viewHeight;
}





#pragma region - Statics Functions/Memebers -

Camera* Camera::mainCamera;

void Camera::SetMainCamera( Camera* cam )
{
	mainCamera = cam;
}

void Camera::SetViewPortSize( int vWidth, int vHeight )
{
	mainCamera->viewWidth = vWidth;
	mainCamera->viewHeight = vHeight;
}


void Camera::ConfigGLForSelectedObjectPhase1()
{
	glEnable( GL_DEPTH_TEST );
	glStencilOp( GL_KEEP, GL_REPLACE, GL_REPLACE );		// GL_KEEP, GL_KEEP, GL_REPLACE
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	glStencilMask( 0x00 );
}
void Camera::ConfigGLForSelectedObjectPhase2( GameObject* selectedGO, GameObject* selectedOutline )
{
	// ************************************ For selected object.
	glStencilFunc( GL_ALWAYS, 1, 0xFF );
	glStencilMask( 0xFF );
	// ************************************ For selected object.

	// Render selected gameObject.
	bool canRenderSelectedGO = selectedGO != nullptr && selectedGO->GetActive();
	if ( canRenderSelectedGO )
	{
		// Check if it has a disabled MeshRenderer?
		MeshRenderer* meshRenderer = selectedGO->GetComponent<MeshRenderer>();
		if ( meshRenderer != nullptr && !meshRenderer->GetEnable() )
			canRenderSelectedGO = false;
	}

	if ( canRenderSelectedGO )
	{
		Camera::RenderFromMainCamera( selectedGO );

		Transform* goTransf = selectedGO->transform;

		// Renderer children of the current selected transfom.
		size_t childCount = goTransf->GetChildCount();
		for ( size_t i = 0; i < childCount; i++ )
			RenderFromMainCamera( goTransf->GetChild( i )->GetGameObject() );
	}

	// ************************************ For selected object.
	glStencilFunc( GL_NOTEQUAL, 1, 0xFF );
	glStencilMask( 0x00 );
	glDisable( GL_DEPTH_TEST );
	// ************************************ For selected object.


	// Render outline gameObject now.
	if ( canRenderSelectedGO && selectedOutline != nullptr )
	{
		// Get the outline gameObject for the current selected gameObject.
		Transform* goTransf = selectedOutline->transform;
			
		// Configure outline transformation (Scale along normals direction by shader).
		selectedOutline->transform->SetLocalModelMatrix( selectedGO->transform->GetModelMatrix() ); 

		// Renderer outline root.
		Camera::RenderFromMainCamera( selectedOutline );

		// Renderer outline children.
		size_t childCount = goTransf->GetChildCount();
		for ( size_t i = 0; i < childCount; i++ )
		{
			goTransf->GetChild( i )->SetLocalModelMatrix( selectedGO->transform->GetChild( i )->GetLocalModelMatrix() );
			RenderFromMainCamera( goTransf->GetChild( i )->GetGameObject() );
		}
	}
	// ************************************ For selected object.
}


void Camera::RenderFromEditorCamera()
{
	Render( Renderer::RendererSceneObjects, Renderer::RendererSceneObjectsSorted );
}
void Camera::RenderFromRuntimeCamera()
{
	Render( Renderer::RendererRuntimeObjects, Renderer::RendererRuntimeObjectsSorted );
}


void Camera::RenderFromMainCamera( GameObject* toBeRendered )
{
	if ( toBeRendered == nullptr )
	{
		//Debug::Log( "Object to be rendered is NULL!" );
		return;
	}

	Renderer* renderer = toBeRendered->GetComponent<MeshRenderer>();
	if ( renderer == nullptr )
	{
		renderer = toBeRendered->GetComponent<SkinnedMeshRenderer>();

		if ( renderer == nullptr )
			return;
	}

	renderer->Render();
}


void Camera::Render( void ( *RenderFunc )(), void ( *BlendingRenderFunc )() )
{
	if ( !mainCamera->gameObject->GetActive() )
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
		return;
	}

	Color color = mainCamera->bgColor;
	glClearColor( color.r, color.g, color.b, color.a );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	// Pass projection matrix to shader.
	mat4 projection = mainCamera->GetProjectionMatrix();

	// Camera/view transformation.
	mat4 view = mainCamera->GetViewMatrix();

	MShader::SetUniformBlocks( projection, view );

	if ( GraphicSettings::GetIsBlendingEnabled() )
	{
		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		BlendingRenderFunc();
	}
	else
	{
		glDisable( GL_BLEND );
		RenderFunc();
	}
}


Camera* Camera::GetCurrentMainCamera()
{
	return mainCamera;
}

#pragma endregion