#include "MANA3DEngine.h"
#include "Screen.h"
#include "Time.h"
#include "Input.h"
#include "Application.h"
#include "GameObject.h"
#include "Camera.h"
#include "Editor3DCamera.h"
#include "Renderer.h"
#include "MSkinnedMesh.h"
#include "Skeleton.h"
#include "MPrimitive.h"
#include "MCubemap.h"
#include "SceneManager.h"
#include "MANA3DFramework.h"
#include "EditorGuiManager.h"
#include "Editor3DSceneManager.h"
#include "Texture2DImporter.h"
#include "GraphicSettings.h"
using namespace MANA3D;

using std::string;

// Just to load window icon.
#include <stb_image.h>




MANA3DEngine::MANA3DEngine() :
	frameBufferObject( NULL ),
	textureColorBuffer( NULL ),
	screenTexture( NULL ),
	renderBufferObject( NULL ),
	screenQuadVAO( NULL ),
	screenQuadVBO( NULL ),
	msaaFrameBufferObject( NULL ),
	msaaIntermediateFrameBufferObject( NULL ),
	msaaRenderBufferObject( NULL ),
	msaaTextureColorBuffer( NULL ),
	framework( nullptr ),
	mana3DEditorGUI( nullptr ),
	sceneManager( nullptr ),
	mana3dEditorManager( nullptr ),
	isEditorLoop( true ),
	isEnterGameLoopTriggered( false )
{
}

MANA3DEngine::~MANA3DEngine()
{
	// Free all used memory.
	glDeleteVertexArrays( 1, &screenQuadVAO );
	glDeleteBuffers( 1, &screenQuadVBO );
	glDeleteFramebuffers( 1, &frameBufferObject );
	glDeleteFramebuffers( 1, &msaaFrameBufferObject );
	glDeleteFramebuffers( 1, &msaaIntermediateFrameBufferObject );
	glDeleteTextures( 1, &textureColorBuffer );
	glDeleteTextures( 1, &msaaTextureColorBuffer );
	glDeleteTextures( 1, &msaaTextureColorBuffer );
	glDeleteRenderbuffers( 1, &screenTexture );
	glDeleteRenderbuffers( 1, &msaaRenderBufferObject );
}


void MANA3DEngine::Init()
{
	// Initialize OpenGL and Create a GLFW window.
	GLFWwindow* window = CreateGLFWWindow();

	// Check if we faild to create a GLFW window?
	if ( window == nullptr )
	{
		// terminate the application.
		glfwTerminate();
		return;
	}

	// Create screen instance.
	int width, height;
	glfwGetWindowSize( window, &width, &height );
	Screen::Create( window, width, height );

	// Initilaize Time
	Time::Init();

	// Initialize ImGUI.
	ImGuiManager::Init( window );
	
	//glEnable( GL_MULTISAMPLE );

	// Set Input window instance.
	Input::SetWindow( window );

	// Set application window instance.
	Application::SetWindow( window );

	// Register input listeners.
	RegisterGLFWCallbacks( window );
	
	// Load Shaders folders.
	MShader::LoadShadersFolder();

	// Load Editor Textures.
	Texture2DImporter::LoadEditorTextures();

	// Initialize all default materials.
	MMaterial::InitPrivateMatrials( MTexture::privateTexturesMap );

	// Create private meshes.
	MMesh::CreatePrivateMeshes();

	// Create All Primitive Meshes
	MPrimitive::CreateAllMeshes();

	// Config MANA 3D Editor.
	ConfigMANAEditor();

	// Create Graphic Settings
	GraphicSettings::Init();
	GraphicSettings::SetFaceCulling( 0 );
	

	// Create a instance of SceneManager.
	sceneManager = new SceneManager( MMaterial::GetPrivate( "EditorOutlineMat" ) );

	// Create framewrok instance.
	framework = new MANA3DFramework;

	// Load application icon.
	LoadApplicationIcon( window );

	// Framebuffer configuration.
	GenerateFramebuffer();
	GenerateMSAAFramebuffer();

	// Create screen quad VAO that which is going to take the framebuffer as a final texture.
	CreateScreenQuad();

	// Configure global opengl state:
	glViewport( 0, 0, Screen::GetWidth(), Screen::GetHeight() );


	// Enter Editor Loop.
	EnterEditorLoop();

	// Enter the main loop.
	MainLoop( window );

	// Call onTerminate once we done.
	OnTerminate();
}

