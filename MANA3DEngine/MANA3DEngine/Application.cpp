#include "Application.h"
#include "Debug.h"
using namespace MANA3D;

#include <windows.h>
#include <algorithm>
#include <dirent.h>
#include <filesystem> 
namespace fs = std::filesystem;

using std::list;
using std::vector;
using std::string;


GLFWwindow* Application::window;



MString Application::GetAppFullPath()
{
	char result[MAX_PATH];
	return std::string( result, GetModuleFileName( nullptr, result, MAX_PATH ) );
}

MString Application::GetAppName()
{
	char szAppPath[MAX_PATH] = "";
	std::string strAppName;

	::GetModuleFileName( 0, szAppPath, MAX_PATH );

	// Extract name
	strAppName = szAppPath;
	strAppName = strAppName.substr( strAppName.rfind( "\\" ) + 1 );
	return strAppName;
}

MString Application::GetAppDirctory()
{
	string fullPath = GetAppFullPath();
	int appNameLength = GetAppName().GetLength();
	
	fullPath = fullPath.substr( 0, fullPath.length() - appNameLength );

	return fullPath;
}

vector<MString> Application::GetFileNames( const MString &path )
{
	DIR *dir;
	struct dirent *ent;

	vector<MString> allFiles;

	if ( ( dir = opendir( path ) ) != nullptr ) 
	{
		/* print all the files and directories within directory */
		while ( ( ent = readdir( dir ) ) != nullptr ) 
			allFiles.push_back( ent->d_name );

		// Remove the first two elements:
		allFiles.erase( std::remove( allFiles.begin(), allFiles.end(), MString( "." ) ), allFiles.end() );
		allFiles.erase( std::remove( allFiles.begin(), allFiles.end(), MString( ".." ) ), allFiles.end() );

		closedir( dir );
	}
	else 
	{
		/* could not open directory */
	}

	return allFiles;
}

vector<MFile> Application::GetAllFiles( const MString &path, MFileFilter filter )
{
	DIR *dir;
	struct dirent *ent;

	vector<MFile> allFiles;

	if ( ( dir = opendir( path ) ) != nullptr )
	{
		/* All the files and directories within directory */
		while ( ( ent = readdir( dir ) ) != nullptr )
		{
			int type = -1;

			MString currentPath( "%s%s", path.GetCStr(), ent->d_name );

			try {
				// Create a Path object from given path string
				fs::path pathObj( currentPath.GetStr() );
				
				// Check if path exists and is of a regular file
				if ( fs::exists( pathObj ) )
				{
					if ( fs::is_regular_file( pathObj ) )
						type = 0;
					if ( fs::is_directory( pathObj ) )
						type = 1;
				}
			}
			catch ( fs::filesystem_error & e )
			{
				//std::cerr << e.what() << std::endl;
				Debug::LogError( e.what() );
			}


			if ( filter == MFileFilter::MFF_All && type != -1 ||
				 ( filter == MFileFilter::MFF_File && type == 0 ) ||
				 ( filter == MFileFilter::MFF_Directory && type == 1 ) )
			{
				MFile file;
				file.name = ent->d_name;
				file.path = path;
				file.type = type;

				allFiles.push_back( file );
			}
		}

		closedir( dir );
	}
	else
	{
		/* could not open directory */
	}

	// Remove the first two elements [.] [..]
	if ( allFiles.begin() != allFiles.end() )
	{
		if ( allFiles[0].name == "." || allFiles[0].name == ".." )
			allFiles.erase( allFiles.begin() );
		if ( allFiles[0].name == "." || allFiles[0].name == ".." )
			allFiles.erase( allFiles.begin() );
	}

	return allFiles;
}

list<string> Application::GetDrives()
{
	list<string> driveList;

	const int MAX = 256;
	int dr_type = 99;
	char dr_avail[MAX];
	char *temp = dr_avail;

	// Now we'll fill our buffer, as well as indicate the max size of the buffer. Since we defined MAX to be 256 earlier
	GetLogicalDriveStrings( MAX, dr_avail );

	// Now dr_avail, defined as char, will contain a Null terminated string of all of our drive letters. 
	// Now we'll parse that into the pointer at each Null, & then deal with them individually. 
	// Once they are seperated into the Temp pointer, we will pass that pointer to GetDriveType. 
	// Since the string already holds the format that GetDriveType requires, we don't need to alter anything. 
	// GetDriveType requires the drive letter, the colon, & it must have the backspace.
	while ( *temp != NULL )  // Split the buffer by null
	{ 
		dr_type = GetDriveType( temp );
		switch ( dr_type ) 
		{
			case 0: // Unknown Drive type
			break;
			case 1: // Drive is invalid
			break;
			case 2: // Removable Drive
				driveList.push_back( temp );
			break;
			case 3: // Hard Disk (Fixed)
				driveList.push_back( temp );
			break;
			case 4: // Remote (Network) Drive
			break;
			case 5: // CD-Rom/DVD-Rom
			break;
			case 6: // Ram Drive
			break;
		}
		temp += lstrlen( temp ) + 1; // incriment the buffer
	}

	return driveList;
}



void Application::SetWindow( GLFWwindow* window )
{
	Application::window = window;
}

void Application::LockCursor()
{
	glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
}
void Application::UnlockCursor()
{
	glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
}
void Application::HideCursor()
{
	glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN );
}

void Application::Quit()
{
	glfwSetWindowShouldClose( window, true );
}