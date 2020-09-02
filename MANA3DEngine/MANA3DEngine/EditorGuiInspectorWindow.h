#ifndef MANA3D_EDITOR_GUI_INSPECTOR_WINDOW_H
#define MANA3D_EDITOR_GUI_INSPECTOR_WINDOW_H

#include "EditorGuiWindow.h"
#include "MString.h"
#include "Math3D.h"
#include <map>



namespace MANA3D
{
	class MTexture;
	class Component;

	class EditorGuiInspectorWindow : public EditorGuiWindow
	{
		friend class EditorGuiManager;

	protected:
		EditorGuiInspectorWindow( EditorGuiManager* editorGuiManager );
		virtual ~EditorGuiInspectorWindow();


		void Display( std::map<std::string, MTexture*>& privateTexturesMap );

		void DisplayInspectorWindowActiveCheckbox();
		void DisplayInspectorWindowGameObjectName();
		void DisplayInspectorWindowGameObjectTag( std::map<std::string, MTexture*>& privateTexturesMap );
		void DisplayInspectorWindowTransfrom();
		void DisplayInspectorWindowTransfromElement( glm::vec3 elementVec, const MString& elementID );
		void DisplayInspectorWindowComponents();
		void DisplayInspectorWindowComponentActiveCheckbox( const MString& id, Component* component, bool isEnabled );

		void DisplayInspectorWindowMeshRenderer( Component* component );
		void DisplayInspectorWindowSkinnedMeshRenderer( Component* component );

		void DrawSimpleSpecialInstanceIcon( const unsigned int& id, const float& leftMargin, const float& topMargin, const float& size );
		void DrawSimpleSpecialInstanceIcon( const float& leftMargin, const float& topMargin, const float& size );
		void DisplayInspectorWindowCameraInstance();
		void DisplayInspectorWindowLightInstance();
		void DisplayInspectorWindowCommonLightProperties( const float& leftMargin, const float& targetOffset );
		void DisplayInspectorWindowCommonLightProperties2( const float& leftMargin, const float& targetOffset );
		void DisplayInspectorWindowPointLightInstance();
		void DisplayInspectorWindowSpotLightInstance();
		void DisplayInspectorWindowDirectionalLightInstance();
	};
}

#endif // !MANA3D_EDITOR_GUI_INSPECTOR_WINDOW_H