GLFWwindow* MANA3DEngine::CreateGLFWWindow()
{
	// Initialize GLFW.
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 4 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

#ifdef __APPLE__
	// uncomment this statement to fix compilation on OS X
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
#endif

	// Create GLFW window; MANA 3D Engine main window.
	GLFWwindow* window = glfwCreateWindow( 800, 600, "MANA 3D Enigne", nullptr, nullptr );

	// Check if the window was created succesfully.
	if ( window == nullptr )
	{
		Debug::LogError( "Failed to create GLFW window!" );
		glfwTerminate();
		return nullptr;
	}

	// Make our window as the current context fot he current thread (calling thread).
	glfwMakeContextCurrent( window );
	// Set our custom funciton as a callback func when our window size change.
	glfwSetFramebufferSizeCallback( window, MANA3DEngine::FramebufferSizeCallback );

	// Load GLAD.
	if ( !gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) )
	{
		Debug::LogError( "Failed to initialize GLAD!" );
		return nullptr;
	}

	// Everything work just fine, now we can return the new created window.
	return window;
}

void MANA3DEngine::RegisterGLFWCallbacks( GLFWwindow* window )
{
	// Set our callback function when user uses mouse wheel.
	glfwSetScrollCallback( window, OnScrollCallback );
	// Set our callback function when user moves the mouse's cursor.
	glfwSetCursorPosCallback( window, Input::MouseCallback );
	// Set our callback function when user click mouse button(s).
	glfwSetMouseButtonCallback( window, Input::MouseButtonCallback );

	// Set window input mode.
	glfwSetInputMode( window, GLFW_STICKY_MOUSE_BUTTONS, 1 );
}

void MANA3DEngine::LoadApplicationIcon( GLFWwindow* window )
{
	// Get the full path for MANA 3D logo.
	MString path( "%sEditor\\AppWindow\\Icon.png", Application::GetAppDirctory().GetCStr() );
	
	// Declare glfw image to be used as icon for our window.
	GLFWimage image;

	// Load our logo image using stbi.
	image.pixels = stbi_load( path.GetCStr(), &image.width, &image.height, 0, 4 ); 

	// Set the loaded logo image as icon for our window.
	glfwSetWindowIcon( window, 1, &image );

	// Clear image memory.
	stbi_image_free( image.pixels );
}

void MANA3DEngine::ConstructDefaultTags()
{
	// Construct tag map with default tags.
	SceneManager::ConstructTagMap( list<string>{ "MainCamera", "Light", "Player", "Enemy" } );
}


void MANA3DEngine::GenerateFramebuffer()
{
	GLsizei width = (GLsizei)Screen::GetWidth();
	GLsizei height = (GLsizei)Screen::GetHeight();

	glGenFramebuffers( 1, &frameBufferObject );
	glBindFramebuffer( GL_FRAMEBUFFER, frameBufferObject );

	// Create a color attachment texture.
	glGenTextures( 1, &textureColorBuffer );
	glBindTexture( GL_TEXTURE_2D, textureColorBuffer );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0 );
	
	// Create a renderbuffer object for depth and stencil attachment (we won't be sampling these).
	glGenRenderbuffers( 1, &renderBufferObject );
	glBindRenderbuffer( GL_RENDERBUFFER, renderBufferObject );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height ); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject ); // now actually attach it
	
	// Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now.
	if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
		Debug::LogError( "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" );

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void MANA3DEngine::GenerateMSAAFramebuffer()
{
	GLsizei width = (GLsizei)Screen::GetWidth();
	GLsizei height = (GLsizei)Screen::GetHeight();

    glGenFramebuffers( 1, &msaaFrameBufferObject );
    glBindFramebuffer( GL_FRAMEBUFFER, msaaFrameBufferObject );

    // create a multisampled color attachment texture
    glGenTextures( 1, &msaaTextureColorBuffer );
    glBindTexture( GL_TEXTURE_2D_MULTISAMPLE, msaaTextureColorBuffer );
    glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width, height, GL_TRUE );
    glBindTexture( GL_TEXTURE_2D_MULTISAMPLE, 0 );
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, msaaTextureColorBuffer, 0 );

    // create a (also multisampled) renderbuffer object for depth and stencil attachments
    glGenRenderbuffers( 1, &msaaRenderBufferObject );
    glBindRenderbuffer( GL_RENDERBUFFER, msaaRenderBufferObject );
    glRenderbufferStorageMultisample( GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height );
    glBindRenderbuffer( GL_RENDERBUFFER, 0 );
    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, msaaRenderBufferObject);

    if ( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE )
        Debug::LogError( "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" );

    glBindFramebuffer( GL_FRAMEBUFFER, 0 );

    // configure second post-processing framebuffer
    glGenFramebuffers( 1, &msaaIntermediateFrameBufferObject );
    glBindFramebuffer( GL_FRAMEBUFFER, msaaIntermediateFrameBufferObject );

    // create a color attachment texture
    glGenTextures( 1, &screenTexture );
    glBindTexture( GL_TEXTURE_2D, screenTexture );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0 );	// we only need a color buffer

    if ( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE )
		Debug::LogError( "ERROR::FRAMEBUFFER:: Intermediate Framebuffer is not complete!" );

    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void MANA3DEngine::CreateScreenQuad()
{
	// Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	float quadVertices[] = { 
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	// Screen quad VAO.
	glGenVertexArrays( 1, &screenQuadVAO );
	glGenBuffers( 1, &screenQuadVBO );
	glBindVertexArray( screenQuadVAO );
	glBindBuffer( GL_ARRAY_BUFFER, screenQuadVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0 );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)( 2 * sizeof(float) ) );
}

