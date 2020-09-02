#ifndef MANA3D_RENDERER_H
#define MANA3D_RENDERER_H

#include "Component.h"
#include "MMaterial.h"
#include "MString.h"
#include <vector>

namespace MANA3D
{
	class Renderer : public Component
	{
		friend class MANA3DEngine;
		friend class EditorGuiManager;
		friend class Camera;
		friend class Model3DImporter;

	public:
		virtual void SetMaterial( MMaterial* material );
		virtual void SetMaterialShader( MShader* shader );
		virtual void SetMaterialTexture( const MString &name, MTexture* texture );
		virtual void SetMaterialBool( const MString &name, bool value ) const;
		virtual void SetMaterialInt( const MString &name, int value ) const;
		virtual void SetMaterialFloat( const MString &name, float value ) const;
		virtual void SetMaterialDouble( const MString &name, double value ) const;
		virtual void SetMaterialVec2( const MString &name, const glm::vec2 &value ) const;
		virtual void SetMaterialVec2( const MString &name, float x, float y ) const;
		virtual void SetMaterialVec3( const MString &name, const glm::vec3 &value ) const;
		virtual void SetMaterialVec3( const MString &name, float x, float y, float z ) const;
		virtual void SetMaterialVec4( const MString &name, const glm::vec4 &value ) const;
		virtual void SetMaterialVec4( const MString &name, float x, float y, float z, float w );
		virtual void SetMaterialMat2( const MString &name, const glm::mat2 &mat ) const;
		virtual void SetMaterialMat3( const MString &name, const glm::mat3 &mat ) const;
		virtual void SetMaterialMat4( const MString &name, glm::mat4 value ) const;

		MMaterial* GetMaterial();


	protected:
		Renderer();
		virtual ~Renderer();

		virtual void Render();

	protected:
		MMaterial* material;

		


	protected:
		static std::vector<Renderer*> sceneRenderers;
		static std::vector<Renderer*> runtimeRenderers;

		static int drawCallsCount;

	private: // This should be PRIVATE and only accessed by Camera class.
		static void RendererSceneObjects();
		static void RendererRuntimeObjects();
		static void RenderAll( std::vector<Renderer*>& container );

		static void RendererSceneObjectsSorted();
		static void RendererRuntimeObjectsSorted();
		static void RenderAllSorted( std::vector<Renderer*>& container );

		static void ClearRuntimeRenderers();

		static void RemoveRendererFromSceneRenderersContainer( Renderer* toBeRemoved );

	public:
		static int GetDrawCallsCount();

	};
}

#endif
