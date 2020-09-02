#ifndef APPLICATION_H
#define APPLICATION_H

#include <glfw3.h>
#include <vector>
#include <list>
#include <sys/stat.h> 

#include "MFile.h"

namespace MANA3D
{
	class Application
	{
		friend class MANA3DEngine;

	private: // Statics
		static void SetWindow( GLFWwindow* window );

		static GLFWwindow* window;

	public: // Statics 
		static MString GetAppFullPath();
		static MString GetAppName();
		static MString GetAppDirctory();
		static std::list<std::string> GetDrives();


		static std::vector<MString> GetFileNames( const MString &path ); 
		static std::vector<MFile> GetAllFiles( const MString &path, MFileFilter filter = MFileFilter::MFF_All );

		static void LockCursor();
		static void UnlockCursor();
		static void HideCursor();

		static void Quit();

	};

	
}

#endif // APPLICATION_H