void MANA3DEngine::ClearWindow( float x, float y, float z ) const
{	
	//clear the color of the window with an alpha value of 1.0f
	glClearColor( x, y, z, 1.0f );  

	//clear both the color buffer bit and the depth buffer bit
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
}

void MANA3DEngine::ConfigMANAEditor()
{
	mana3dEditorManager = new Editor3DSceneManager( MMaterial::GetPrivate( "Editor3DGridMat" ), MShader::GetPrivate( "Editor3DGrid" ), MMesh::GetPrivate( "Grid" ), MMesh::GetPrivate( "CrossGrid" ) );

	// Create Editor instance
	mana3DEditorGUI = new EditorGuiManager;
	mana3DEditorGUI->SetEnterGameLoopCallbackFunc( EnterGameLoopCallback );

	mana3dEditorManager->SetEditorGuiManager( mana3DEditorGUI );

	//Debug::SetMANA3DEditorGUI( mana3DEditorGUI );

	// Construct default tags.
	ConstructDefaultTags();

	// Set on destroy texture callback function.
	MTexture::SetOnDestroyTextureCallbackFunc( OnDestroyMTexture );
	// Set on destroy mesh callback function.
	MMesh::SetOnDestroyMeshCallbackfunc( OnDestroyMMesh );

	// Set interface to maximize.
	Screen::Maximize();
}



void MANA3DEngine::MainLoop( GLFWwindow* window )
{
	while ( !glfwWindowShouldClose( window ) )
	{
		// Update time.
		if ( Time::Update() )
		{
			// Check whether we are in the editor loop or game loop.
			if ( isEditorLoop )
				EditorLoop();
			else
				GameLoop();

			// Swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			SwapBuffer( window );
		}
	}
}

