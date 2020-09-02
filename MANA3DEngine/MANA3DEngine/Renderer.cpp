#include "Renderer.h"
#include "Transform.h"
#include "SceneManager.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Camera.h"
#include "MANA3DEngine.h"
#include "GraphicSettings.h"
using namespace MANA3D;
using namespace glm;

using std::map;
using std::vector;


Renderer::Renderer() : Component(), material( nullptr )
{
	if ( MANA3DEngine::GetIsEditorLoop() )
		sceneRenderers.push_back( this );
	else
		runtimeRenderers.push_back( this );
}

Renderer::~Renderer()
{
	RemoveRendererFromSceneRenderersContainer( this );

	for ( vector<Renderer*>::iterator it = runtimeRenderers.begin(); it != runtimeRenderers.end(); ++it )
	{
		if ( *it == this )
		{
			runtimeRenderers.erase( it );
			return;
		}
	}
}


void Renderer::Render()
{
	material->BindMaterial();

	// Set Model-Projection-View Matrices
	SetMaterialMat4( "model", transform->GetModelMatrix() );

	// Set Current Main Camera position/ View Position
	SetMaterialVec3( "viewPos", Camera::GetCurrentMainCamera()->GetTransform()->GetLocalPosition() );

	// Set Model-Projection-View Matrices
	SetMaterialBool( "gammaCorrection", GraphicSettings::GetIsGammaCorrectionEnabled() );


	// Check if this is Light object?
	if ( gameObject->GetTag() == "Light" )
		return;

	// Directional Lights
	vector<DirectionalLight*> allDirLights = SceneManager::GetAllDirectionalLights();
	int index = 0;
	for ( vector<DirectionalLight*>::iterator it = allDirLights.begin(); it != allDirLights.end(); ++it )
	{
		if ( !( *it )->GetGameObject()->GetActive() )
			continue;

		MString str( "dirLights[%i]", index );
		const char* cStr = str.GetCStr();
		SetMaterialVec3( MString( "%s.direction", cStr ), ( *it )->GetTransform()->GetForward() );
		SetMaterialFloat( MString( "%s.ambient", cStr ), ( *it )->GetAmbient() );
		SetMaterialFloat( MString( "%s.diffuse", cStr ), ( *it )->GetDiffuse() );
		SetMaterialFloat( MString( "%s.specular", cStr ), ( *it )->GetSpecular() );
		index++;
	}
	SetMaterialInt( "directionalLightsCount", index );

	// Point Lights:
	vector<PointLight*> allPointLights = SceneManager::GetAllPointLights();
	index = 0;
	for ( vector<PointLight*>::iterator it = allPointLights.begin(); it != allPointLights.end(); ++it )
	{
		if ( !( *it )->GetGameObject()->GetActive() )
			continue;

		MString str( "pointLights[%i]", index );
		const char* cStr = str.GetCStr();
		SetMaterialVec3( MString( "%s.position", cStr ), ( *it )->GetTransform()->GetLocalPosition() );
		SetMaterialFloat( MString( "%s.ambient", cStr ), ( *it )->GetAmbient() );
		SetMaterialFloat( MString( "%s.diffuse", cStr ), ( *it )->GetDiffuse() );
		SetMaterialFloat( MString( "%s.specular", cStr ), ( *it )->GetSpecular() );
		SetMaterialFloat( MString( "%s.constant", cStr ), ( *it )->GetConstant() );
		SetMaterialDouble( MString( "%s.linear", cStr ), ( *it )->GetLinear() );
		SetMaterialDouble( MString( "%s.quadratic", cStr ), ( *it )->GetQuadratic() );
		index++;
	}
	SetMaterialInt( "pointLightsCount", index );

	// Spot Lights
	vector<SpotLight*> allSpotLights = SceneManager::GetAllSpotLights();
	index = 0;
	for ( vector<SpotLight*>::iterator it = allSpotLights.begin(); it != allSpotLights.end(); ++it )
	{
		if ( !( *it )->GetGameObject()->GetActive() )
			continue;

		MString str( "spotLights[%i]", index );
		const char* cStr = str.GetCStr();
		SetMaterialVec3( MString( "%s.position", cStr ), ( *it )->GetTransform()->GetLocalPosition() );
		SetMaterialVec3( MString( "%s.direction", cStr ), ( *it )->GetTransform()->GetForward() );
		SetMaterialFloat( MString( "%s.ambient", cStr ), ( *it )->GetAmbient() );
		SetMaterialFloat( MString( "%s.diffuse", cStr ), ( *it )->GetDiffuse() );
		SetMaterialFloat( MString( "%s.specular", cStr ), ( *it )->GetSpecular() );
		SetMaterialFloat( MString( "%s.constant", cStr ), ( *it )->GetConstant() );
		SetMaterialDouble( MString( "%s.linear", cStr ), ( *it )->GetLinear() );
		SetMaterialDouble( MString( "%s.quadratic", cStr ), ( *it )->GetQuadratic() );
		SetMaterialFloat( MString( "%s.cutOff", cStr ), ( *it )->GetCutOff() );
		SetMaterialFloat( MString( "%s.outerCutOff", cStr ), ( *it )->GetOuterCutOff() );
		index++;
	}
	SetMaterialInt( "spotLightsCount", index );
}


