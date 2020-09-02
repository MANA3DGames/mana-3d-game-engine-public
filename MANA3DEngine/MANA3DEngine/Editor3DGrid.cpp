#include "Editor3DGrid.h"
#include "SceneManager.h"
#include "MeshRenderer.h"
#include "MShader.h"
#include "Math3D.h"
using namespace MANA3D;
using namespace glm;

#include <glad/glad.h>


Editor3DGrid::Editor3DGrid( GameObject* grid, GameObject* cross, MMaterial* gridMat, MShader* gridShader, MMesh* gridMesh, MMesh* crossGridMesh ) : 
	gridShader( gridShader )
{
	mainGridGameObject = grid;
	mainGridMeshRenderer = mainGridGameObject->AddComponent<MeshRenderer>();
	mainGridMeshRenderer->SetMesh( gridMesh ); 
	mainGridMeshRenderer->SetDrawMode( GL_LINES );
	mainGridMeshRenderer->SetMaterial( gridMat );

	crossGameObject = cross;
	crossMeshRenderer = crossGameObject->AddComponent<MeshRenderer>();
	crossMeshRenderer->SetMesh( crossGridMesh );
	crossMeshRenderer->SetDrawMode( GL_LINES );
	crossMeshRenderer->SetMaterial( gridMat );
}

Editor3DGrid::~Editor3DGrid()
{
}


void Editor3DGrid::SetMainGridColor()
{
	glLineWidth( 1.0f );
	float val = 0.15f;
	gridShader->Use();
	gridShader->SetVec4( "fColor", vec4( val, val, val, 1.0 ) );
}

void Editor3DGrid::SetCrossGridColor()
{
	glLineWidth( 2.5f );
	float val = 0.45f;
	gridShader->Use();
	gridShader->SetVec4( "fColor", vec4( val, val, val, 1.0 ) );
}