void MANA3DEngine::EnterEditorLoop()
{
	isEditorLoop = true;
	framework->OnTerminate();
	mana3dEditorManager->SetEditorCamAsMain();
	Screen::SetFullScreen( false );
	Application::UnlockCursor();
}
void MANA3DEngine::EditorLoop()
{
	glEnable( GL_DEPTH_TEST );

	// ************************************
	glDepthFunc( GL_LESS );
	glEnable( GL_STENCIL_TEST );
	glStencilFunc( GL_NOTEQUAL, 1, 0xFF );
	glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
	// ************************************

	// ************************************
	glStencilMask( 0x00 );
	// ************************************

	// Start new GUI frame.
	ImGuiManager::StartFrame();

	// ************************************
	// Draw objects as normal, writing to the stencil buffer.
	glStencilFunc( GL_ALWAYS, 1, 0xFF );
	glStencilMask( 0xFF );
	// ************************************

	// Switch to our custom FBO
	glBindFramebuffer( GL_FRAMEBUFFER, frameBufferObject );

	// Update every single enabled component for all GameObjects in the scene.
	sceneManager->UpdateScene();
	mana3dEditorManager->Update();

	// ************************************ For selected object.
	Camera::ConfigGLForSelectedObjectPhase1();
	// ************************************ For selected object.


	// Render scene From current main camera.
	Camera::RenderFromEditorCamera();


	// Renderer Editor 3D Grid.
	mana3dEditorManager->SetMainGridColor();
	Camera::RenderFromMainCamera( mana3dEditorManager->GetMainGridGO() );
	mana3dEditorManager->SetCrossGridColor();
	Camera::RenderFromMainCamera( mana3dEditorManager->GetCrossGridGO() );

	// Render the skybox
	GraphicSettings::RenderSkybox();

	// ************************************ For selected object.
	Camera::ConfigGLForSelectedObjectPhase2( mana3DEditorGUI->currentSelectedGameObjectInScene, SceneManager::GetGameObjectOutline( mana3DEditorGUI->currentSelectedGameObjectInScene ) );
	// ************************************ For selected object.

	// Reset all input values.
	Input::Reset();

	// Switch to the default FBO
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	// Display Editor GUI
	mana3DEditorGUI->Display( textureColorBuffer, Renderer::GetDrawCallsCount(), MTexture::privateTexturesMap );

	// Render GUI
	ImGuiManager::Render();

	// ************************************
	glStencilMask( 0xFF );
	glEnable( GL_DEPTH_TEST );
	// ************************************

	if ( isEnterGameLoopTriggered )
		EnterGameLoop();
}

void MANA3DEngine::TriggerEnterGameLoop()
{
	isEnterGameLoopTriggered = true;
}
void MANA3DEngine::EnterGameLoop()
{
	isEditorLoop = false;
	Camera::SetMainCamera( nullptr );
	sceneManager->CloneSceneGameObjects();
	sceneManager->SetMainCamera();
	framework->OnStart();

	// Check if there is no main camera to render the scene.
	if ( Camera::GetCurrentMainCamera() == nullptr )
	{
		isEnterGameLoopTriggered = false;
		ExitGameLoop();
		EnterEditorLoop();

		Debug::LogError( "Error: Main Camera is NULL" );
		return;
	}

	//Screen::SetFullScreen( true );
}
void MANA3DEngine::GameLoop()
{
	if ( Input::GetKeyPressed( KeyCode::KC_Escape ) )
	{
		isEnterGameLoopTriggered = false;
		ExitGameLoop();
		EnterEditorLoop();
		return;
	}

	//glDisable( GL_FRAMEBUFFER_SRGB );
	glDisable( GL_STENCIL_TEST );
	glEnable( GL_DEPTH_TEST );

	// ************************************
	//glStencilMask( 0x00 );
	// ************************************

	// Start new GUI frame.
	ImGuiManager::StartFrame();

	//// ************************************
	//// Draw objects as normal, writing to the stencil buffer.
	//// --------------------------------------------------------------------
	//glStencilFunc( GL_ALWAYS, 1, 0xFF );
	//glStencilMask( 0xFF );
	//// ************************************

	// Switch to our custom FBO.
	glBindFramebuffer( GL_FRAMEBUFFER, GraphicSettings::GetIsMSAAEnabled() ? msaaFrameBufferObject : frameBufferObject );

	// Call Framework OnUpdate.
	framework->OnUpdate();

	// Update every single enabled component for all GameObjects in runtime.
	sceneManager->UpdateRuntime();

	// Render scene From current main camera.
	Camera::RenderFromRuntimeCamera();

	// Render the skybox.
	GraphicSettings::RenderSkybox();

	// Reset all input values.
	Input::Reset();

	// Switch to the default FBO.
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	// Renderer final image.
	if ( GraphicSettings::GetIsMSAAEnabled() )
		RenderMSAAFrameBuffer();
	else
		RenderFrameBuffer();

	// Render GUI
	ImGuiManager::Render();

	//// ************************************
	//glStencilMask( 0xFF );
	//glEnable( GL_DEPTH_TEST );
	//// ************************************
}
void MANA3DEngine::ExitGameLoop()
{
	sceneManager->ClearRuntimeGameObjects();
	Renderer::ClearRuntimeRenderers();
}


void MANA3DEngine::RenderFrameBuffer()
{
	glDisable( GL_DEPTH_TEST );
	glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );

	GraphicSettings::UsePPShader();

	glBindVertexArray( screenQuadVAO );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, textureColorBuffer );
	glDrawArrays( GL_TRIANGLES, 0, 6 );
}

