#ifndef MANA3D_MAIN_H
#define MANA3D_MAIN_H

#include <glad\glad.h>
#include <glfw3.h>

namespace MANA3D
{
#pragma region

	// Forward decleration(s) for some needed classes.
	class EditorGuiManager;
	class Editor3DSceneManager;
	class MANA3DFramework;
	class SceneManager;
	class Skybox;
	class MTexture;
	class MMaterial;
	class MMesh;
	class GameObject;
	class MString;

#pragma endregion [Forward decleration(s)]


	// Main class which controls the whole engine.
	class MANA3DEngine
	{
	private:
		// Default constrcutor.
		MANA3DEngine();
		// Default destructor.
		~MANA3DEngine();
		

		// Initializes engine components.
		void Init();

		// Create an instance of GLFW window.
		GLFWwindow* CreateGLFWWindow();

		// Register all GLFW with our custom callback functions.
		// * window: current glfwWindow instance.
		void RegisterGLFWCallbacks( GLFWwindow* window );

		// Generate frame buffer to be used as a final image.
		void GenerateFramebuffer();

		// Generate frame buffer to be used with MSAA.
		void GenerateMSAAFramebuffer();

		// Create screen quad which going to take a frame buffer as final texture.
		void CreateScreenQuad();

		// Loads applicaiton special icon.
		// * window: current glfwWindow instance.
		void LoadApplicationIcon( GLFWwindow* window );

		// Constructs default tags array.
		void ConstructDefaultTags();

		// Configures MANA Editor.
		void ConfigMANAEditor();

		// MANA 3D Engine main loop.
		// * window: current glfwWindow instance.
		void MainLoop( GLFWwindow* window );

		// Swaps back buffer to draw next frame.
		// * window: current glfwWindow instance.
		void SwapBuffer( GLFWwindow* window );

		// To be called on application termination to release resources and free memory.
		void OnTerminate();

		// Clears OpenGL window.
		// * x: r value of the rgb color.
		// * y: g value of the rgb color.
		// * z: b value of the rgb color.
		void ClearWindow( float x, float y, float z ) const;


		// Enters MANA 3D editor loop.
		void EnterEditorLoop();
		// MANA 3D Editor loop.
		void EditorLoop();

		// Lets the engine know that it should enter the game loop in the upcoming frame.
		void TriggerEnterGameLoop();
		// Enters the gameplay loop.
		void EnterGameLoop();
		// Gameplay loop.
		void GameLoop();
		// Exits gameplay loop.
		void ExitGameLoop();



		// Renders final frame buffer.
		void RenderFrameBuffer();
		// Renders final frame buffer with MSAA.
 		void RenderMSAAFrameBuffer();


	private:

#pragma region 

		// MANA 3D editor gui manager instance.
		EditorGuiManager* mana3DEditorGUI;
		// MANA 3D editor scene manager instance.
		Editor3DSceneManager* mana3dEditorManager;
		// Scene manager instance.
		SceneManager* sceneManager;
		// MANA 3D Framework instance.
		MANA3DFramework* framework;

#pragma endregion [Managers]
		

#pragma region 

		// Our custom Frame buffer object (combination of all buffers; color, depth and stencil).
		unsigned int frameBufferObject;
		// Texture to be generated to be attached to the framebuffer.
		// All rendering commands will write to the texture as if it was a normal color/depth or stencil buffer. 
		// The advantage of using textures is that the render output is stored inside the texture image that we can then easily use in our shaders. 
		unsigned int textureColorBuffer;
		// Actual buffer stores all rendering data without any conversion to texture specific format (more optimized).
		unsigned int renderBufferObject;

#pragma endregion [Normal frame buffer object]

#pragma region 

		// Our custom frame buffer object which will be used when MSAA is enabled.
		unsigned int msaaFrameBufferObject;
		// Intermediate custom frame buffer object; "We can't directly use the multisampled texture(s) in the fragment shader. 
		// What we can do however is blit the multisampled buffer(s) to a different FBO with a non-multisampled texture attachment. 
		// We then use this ordinary color attachment texture for post-processing, effectively post-processing an image rendered via multisampling. 
		// This does mean we have to generate a new FBO that acts solely as an intermediate framebuffer object to resolve the multisampled buffer into; 
		// a normal 2D texture we can use in the fragment shader."
		unsigned int msaaIntermediateFrameBufferObject;
		// texture buffer to be attached to frame buffer when MSAA is enabled.
		unsigned int msaaTextureColorBuffer;
		// Actual buffer to store all rendering data.
		unsigned int msaaRenderBufferObject;
		// Final generated texture for the whole scene.
		unsigned int screenTexture;

#pragma endregion [MSAA frame buffer object]

		// Final quad to be rendered on the whole screen.
		unsigned int screenQuadVAO;
		// Final quad to be rendered on the whole screen.
		unsigned int screenQuadVBO;

		// Indicates whether we are in the editor loop or not.
		bool isEditorLoop;
		// Indicates whether the entering for the game loop is triggered or not yet.
		bool isEnterGameLoopTriggered;


#pragma region

	public:
		// Returns true if we are in the editor loop otherwise return false.
		static bool GetIsEditorLoop();

		// Creates gizmo.
		// * go: gizmo's gameObject.
		// * name: gizmo's type name (icon).
		static void CreateGizmo( GameObject* go, const MString& name );

		// Creates instance of MANA 3D Engine class.
		static void Create();
		// Destroys current MANA 3D Engine instance.
		static void Destroy();


	private:
		// Current instance of MANA 3D Engine (singleton: only one instance is allowed).
		static MANA3DEngine* mana3DMain;

		// Enters gameplay loop, mainly called from editor play btn.
		static void EnterGameLoopCallback();

		// To be called when framebuffer window resized.
		// * window: current instance of GLFW window.
		// * width: new window width.
		// * height: new window height.
		static void FramebufferSizeCallback( GLFWwindow* window, GLint width, GLint height );

		// To be called when user uses mouse wheel on glfw window.
		static void OnScrollCallback( GLFWwindow* window, double xoffset, double yoffset );
		
		// To be called when user destroy a texture asset.
		// * tex: MTexture asset to be destroyed.
		static void OnDestroyMTexture( MTexture* tex );
		
		// To be called when user destroy a mesh asset.
		// * mesh: MMesh asset to be destroyed.
		static void OnDestroyMMesh( MMesh* mesh );

#pragma endregion [Static Functionality]
	};
}

#endif