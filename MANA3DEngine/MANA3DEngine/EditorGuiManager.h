#ifndef EDITOR_GUI_MANAGER_H
#define EDITOR_GUI_MANAGER_H

#include "ImGuiManager.h"
#include "MFile.h"
#include "Color.h"
#include "Math3D.h"
#include <list>
#include <stack>
#include <map>

#include <ImGui/imgui_internal.h>


namespace MANA3D
{
	class SpotLight;
	class GameObject;
	class Transform;
	class Component;
	class MTexture;
	class MCubemap;
	class MeshRenderer;
	class MMesh;
	class MMaterial;
	class EditorGuiMenuBar;
	class EditorGuiToolBar;
	class EditorGuiSceneHierarchyWindow;
	class EditorGuiSceneWindow;
	class EditorGuiUtilitesWindow;
	class EditorGuiInspectorWindow;
	class EditorGuiAssetsOverviewWindow;

	enum class OpenFileDialogAction
	{
		OFDA_OpenScene,
		OFDA_OpenProject,
		OFDA_SaveSceneAs,
		OFDA_Import3DModel,
		OFDA_Import2DTexture,
		OFDA_ImportCubemap
	};

	struct OpenFileDialogActionData
	{
		MString dialogTitle;
		MString btnLabel;
		OpenFileDialogAction action;
	};

	struct OpenFileDialogData
	{
		// Open File Dialog
		OpenFileDialogActionData actionData;
		
		MString driveLetter;
		MString currentPath;
		MString prevPath;		// Will be used to check if the path was changed or still the same.

		std::vector<MFile> splitPathData;
		std::vector<MFile> specialDirectories;
		std::stack<MString> previousPathStack;

		std::vector<MFile> currentAlldirectories;
		std::vector<MFile> currentAllfiles;

		int currentSelectedFolderIndex = -1;
		int currentSelectedFileIndex = -1;
		MFile currentSelectedFilePathData;
	};

	struct DragDropGameObject
	{
		MString name;
		GameObject* go;
	};
	struct DragDropTexture
	{
		MString name;
		MTexture* texture;
	};
	struct DragDropMaterial
	{
		MString name;
		MMaterial* material;
	};
	struct DragDropMesh
	{
		MString name;
		MMesh* mesh;
	};



	class EditorGuiManager
	{
		friend class MANA3DEngine;
		friend class Debug;

	private:

		EditorGuiManager();
		~EditorGuiManager();

		void Display( const unsigned int& textureColorbuffer, const int& drawCallsCount, std::map<std::string, MTexture*>& privateTexturesMap );


		void DisplayOpenFileDialog();

		void SetEnterGameLoopCallbackFunc( void ( *callbackFunc)() );


		void AddFolderPathToSplitPath( const MString& name, const MString& path );
		void AddFolderPathData( std::vector<MFile> &container, const MString& name, const MString& path );
		void CreateNewSplitPath( const MString& newPath );
		void ClearPreviousPathStack();
		void GetSpecialDirectories();
		void OnDoubleClickFileInOpenFileDialog( const MFile& file, const bool& importAll = false );

	private:

		EditorGuiMenuBar* guiMenuBar;
		EditorGuiToolBar* guiToolBar;
		EditorGuiSceneHierarchyWindow* guiSceneHierarchyWindow;
		EditorGuiSceneWindow* guiSceneWindow;
		EditorGuiUtilitesWindow* guiUtilitesWindow;
		EditorGuiInspectorWindow* guiInspectorWindow;
		EditorGuiAssetsOverviewWindow* guiAssetsOverviewWindow;

		bool isEditorOpened;
		bool isSceneOpened;
		bool isUtilitesOpened;
		bool isOpenFileDialogOpend;
		bool isCurrentSelectedGameObjectActive;

		ImVec2 sceneHierarchyWindowSize;
		ImVec2 utilitesWindowSize;

		GameObject* currentSelectedGameObjectInScene;
		GameObject* gameObjectToBeRemovedFromScene;
		GameObject* currentSelectedPrefabInAsset;
		MMesh* currentSelectedMeshInAsset;
		MMaterial* currentSelectedMaterialInAsset;
		MMaterial* materialToBeSelectedInAsset;
		MTexture* currentSelectedTextureInAsset;
		MTexture* textureToBeSelectedFromAsset;
		MCubemap* currentSelectedCubemapInAsset;
		MCubemap* cubemapToBeSelectedFromAsset;

		OpenFileDialogData gOpenFileDialogData;
		
		void (*enterGameLoopCallbackFunc)();
		