void MANA3DEngine::RenderMSAAFrameBuffer()
{
	GLsizei width = (GLsizei)Screen::GetWidth();
	GLsizei height = (GLsizei)Screen::GetHeight();

    // Blit multisampled buffer(s) to normal colorbuffer of intermediate FBO. Image is stored in screenTexture.
    glBindFramebuffer( GL_READ_FRAMEBUFFER, msaaFrameBufferObject );
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, msaaIntermediateFrameBufferObject );
    glBlitFramebuffer( 0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    // Now render quad with scene's visuals as its texture image
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );
    glDisable(GL_DEPTH_TEST);

	GraphicSettings::UsePPShader();

    glBindVertexArray( screenQuadVAO );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, screenTexture ); // use the now resolved color attachment as the quad's texture
    glDrawArrays( GL_TRIANGLES, 0, 6 );
}


void MANA3DEngine::SwapBuffer( GLFWwindow* window )
{
	glfwSwapBuffers( window );
	glfwPollEvents();
}


void MANA3DEngine::OnTerminate()
{
	// Set the cursor to its normal state.
	Application::UnlockCursor();

	// ImGUI Cleanup.
	ImGuiManager::Shutdown();

	// Clear all objects that been created in the runtime framework.
	framework->OnTerminate();

	// Delete all manager instances.
	delete framework;
	delete mana3DEditorGUI;
	delete mana3dEditorManager;
	delete sceneManager;
	

	// Delete All MANA-Strutures
	GraphicSettings::DestroyAll();
	MMaterial::DestroyAll();
	MTexture::DestroyAll();
	MCubemap::DestroyAll();
	MMesh::DestroyAll();
	MSkinnedMesh::DestroyAll();
	MShader::DestroyAll();
	MPrimitive::DestroyAll();
	Skeleton::DestroyAll();

	// Terminate the app.
	glfwTerminate();
}




#pragma region

void MANA3DEngine::FramebufferSizeCallback( GLFWwindow* window, GLint width, GLint height )
{
	if ( width == 0 || height == 0 )
		return;
	Camera::SetViewPortSize( width, height );
	Screen::instance->width = width;
	Screen::instance->height = height;

	glViewport( 0, 0, width, height );
	GLdouble x0 = -(width / height);
	GLdouble x1 = width / height;
	glFrustum( x0, x1, -1, 1, Camera::GetCurrentMainCamera()->GetNearPlane(), Camera::GetCurrentMainCamera()->GetFarPlane() );
}

void MANA3DEngine::OnScrollCallback( GLFWwindow* window, double xoffset, double yoffset )
{
	ImGui_ImplGlfw_ScrollCallback( window, xoffset, yoffset );
	Input::ScrollCallback( window, xoffset, yoffset );
}

void MANA3DEngine::OnDestroyMTexture( MTexture* tex )
{
	MMaterial::RemoveTextureReferences( tex );
}
void MANA3DEngine::OnDestroyMMesh( MMesh* mesh )
{
	MeshRenderer::RemoveMeshReferences( mesh );
}


void MANA3DEngine::EnterGameLoopCallback()
{
	mana3DMain->TriggerEnterGameLoop();
}


void MANA3DEngine::CreateGizmo( GameObject* go, const MString& name )
{
	if ( go == nullptr )
		return;

	MeshRenderer* meshRenderer = go->AddComponent<MeshRenderer>();
	meshRenderer->SetMesh( MMesh::Get( "Rectangle" ) );
	meshRenderer->SetMaterial( MMaterial::GetPrivate( MString( "%sGizmoMat", name.GetCStr() ) ) );
	meshRenderer->SetFaceCullingType( 2 );
}


MANA3DEngine* MANA3DEngine::mana3DMain;

void MANA3DEngine::Create()
{
	if ( mana3DMain != nullptr ) 
	{
		Debug::LogWarning( "You cannot create another instance of MANA3DEngine!" );
		return;
	}

	mana3DMain = new MANA3DEngine;
	mana3DMain->Init();
}

void MANA3DEngine::Destroy()
{
	Screen::Destroy();
	delete mana3DMain;
}


bool MANA3DEngine::GetIsEditorLoop()
{
	return mana3DMain->isEditorLoop;
}

#pragma endregion [Static Functions]