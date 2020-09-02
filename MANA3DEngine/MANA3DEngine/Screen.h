#ifndef SCREEN_H
#define SCREEN_H

#include <array>
#include <glfw3.h>

namespace MANA3D
{
	class Screen
	{
		friend class MANA3DEngine;

	private:
		Screen();
		~Screen();

		int width;
		int height;

		GLFWwindow* window;
		GLFWmonitor* monitor;

		std::array<int, 2> _wndPos{ 0, 0 };
		std::array<int, 2> _wndSize{ 0, 0 };
		std::array<int, 2> _vpSize{ 0, 0 };

	private:
		static Screen* instance;

		static void Create( GLFWwindow* window, const int& width, const int& height );
		static void Destroy();

		static void SetSize( const int& width, const int& height );

	public:
		static int GetWidth();
		static int GetHeight();
		static void Maximize();

		static bool IsFullscreen( void );
		static void SetFullScreen( const bool& fullscreen );
	};
}

#endif // !SCREEN_H
