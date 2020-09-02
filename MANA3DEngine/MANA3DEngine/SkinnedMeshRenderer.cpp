#include "SkinnedMeshRenderer.h"
#include "Skeleton.h"
#include "Debug.h"
using namespace MANA3D;

#include <glad\glad.h>


SkinnedMeshRenderer::SkinnedMeshRenderer() : MeshRenderer(), mesh( nullptr ), skeleton( nullptr )
{
}

SkinnedMeshRenderer::~SkinnedMeshRenderer() 
{
}


void SkinnedMeshRenderer::CopyFrom( Component* source )
{
	SkinnedMeshRenderer* mRenderer = (SkinnedMeshRenderer*)source;
	this->SetSkinnedMesh( mRenderer->mesh );
	this->SetMaterial( mRenderer->material );
	this->SetSkeleton( mRenderer->skeleton );
}


void SkinnedMeshRenderer::SetSkinnedMesh( MSkinnedMesh* mesh )
{
	SetMesh( mesh );
	this->mesh = mesh;
}

void SkinnedMeshRenderer::Render()
{
	// Call base render function to bind the material and to set common uniforms.
	Renderer::Render();

	// Load Joint Transforms to shader.
	if ( skeleton != nullptr )
		skeleton->Update( this );

	// Draw the mesh.
	DrawMesh();
}



MSkinnedMesh* SkinnedMeshRenderer::GetSkinnedMesh()
{
	return mesh;
}


void SkinnedMeshRenderer::SetSkeleton( Skeleton* skeleton )
{
	this->skeleton = skeleton;
}