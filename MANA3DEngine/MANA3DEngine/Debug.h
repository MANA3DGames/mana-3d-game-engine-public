#ifndef DEBUG_H
#define DEBUG_H

#include "MString.h"
#include "Math3D.h"

namespace MANA3D
{
	enum EditorLogType
	{
		EDITOR_LOG_TYPE_INFO = 0,
		EDITOR_LOG_TYPE_WARNING = 1,
		EDITOR_LOG_TYPE_ERROR = 2
	};

	class EditorGuiManager;

	class Debug
	{
	public:
		static void Log( int val, EditorLogType logType = EDITOR_LOG_TYPE_INFO );
		static void Log( bool val, EditorLogType logType = EDITOR_LOG_TYPE_INFO );
		static void Log( size_t val, EditorLogType logType = EDITOR_LOG_TYPE_INFO );
		static void Log( float val, EditorLogType logType = EDITOR_LOG_TYPE_INFO );
		static void Log( double val, EditorLogType logType = EDITOR_LOG_TYPE_INFO );
		static void Log( glm::vec3 vec, EditorLogType logType = EDITOR_LOG_TYPE_INFO );
		static void Log( char* str, EditorLogType logType = EDITOR_LOG_TYPE_INFO );
		static void Log( const char* str, EditorLogType logType = EDITOR_LOG_TYPE_INFO );
		static void Log( std::string str, EditorLogType logType = EDITOR_LOG_TYPE_INFO );
		static void Log( MString str, EditorLogType logType = EDITOR_LOG_TYPE_INFO );

		static void LogWarning( int val );
		static void LogWarning( bool val );
		static void LogWarning( size_t val );
		static void LogWarning( float val );
		static void LogWarning( double val );
		static void LogWarning( glm::vec3 vec );
		static void LogWarning( char* str );
		static void LogWarning( const char* str );
		static void LogWarning( std::string str );
		static void LogWarning( MString str );

		static void LogError( int val );
		static void LogError( bool val );
		static void LogError( size_t val );
		static void LogError( float val );
		static void LogError( double val );
		static void LogError( glm::vec3 vec );
		static void LogError( char* str );
		static void LogError( const char* str );
		static void LogError( std::string str );
		static void LogError( MString str );
	};
}


#endif // !DEBUG_H

