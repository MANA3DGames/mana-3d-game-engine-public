#include "Debug.h"
#include "EditorGuiLogWindow.h"
using namespace MANA3D;

using std::string;


void Debug::Log( int val, EditorLogType logType ) { EditorGuiLogWindow::AddLog( MString( "%i", val ), logType ); }
void Debug::Log( bool val, EditorLogType logType ) { EditorGuiLogWindow::AddLog( MString( "%s", val ? "True" : "False" ), logType ); }
void Debug::Log( size_t val, EditorLogType logType ) { EditorGuiLogWindow::AddLog( MString( "%i", (int)val ), logType ); }
void Debug::Log( float val, EditorLogType logType ) { EditorGuiLogWindow::AddLog( MString( "%.3f", val ), logType ); }
void Debug::Log( double val, EditorLogType logType ) { EditorGuiLogWindow::AddLog( MString( "%.3f", val ), logType ); }
void Debug::Log( glm::vec3 vec, EditorLogType logType ) { EditorGuiLogWindow::AddLog( MString( "%.3f, %.3f, %.3f", vec.x, vec.y, vec.z ), logType ); }
void Debug::Log( char* str, EditorLogType logType ) { EditorGuiLogWindow::AddLog( MString( "%s", str ), logType ); }
void Debug::Log( const char* str, EditorLogType logType ) { EditorGuiLogWindow::AddLog( MString( "%s", str ), logType ); }
void Debug::Log( string str, EditorLogType logType ) { EditorGuiLogWindow::AddLog( MString( "%s", str.c_str() ), logType ); }
void Debug::Log( MString str, EditorLogType logType ) { EditorGuiLogWindow::AddLog( MString( "%s", str.GetCStr() ), logType ); }


void Debug::LogWarning( int val ) { Log( val, EDITOR_LOG_TYPE_WARNING ); }
void Debug::LogWarning( bool val ) { Log( val, EDITOR_LOG_TYPE_WARNING ); }
void Debug::LogWarning( size_t val ) { Log( val, EDITOR_LOG_TYPE_WARNING ); }
void Debug::LogWarning( float val ) { Log( val, EDITOR_LOG_TYPE_WARNING ); }
void Debug::LogWarning( double val ) { Log( val, EDITOR_LOG_TYPE_WARNING ); }
void Debug::LogWarning( glm::vec3 vec ) { Log( vec, EDITOR_LOG_TYPE_WARNING ); }
void Debug::LogWarning( char* str ) { Log( str, EDITOR_LOG_TYPE_WARNING ); }
void Debug::LogWarning( const char* str ) { Log( str, EDITOR_LOG_TYPE_WARNING ); }
void Debug::LogWarning( std::string str ) { Log( str, EDITOR_LOG_TYPE_WARNING ); }
void Debug::LogWarning( MString str ) { Log( str, EDITOR_LOG_TYPE_WARNING ); }

void Debug::LogError( int val ) { Log( val, EDITOR_LOG_TYPE_ERROR ); }
void Debug::LogError( bool val ) { Log( val, EDITOR_LOG_TYPE_ERROR ); }
void Debug::LogError( size_t val ) { Log( val, EDITOR_LOG_TYPE_ERROR ); }
void Debug::LogError( float val ) { Log( val, EDITOR_LOG_TYPE_ERROR ); }
void Debug::LogError( double val ) { Log( val, EDITOR_LOG_TYPE_ERROR ); }
void Debug::LogError( glm::vec3 vec ) { Log( vec, EDITOR_LOG_TYPE_ERROR ); }
void Debug::LogError( char* str ) { Log( str, EDITOR_LOG_TYPE_ERROR ); }
void Debug::LogError( const char* str ) { Log( str, EDITOR_LOG_TYPE_ERROR ); }
void Debug::LogError( std::string str ) { Log( str, EDITOR_LOG_TYPE_ERROR ); }
void Debug::LogError( MString str ) { Log( str, EDITOR_LOG_TYPE_ERROR ); }