		ImGuiTabItemFlags importSettingsFlags;

		MString gtempTargetWrapMode;


	public:
		bool GetIsSceneWindowFocused();
		float GetSceneWindowTopOffsetY();
		ImVec2 GetSceneWindowSize();
		ImVec2 GetSceneHierarchyWindowSize();
		ImVec2 GetUtilitiesWindowSize();


		void StartDisableElement();
		void EndDisableElement();

		void AddLabelWithOffset( const MString& label, const float& leftMargin, const float& targetOffset );

		MString AddAssetElementTitle( const MString& str );

		void SetTempTargetWrapMode( const MString& str );
		MString GetTempTargetWrapMode();

		ImVec4 GetImVec4( const Color& color );
		Color GetColor( const ImVec4& vec4Color );
		float GetTextWidth( const char* txt );
		float CalcTextOffset( const char* txt, float targetOffset );

		void CreateNewMaterial();

		void StartOpenFileDialogAs( const MString& title, const MString& btnLabel, const OpenFileDialogAction& action );

		GameObject* GetCurrentSelectedGameObjectInScene();
		GameObject* GetCurrentGameObjectToBeRemovedFromScene();

		void SetCurrentSelectedPrefabInAsset( GameObject* go );
		GameObject* GetCurrentSelectedPrefabInAsset();

		void AddMeshRendererToCurrentSelectedGameObjectInScene();

		void DeleteCurrentGameObjectToBeRemovedFromScene();

		void OnClickToolBarPlayBtn();

		void OnClickItemInSceneHierarchy( Transform* root );

		void OnCreateNewSceneObjectFromMenuBar();



		void OnDropPrefabOnScene();

		void MakeDragDropSourceTexture( const MString& ddID, const MString& label, const DragDropTexture& data );
		void MakeDragDropTargetTexture( const ImRect& rect, const int& index, const MString& shaderTexName, MMaterial* mat );
		void OnDropTextureOnSlot( const ImRect& rect, const int& index, const MString& shaderTexName, MMaterial* mat  );

		void MakeDragDropSourceMaterial( const MString& ddID, const MString& label, const DragDropMaterial& data );
		void MakeDragDropTargetMaterial( const ImRect& rect, MeshRenderer* renderer );
		void OnDropMaterialOnSlot( const ImRect& rect, MeshRenderer* renderer );

		void MakeDragDropSourceMesh( const MString& ddID, const MString& label, const DragDropMesh& data );
		void MakeDragDropTargetMesh( const ImRect& rect, MeshRenderer* renderer );
		void OnDropMeshOnSlot( const ImRect& rect, MeshRenderer* renderer );



		void SetCurrentGameObjectToBeRemovedFromScene( GameObject* toBeRemoved );
		
		void SetCurrentSelectedTextureInAsset( MTexture* tex );
		MTexture* GetCurrentSelectedTextureInAsset();
		MTexture* GetTextureToBeSelectedFromAsset();

		void SetCurrentSelectedCubemapInAsset( MCubemap* );
		MCubemap* GetCurrentSelectedCubemapInAsset();

		void SetCurrentSelectedMaterialInAsset( MMaterial* mat );
		MMaterial* GetCurrentSelectedMaterialInAsset();
		MMaterial* GetMaterialToBeSelectedInAsset();

		void SetCurrentSelectedMeshInAsset( MMesh* );
		MMesh* GetCurrentSelectedMeshInAsset();


		void SetImportSettingsFlags( ImGuiTabItemFlags importSettingsFlags );
		ImGuiTabItemFlags GetImportSettingsFlags();

		bool GetIsCurrentSelectedGameObjectActive();


		void NulloutCurrentSelectedAssets();

		void DrawComboDrop( const float& leftMargin, const float& offset, const MString& label, const MString& startupVal, std::list<std::string>& choicesList, void ( *callbackFunc )( MString ) );
	
		template <typename T>
		void DrawComboDrop( 
			const ImVec2& leftMargin, const ImVec2& comboOffset, 
			const float& width, 
			const MString& label, const MString& startupSelected, std::list<std::string>& choicesList, const MString& selected,
			T callbackInstance, void (*callbackFunc)( T, MString str ) );

		void DrawCheckBox( const float& leftMargin, const float& offset, const MString& label, const bool& startupVal, void ( *callbackFunc )( bool ) );


		void DisplayMaterialComponent( MMaterial* mat );


#pragma region

	public: 
		static void BrowseDrive( EditorGuiManager* instance, MString newDrive );

#pragma endregion [Static Functions]

	};
}

#endif // !EDITOR_GUI_MANAGER_H

