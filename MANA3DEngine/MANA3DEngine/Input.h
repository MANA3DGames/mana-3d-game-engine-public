#ifndef INPUT_H
#define INPUT_H

#include <glfw3.h>

namespace MANA3D
{
	enum KeyCode
	{
		KC_Unknown			= GLFW_KEY_UNKNOWN,

		/* Printable keys */
		KC_Space			= GLFW_KEY_SPACE,
		KC_Apostrophe		= GLFW_KEY_APOSTROPHE,
		KC_Comma			= GLFW_KEY_COMMA,
		KC_Minus			= GLFW_KEY_MINUS,
		KC_Period			= GLFW_KEY_PERIOD,  /* . */
		KC_Slash			= GLFW_KEY_SLASH, /* / */
		KC_Num0				= GLFW_KEY_0,
		KC_Num1				= GLFW_KEY_1,
		KC_Num2				= GLFW_KEY_2,
		KC_Num3				= GLFW_KEY_3,
		KC_Num4				= GLFW_KEY_4,
		KC_Num5				= GLFW_KEY_5,
		KC_Num6				= GLFW_KEY_6,
		KC_Num7				= GLFW_KEY_7,
		KC_Num8				= GLFW_KEY_8,
		KC_Num9				= GLFW_KEY_9,
		KC_Semicolon		= GLFW_KEY_SEMICOLON,  /* ; */
		KC_Equal			= GLFW_KEY_EQUAL,  /* = */
		KC_A				= GLFW_KEY_A,
		KC_B				= GLFW_KEY_B,
		KC_C				= GLFW_KEY_C,
		KC_D				= GLFW_KEY_D,
		KC_E				= GLFW_KEY_E,
		KC_F				= GLFW_KEY_F,
		KC_G				= GLFW_KEY_G,
		KC_H				= GLFW_KEY_H,
		KC_I				= GLFW_KEY_I,
		KC_J				= GLFW_KEY_J,
		KC_K				= GLFW_KEY_K,
		KC_L				= GLFW_KEY_L,
		KC_M				= GLFW_KEY_M,
		KC_N				= GLFW_KEY_N,
		KC_O				= GLFW_KEY_O,
		KC_P				= GLFW_KEY_P,
		KC_Q				= GLFW_KEY_Q,
		KC_R				= GLFW_KEY_R,
		KC_S				= GLFW_KEY_S,
		KC_T				= GLFW_KEY_T,
		KC_U				= GLFW_KEY_U,
		KC_V				= GLFW_KEY_V,
		KC_W				= GLFW_KEY_W,
		KC_X				= GLFW_KEY_X,
		KC_Y				= GLFW_KEY_Y,
		KC_Z				= GLFW_KEY_Z,
		KC_LeftBracket		= GLFW_KEY_LEFT_BRACKET,	/* [ */
		KC_Backslash		= GLFW_KEY_BACKSLASH,		/* \ */
		KC_RightBracket	= GLFW_KEY_RIGHT_BRACKET,	/* ] */
		KC_GraveAccent		= GLFW_KEY_GRAVE_ACCENT,	/* ` */
		KC_World1			= GLFW_KEY_WORLD_1,			/* non-US #1 */
		KC_World2			= GLFW_KEY_WORLD_2, /* non-US #2 */
		//