void Renderer::SetMaterial( MMaterial* material )
{
	glm::mat4 model = transform->GetModelMatrix();
	this->material = material;
	this->material->BindMaterial();
	transform->SetLocalModelMatrix( model );
}

void Renderer::SetMaterialShader( MShader* shader )
{
	material->SetShader( shader );
}

//void Renderer::SetMaterialTexture( const MString &name, MTexture* texture, int index )
//{
//	material->SetTexture( name, texture, index );
//}
void Renderer::SetMaterialTexture( const MString &name, MTexture* texture )
{
	material->SetTexture( name, texture );
}

void Renderer::SetMaterialBool( const MString &name, bool value ) const
{
	material->SetBool( name, value );
}
void Renderer::SetMaterialInt( const MString &name, int value ) const
{
	material->SetInt( name, value );
}
void Renderer::SetMaterialFloat( const MString &name, float value ) const
{
	material->SetFloat( name, value );
}
void Renderer::SetMaterialDouble( const MString &name, double value ) const
{
	material->SetDouble( name, value );
}
void Renderer::SetMaterialVec2( const MString &name, const glm::vec2 &value ) const
{
	material->SetVec2( name, value );
}
void Renderer::SetMaterialVec2( const MString &name, float x, float y ) const
{
	material->SetVec2( name, x, y );
}
void Renderer::SetMaterialVec3( const MString &name, const glm::vec3 &value ) const
{
	material->SetVec3( name, value );
}
void Renderer::SetMaterialVec3( const MString &name, float x, float y, float z ) const
{
	material->SetVec3( name, x, y, z );
}
void Renderer::SetMaterialVec4( const MString &name, const glm::vec4 &value ) const
{
	material->SetVec4( name, value );
}
void Renderer::SetMaterialVec4( const MString &name, float x, float y, float z, float w )
{
	material->SetVec4( name, x, y, z, w );
}
void Renderer::SetMaterialMat2( const MString &name, const glm::mat2 &mat ) const
{
	material->SetMat2( name, mat );
}
void Renderer::SetMaterialMat3( const MString &name, const glm::mat3 &mat ) const
{
	material->SetMat3( name, mat );
}
void Renderer::SetMaterialMat4( const MString &name, glm::mat4 value ) const
{
	material->SetMat4( name, value );
}


MMaterial* Renderer::GetMaterial()
{
	return material;
}





// Static - Variables & Functions

vector<Renderer*> Renderer::sceneRenderers;
vector<Renderer*> Renderer::runtimeRenderers;

int Renderer::drawCallsCount;

