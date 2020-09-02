#ifndef EDITOR_GUI_ASSETS_WINDOW_H
#define EDITOR_GUI_ASSETS_WINDOW_H

#include <map>
#include <string>

namespace MANA3D
{
	class EditorGuiManager;
	class MTexture;
	class MString;
	struct DragDropGameObject;

	class EditorGuiAssetsWindow
	{
		friend class EditorGuiUtilitesWindow;

	private:
		EditorGuiAssetsWindow( EditorGuiManager* editorGuiManager );
		~EditorGuiAssetsWindow();


		void Display( std::map<std::string, MTexture*>& privateTexturesMap );

		
		void DisplayImportButtons();
		int DisplayNavigationButoons( float categoriesWindowWidth, float windowHeight );

		void DisplayPrefabs( std::map<std::string, MTexture*>& privateTexturesMap, float itemWidth, float itemWidthExtra, float assetContentBrowserWindowWidth );
		void DisplayMeshes( std::map<std::string, MTexture*>& privateTexturesMap, float itemWidth, float itemWidthExtra, float assetContentBrowserWindowWidth );
		void DisplayTextures( std::map<std::string, MTexture*>& privateTexturesMap, float itemWidth, float itemWidthExtra, float assetContentBrowserWindowWidth );
		void DisplayMaterials( std::map<std::string, MTexture*>& privateTexturesMap, float itemWidth, float itemWidthExtra, float assetContentBrowserWindowWidth );
		void DisplayCubemaps( std::map<std::string, MTexture*>& privateTexturesMap, float itemWidth, float itemWidthExtra, float assetContentBrowserWindowWidth );
		void DisplaySkinnedMeshes( std::map<std::string, MTexture*>& privateTexturesMap, float itemWidth, float itemWidthExtra, float assetContentBrowserWindowWidth );
		void DisplaySkeletons( std::map<std::string, MTexture*>& privateTexturesMap, float itemWidth, float itemWidthExtra, float assetContentBrowserWindowWidth );


		void MakeDragDropSourcePrefab( const MString& ddID, const MString& label, const DragDropGameObject& data );

		void MakeDragDropTargetToCreateNewPrefabAsset();
		void OnDropGameObjectOnPrefabsWindow();


		EditorGuiManager* editorGuiManager;
	};
}


#endif // !EDITOR_GUI_ASSETS_WINDOW_H
