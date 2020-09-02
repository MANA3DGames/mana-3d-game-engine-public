#include "Skybox.h"
#include "Application.h"
#include "Debug.h"
#include "MShader.h"
#include "Camera.h"
using namespace MANA3D;

#include <GL3W/gl3w.h>

Skybox::Skybox( MCubemap* cubemap, MShader* skyboxShader )
{
	this->cubemap = cubemap;
	this->skyboxShader = skyboxShader;

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays( 1, &skyboxVAO );
	glGenBuffers( 1, &skyboxVBO );
	glBindVertexArray( skyboxVAO );
	glBindBuffer( GL_ARRAY_BUFFER, skyboxVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0 );
}

Skybox::~Skybox()
{
	glDeleteVertexArrays( 1, &skyboxVAO );
	glDeleteBuffers( 1, &skyboxVAO );
}


void Skybox::SetCubmap( MCubemap* cubemap )
{
	this->cubemap = cubemap;
}

MCubemap* Skybox::GetCubmap()
{
	return cubemap;
}


void Skybox::Render()
{
	if ( cubemap == nullptr )
		return;

	// Change depth function so depth test passes when values are equal to depth buffer's content.
	glDepthFunc( GL_LEQUAL );  
	
	skyboxShader->Use();
	skyboxShader->SetMat4( "view", glm::mat4( glm::mat3( Camera::GetCurrentMainCamera()->GetViewMatrix() ) ) ); // NOTE:: remove translation from the view matrix
	skyboxShader->SetMat4( "projection", Camera::GetCurrentMainCamera()->GetProjectionMatrix() );

	// skybox cube
	glBindVertexArray( skyboxVAO );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_CUBE_MAP, cubemap->GetID() );
	glDrawArrays( GL_TRIANGLES, 0, 36 );
	glBindVertexArray( 0 );
	glDepthFunc( GL_LESS ); // set depth function back to default
}