#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "Renderer.h"
#include "MMesh.h"

namespace MANA3D
{
	typedef unsigned int GLenum;	// Just a forward declaration.

	class MeshRenderer : public Renderer
	{
		friend class SceneManager;
		friend class MANA3DEngine;
		friend class EditorGuiManager;
		friend class GameObject;
		friend class Camera;

	public:
		void SetMesh( MMesh* mesh );
		MMesh* GetMesh();

		void SetDrawMode( GLenum mode );
		void SetFaceCullingType( const int& type );
		void SetFaceCullingType( const MString& typeName );

		MString GetFaceCullingTypeName();

	protected:
		MeshRenderer();
		virtual ~MeshRenderer();

		void Render() override;

		virtual void DrawMesh();

		void CopyFrom( Component* ) override;

	private:
		MMesh* mesh;

		GLenum drawMode;

		int faceCullingType;




	private:

		static std::list<std::string> faceCullingOptions;

		// Only used from Editor - remove reference for public texture from public material.
		static void RemoveMeshReferences( MMesh* mesh );

	public:
		static std::list<std::string> GetFaceCullingOptionNamesList();

		static void SetFaceCullingOption( MeshRenderer* mRenderer, MString typeName );
	};
}

#endif // !MESH_RENDERER_H