		/* Function keys */
		KC_Escape			= GLFW_KEY_ESCAPE,
		KC_Enter			= GLFW_KEY_ENTER,
		KC_Tab				= GLFW_KEY_TAB,
		KC_Backspace		= GLFW_KEY_BACKSPACE,
		KC_Insert			= GLFW_KEY_INSERT,
		KC_Delete			= GLFW_KEY_DELETE,
		KC_Right			= GLFW_KEY_RIGHT,
		KC_Left				= GLFW_KEY_LEFT,
		KC_Down				= GLFW_KEY_DOWN,
		KC_Up				= GLFW_KEY_UP,
		KC_PageUp			= GLFW_KEY_PAGE_UP,
		KC_PageDown			= GLFW_KEY_PAGE_DOWN,
		KC_Home				= GLFW_KEY_HOME,
		KC_End				= GLFW_KEY_END,
		KC_CapsLock			= GLFW_KEY_CAPS_LOCK,
		KC_ScrollLock		= GLFW_KEY_SCROLL_LOCK,
		KC_NumLock			= GLFW_KEY_NUM_LOCK,
		KC_PrintScreen		= GLFW_KEY_PRINT_SCREEN,
		KC_Pause			= GLFW_KEY_PAUSE,
		KC_F1				= GLFW_KEY_F1,
		KC_F2				= GLFW_KEY_F2,
		KC_F3				= GLFW_KEY_F3,
		KC_F4				= GLFW_KEY_F4,
		KC_F5				= GLFW_KEY_F5,
		KC_F6				= GLFW_KEY_F6,
		KC_F7				= GLFW_KEY_F7,
		KC_F8				= GLFW_KEY_F8,
		KC_F9				= GLFW_KEY_F9,
		KC_F10				= GLFW_KEY_F10,
		KC_F11				= GLFW_KEY_F11,
		KC_F12				= GLFW_KEY_F12,
		KC_F13				= GLFW_KEY_F13,
		KC_F14				= GLFW_KEY_F14,
		KC_F15				= GLFW_KEY_F15,
		KC_F16				= GLFW_KEY_F16,
		KC_F17				= GLFW_KEY_F17,
		KC_F18				= GLFW_KEY_F18,
		KC_F19				= GLFW_KEY_F19,
		KC_F20				= GLFW_KEY_F20,
		KC_F21				= GLFW_KEY_F21,
		KC_F22				= GLFW_KEY_F22,
		KC_F23				= GLFW_KEY_F23,
		KC_F24				= GLFW_KEY_F24,
		KC_KeyPad0			= GLFW_KEY_KP_0,
		KC_KeyPad1			= GLFW_KEY_KP_1,
		KC_KeyPad2			= GLFW_KEY_KP_2,
		KC_KeyPad3			= GLFW_KEY_KP_3,
		KC_KeyPad4			= GLFW_KEY_KP_4,
		KC_KeyPad5			= GLFW_KEY_KP_5,
		KC_KeyPad6			= GLFW_KEY_KP_6,
		KC_KeyPad7			= GLFW_KEY_KP_7,
		KC_KeyPad8			= GLFW_KEY_KP_8,
		KC_KeyPad9			= GLFW_KEY_KP_9,
		KC_KeyPadDecimal	= GLFW_KEY_KP_DECIMAL,
		KC_KeyPadDivide		= GLFW_KEY_KP_DIVIDE,
		KC_KeyPadMultiply	= GLFW_KEY_KP_MULTIPLY,
		KC_KeyPadSubtract	= GLFW_KEY_KP_SUBTRACT,
		KC_KeyPadAdd		= GLFW_KEY_KP_ADD,
		KC_KeyPadEnter		= GLFW_KEY_KP_ENTER,
		KC_KeyPadEqual		= GLFW_KEY_KP_EQUAL,
		KC_LeftShift		= GLFW_KEY_LEFT_SHIFT,
		KC_LeftControl		= GLFW_KEY_LEFT_CONTROL,
		KC_LeftAlt			= GLFW_KEY_LEFT_ALT,
		KC_LeftSuper		= GLFW_KEY_LEFT_SUPER,
		KC_RightShift		= GLFW_KEY_RIGHT_SHIFT,
		KC_RightControl		= GLFW_KEY_RIGHT_CONTROL,
		KC_RightAlt			= GLFW_KEY_RIGHT_ALT,
		KC_RightSuper		= GLFW_KEY_RIGHT_SUPER,
		KC_Menu				= GLFW_KEY_MENU,
		KC_MouseRight		= GLFW_MOUSE_BUTTON_RIGHT,
		KC_MouseLeft		= GLFW_MOUSE_BUTTON_LEFT,
		KC_MouseMiddle		= GLFW_MOUSE_BUTTON_MIDDLE
	};

	class Input
	{
		friend class MANA3DEngine;

	private: // Statics
		static void SetWindow( GLFWwindow* window );

		static void ScrollCallback( GLFWwindow* window, double xoffset, double yoffset );
		static void MouseCallback( GLFWwindow* window, double xpos, double ypos );
		static void MouseButtonCallback( GLFWwindow* window, int button, int action, int mods );

		static void Reset();


		static GLFWwindow* window;

		static double scrollXOffset;
		static double scrollYOffset;

		static double mouseXPos;
		static double mouseYPos;

		static bool isMouseRight;
		static bool isMouseLeft;
		static bool isMouseMiddle;

	public: // Statics
		static bool GetKeyPressed( KeyCode code );
		static bool GetKeyReleased( KeyCode code );

		static double GetScrollX();
		static double GetScrollY();

		static double GetMouseXPos();
		static double GetMouseYPos();

		static bool GetMouseRightClick();
		static bool GetMouseLeftClick();
		static bool GetMouseMiddleClick();

	};
}

#endif // !INPUT_H