void Renderer::RendererSceneObjects()
{
	// Draw without distance sorting.
	RenderAll( sceneRenderers );
}
void Renderer::RendererRuntimeObjects()
{
	// Draw without distance sorting.
	RenderAll( runtimeRenderers );
}
void Renderer::RenderAll( vector<Renderer*>& container )
{
	//Debug::Log( container.size() );
	drawCallsCount = 0;
	for ( vector<Renderer*>::iterator it = container.begin(); it != container.end(); ++it )
	{
		GameObject* go = (*it)->GetGameObject();

		if ( go->GetActive() && (*it)->GetEnable() )
		{
			GameObjectType type = go->GetType();
			if ( type != GameObjectType::GOT_NORMAL )
			{
				continue;
			}

			bool draw = true;
			Transform *parent = go->transform->GetParent();
			while ( parent != nullptr )
			{
				if ( !parent->GetGameObject()->GetActive() )
				{
					draw = false;
					break;
				}
				parent = parent->GetParent();
			}

			if ( draw )
			{
				(*it)->Render();
				drawCallsCount++;
			}
		}
	}
}


void Renderer::RendererSceneObjectsSorted()
{
	RenderAllSorted( sceneRenderers );
}
void Renderer::RendererRuntimeObjectsSorted()
{
	RenderAllSorted( runtimeRenderers );
}
void Renderer::RenderAllSorted( vector<Renderer*>& container )
{
	vec3 camPos = Camera::GetCurrentMainCamera()->GetTransform()->GetLocalPosition();
	map<float, Renderer*> sortedRenderers;
	for ( vector<Renderer*>::iterator it = container.begin(); it != container.end(); ++it )
	{
		GameObjectType type = (*it)->GetGameObject()->GetType();
		bool proceed = type == GameObjectType::GOT_NORMAL || type == GameObjectType::GOT_NORMAL_SELECTED;
		if ( !proceed )
			continue;

		float distance = glm::length( camPos - (*it)->transform->GetLocalPosition() );
		sortedRenderers[distance] = *it;
	}

	drawCallsCount = 0;
	for ( map<float, Renderer*>::reverse_iterator it = sortedRenderers.rbegin(); it != sortedRenderers.rend(); ++it )
	{
		GameObject* go = it->second->GetGameObject();

		if ( go->GetActive() && it->second->GetEnable() )
		{
			GameObjectType type = go->GetType();
			bool proceed = type == GameObjectType::GOT_NORMAL || type == GameObjectType::GOT_NORMAL_SELECTED;
			if ( !proceed )
				continue;

			bool draw = true;
			Transform *parent = go->transform->GetParent();
			while ( parent != nullptr )
			{
				if ( !parent->GetGameObject()->GetActive() )
				{
					draw = false;
					break;
				}
				parent = parent->GetParent();
			}

			if ( draw )
			{
				it->second->Render();
				drawCallsCount++;
			}
		}
	}
}



int Renderer::GetDrawCallsCount()
{
	return drawCallsCount;
}


void Renderer::ClearRuntimeRenderers()
{
	while ( !runtimeRenderers.empty() )
	{
		Renderer *temp = *runtimeRenderers.begin();
		if ( temp != nullptr )
		{
			//Debug::Log( MString( "********* Remove >> %s", temp->gameObject->name.GetCStr()  ) );
			runtimeRenderers.erase( runtimeRenderers.begin() );
			delete temp;
		}
	}
	runtimeRenderers.clear();
}

void Renderer::RemoveRendererFromSceneRenderersContainer( Renderer* toBeRemoved )
{
	for ( vector<Renderer*>::iterator it = sceneRenderers.begin(); it != sceneRenderers.end(); ++it )
	{
		if ( *it == toBeRemoved )
		{
			//Debug::Log( MString( "********* Remove >> %s", (*it)->gameObject->name.GetCStr()  ) );
			sceneRenderers.erase( it );
			return;
		}
	}
}