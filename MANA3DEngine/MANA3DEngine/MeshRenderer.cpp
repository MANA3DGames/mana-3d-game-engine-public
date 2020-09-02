#include "MeshRenderer.h"
#include "GraphicSettings.h"
#include "Debug.h"
#include "MANA3DEngine.h"
using namespace MANA3D;

using std::string;
using std::list;
using std::vector;

#include <glad\glad.h>


MeshRenderer::MeshRenderer() :
	mesh( nullptr ),
	faceCullingType( 0 ),
	drawMode( GL_TRIANGLES )
{
}

MeshRenderer::~MeshRenderer() {}


void MeshRenderer::CopyFrom( Component* source )
{
	MeshRenderer* mRenderer = (MeshRenderer*)source;
	this->SetMesh( mRenderer->mesh );
	this->SetMaterial( mRenderer->material );
	this->faceCullingType = mRenderer->faceCullingType;
}


MMesh* MeshRenderer::GetMesh()
{
	return mesh;
}

void MeshRenderer::SetMesh( MMesh* mesh )
{
	this->mesh = mesh;
}

void MeshRenderer::SetDrawMode( GLenum mode )
{
	this->drawMode = mode;
}

void MeshRenderer::SetFaceCullingType( const int& type )
{
	this->faceCullingType = type;
}
void MeshRenderer::SetFaceCullingType( const MString& typeName )
{
	if ( typeName == "Back Face Culling" )
		this->faceCullingType = 0;
	else if ( typeName == "Front Face Culling" )
		this->faceCullingType = 1;
	else if ( typeName == "Double Face" )
		this->faceCullingType = 2;
}


MString MeshRenderer::GetFaceCullingTypeName()
{
	switch ( faceCullingType )
	{
	case 0:
		return "Back Face Culling";
	case 1:
		return "Front Face Culling";
	case 2:
		return "Double Face";
	default:
		return "None";
	}
}



void MeshRenderer::Render()
{
	// Call base render function to bind the material and to set common uniforms.
	Renderer::Render();

	// Draw the mesh.
	DrawMesh();
}

void MeshRenderer::DrawMesh()
{
	GraphicSettings::SetFaceCullingWithoutSave( faceCullingType );

	// Draw Mesh.
	if ( mesh == nullptr )
	{
		Debug::LogError( "Mesh is set to NULL!" );
		return;
	}

	glBindVertexArray( mesh->GetVAO() );
	glDrawElements( drawMode, mesh->GetIndexCount(), GL_UNSIGNED_INT, 0 );	//glDrawArrays( GL_TRIANGLES, 0, 36 );
	glBindVertexArray( 0 );
}


list<string> MeshRenderer::faceCullingOptions = { "Back Face Culling", "Front Face Culling", "Double Face" };

list<string> MeshRenderer::GetFaceCullingOptionNamesList()
{
	return faceCullingOptions;
}

void MeshRenderer::SetFaceCullingOption( MeshRenderer* mRenderer, MString typeName )
{
	mRenderer->SetFaceCullingType( typeName );
}


void MeshRenderer::RemoveMeshReferences( MMesh* mesh )
{
	vector<Renderer*>& renderers = MANA3DEngine::GetIsEditorLoop() ? sceneRenderers : runtimeRenderers;

	for ( vector<Renderer*>::iterator it = renderers.begin(); it != renderers.end(); ++it )
	{
		MeshRenderer* meshRenderer = (MeshRenderer*)*it;
		if ( meshRenderer->GetMesh() == mesh )
			meshRenderer->SetMesh( nullptr );
	}
}