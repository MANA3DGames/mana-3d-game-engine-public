#define _CRT_SECURE_NO_WARNINGS
#include "MString.h"
using namespace MANA3D;

#include <stdlib.h>		/* for malloc, NULL, size_t */
#include <stdarg.h>		/* for va_ stuff */

#include <sstream>
#include <algorithm>

using std::vector;
using std::string;
//using namespace glm;



MString::MString()
{

}

MString::MString( string gStr )
{
	str = gStr;
}

MString::MString( char *gStr )
{
	str.append( gStr );
}

//MString::MString( const char *gStr )
//{
//	str.append( gStr );
//}

MString::MString( const wchar_t *gStr )
{
	_bstr_t newStr( gStr );
	str.append( (char*)newStr );

	//size_t   i;
	//char str[1024];
	////wcstombs( str, my_documents, 1023 );
	//wcstombs_s( &i, str, my_documents, 1023 );
}

MString::MString( const char* fmt, ... )
{
	va_list args;
	char buffer[256];
	va_start( args, fmt );
	int len = _vscprintf( fmt, args ) + 1;
	vsprintf_s( buffer, len, fmt, args );
	str.append( buffer );
	va_end( args );
}


//int ImFormatStringV(char* buf, size_t buf_size, const char* fmt, va_list args)
//{
//#ifdef IMGUI_USE_STB_SPRINTF
//	int w = stbsp_vsnprintf(buf, (int)buf_size, fmt, args);
//#else
//	int w = vsnprintf(buf, buf_size, fmt, args);
//#endif
//	if (buf == NULL)
//		return w;
//	if (w == -1 || w >= (int)buf_size)
//		w = (int)buf_size - 1;
//	buf[w] = 0;
//	return w;
//}
//MString::MString( const char* fmt, ... )
//{
//	va_list args;
//	va_start( args, fmt );
//
//	ImGuiContext& g = *GImGui;
//	const char* text_end = g.TempBuffer + ImFormatStringV( g.TempBuffer, IM_ARRAYSIZE( g.TempBuffer ), fmt, args );
//
//	va_end( args );
//}



MString::~MString()
{
}


bool MString::operator==( const MString &rh ) const
{
	return str == rh.str;
}
bool MString::operator==( const char *rh ) const
{
	return str == rh;
}

bool MString::operator!=( const MString &rh ) const
{
	return str != rh.str;
}
bool MString::operator!=( const char *rh ) const
{
	return str != rh;
}



//MString MString::operator+( const char *cChr ) const
//{
//	MString temp( str );
//	temp.Append( cChr );
//	return MString( temp );
//}
//MString MString::operator+=( const char *cChr )
//{
//	str.append( cChr );
//	return *this;
//}
//MString MANA3D::operator+(const char* lhs, const MString& rhs)
//{
//	return MString( lhs ) + rhs;
//}
//
//MString MString::operator+( string sStr ) const
//{
//	MString temp( str );
//	temp.Append( sStr );
//	return MString( temp );
//}
//MString MString::operator+=( string sStr )
//{
//	str.append( sStr );
//	return *this;
//}
//
//MString MString::operator+( MString sStr ) const
//{
//	MString temp( str );
//	temp.Append( sStr );
//	return MString( temp );
//}
//MString MString::operator+=( MString sStr )
//{
//	str.append( sStr );
//	return *this;
//}
//
//MString MString::operator+( int value ) const
//{
//	MString temp( str );
//	char buffer[256];
//	sprintf_s( buffer, sizeof(buffer), "%i", value );
//	temp.Append( buffer );
//	return temp;
//}
//MString MString::operator+=( int value )
//{
//	char buffer[256];
//	sprintf_s( buffer, sizeof(buffer), "%i", value );
//	str.append( buffer );
//	return *this;
//}
//
//MString MString::operator+( float value ) const
//{
//	MString temp( str );
//	char buffer[256];
//	sprintf_s( buffer, sizeof(buffer), "%f", value );
//	temp.Append( buffer );
//	return temp;
//}
//MString MString::operator+=( float value )
//{
//	char buffer[256];
//	sprintf_s( buffer, sizeof(buffer), "%f", value );
//	str.append( buffer );
//	return *this;
//}
//
//
//MString MString::operator+( long value ) const
//{
//	MString temp( str );
//	char buffer[256];
//	sprintf_s( buffer, sizeof(buffer), "%ld", value );
//	temp.Append( buffer );
//	return temp;
//}
//MString MString::operator+=( long value )
//{
//	char buffer[256];
//	sprintf_s( buffer, sizeof(buffer), "%ld", value );
//	str.append( buffer );
//	return *this;
//}
//
//
//MString MString::operator+( double value ) const
//{
//	MString temp( str );
//	char buffer[256];
//	sprintf_s( buffer, sizeof(buffer), "%f", value );
//	temp.Append( buffer );
//	return temp;
//}
//MString MString::operator+=( double value )
//{
//	char buffer[256];
//	sprintf_s( buffer, sizeof(buffer), "%f", value );
//	str.append( buffer );
//	return *this;
//}
//
//
//MString MString::operator+( vec3 vec ) const
//{
//	MString temp( str );
//	char buffer[256];
//	sprintf_s( buffer, sizeof(buffer), "(%f, %f, %f)", vec.x, vec.y, vec.z );
//	temp.Append( buffer );
//	return temp;
//}
//MString MString::operator+=( vec3 vec )
//{
//	char buffer[256];
//	sprintf_s( buffer, sizeof(buffer), "(%f, %f, %f)", vec.x, vec.y, vec.z );
//	str.append( buffer );
//	return *this;
//}


MString::operator const char*() const
{
	return str.c_str();
}

MString::operator LPWSTR() const
{
	return ToLPWSTR( str.c_str() );
}

MString::operator const wchar_t*() const
{
	return ToWChar_T( str.c_str() );
}

const string MString::GetStr() const
{
	return str;
}

const char* MString::GetCStr() const
{
	return str.c_str();
}

char* MString::GetCharArray() const
{
	//char * my_other_str = strdup(some_const_str);
	return (char*)str.c_str();
}


char* MString::ToLower()
{
	return ToLower( str.c_str() );
}

bool MString::IsEmpty()
{
	return str.empty();
}

void MString::Clear()
{
	str.clear();
}

int MString::GetLength()
{
	return (int)str.length();
}



void MString::TrimStart()
{
	str.erase(	str.begin(), 
				std::find_if( str.begin(), str.end(), [](int ch) { return !std::isspace(ch); } ) );
}
void MString::TrimEnd()
{
	str.erase(	std::find_if( str.rbegin(), str.rend(), [](int ch) { return !std::isspace(ch); } ).base(), 
				str.end());
}

void MString::Trim()
{
	TrimStart();
	TrimEnd();
}

//// trim from start (in place)
//static inline void ltrim(std::string &s) {
//    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
//        return !std::isspace(ch);
//    }));
//}
//
//// trim from end (in place)
//static inline void rtrim(std::string &s) {
//    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
//        return !std::isspace(ch);
//    }).base(), s.end());
//}
//
//// trim from both ends (in place)
//static inline void trim(std::string &s) {
//    ltrim(s);
//    rtrim(s);
//}
//
//// trim from start (copying)
//static inline std::string ltrim_copy(std::string s) {
//    ltrim(s);
//    return s;
//}
//
//// trim from end (copying)
//static inline std::string rtrim_copy(std::string s) {
//    rtrim(s);
//    return s;
//}
//
//// trim from both ends (copying)
//static inline std::string trim_copy(std::string s) {
//    trim(s);
//    return s;
//}




// ************** Statics ************** 
LPWSTR MString::ToLPWSTR( const char* gStr )
{
	size_t size = strlen( gStr ) + 1;
	wchar_t* wChar = new wchar_t[size];
	size_t outSize;
	mbstowcs_s( &outSize, wChar, size, gStr, size - 1 );
	LPWSTR ptr = wChar;
	return ptr;
}

const wchar_t* MString::ToWChar_T( const char *cStr )
{
	const size_t cSize = strlen( cStr ) + 1;
	wchar_t wc[256];
	mbstowcs( wc, cStr, cSize );
	return wc;
}

char* MString::ToCharPtr( const wchar_t* wChar )
{
	_bstr_t newStr( wChar );
	return (char*)newStr;
}

char* MString::ToLower( const char* cStr )
{
	char* temp = _strdup(cStr);
	for (int i = 0; temp[i] != '\0'; i++)
		temp[i] = tolower(temp[i]);
	return temp;
}




MString MString::GetSubStr( const MString & mainStr, const size_t startIndex, const size_t count )
{
	return mainStr.str.substr( startIndex, count );
}


MString MString::EraseSubStr( const MString & mainStr, const MString & toErase )
{
	string tempStr = mainStr;

	// Search for the substring in string.
	size_t index = tempStr.find( toErase );

	if ( index != std::string::npos )
	{
		// If found it then erase it from string.
		tempStr.erase( index, toErase.str.length() );
	}

	return tempStr;
}

MString MString::EraseSubStrToFirst( const MString & mainStr, const int startIndex, const char toFirstFound )
{
	return EraseSubStr( mainStr, startIndex, GetFirstFoundIndex( mainStr, toFirstFound ) );
}

MString MString::EraseSubStrToLast( const MString & mainStr, const int startIndex, const char toLastFound )
{
	return EraseSubStr( mainStr, startIndex, GetLastFoundIndex( mainStr, toLastFound ) );
}

MString MString::EraseSubStr( const MString & mainStr, const size_t startIndex, const size_t endIndex )
{
	string tempStr = mainStr;

	if ( startIndex != std::string::npos && endIndex != std::string::npos )
		tempStr.erase( startIndex, endIndex );

	return tempStr;
}


size_t MString::GetFirstFoundIndex( const MString & Str, const char firstFoundChar )
{
	return Str.GetStr().find_first_of( firstFoundChar );
}

size_t MString::GetLastFoundIndex( const MString & Str, const char lastFoundChar )
{
	return Str.GetStr().find_last_of( lastFoundChar );
}


//vector<int> MString::SplitInt( MString str, char c )
//{
//	std::replace( str.GetStr().begin(), str.GetStr().end(), c, ' ' );  // replace c by ' '
//
//	vector<int> array;
//	std::stringstream ss( str.GetStr() );
//	int temp;
//	while ( ss >> temp )
//		array.push_back( temp );
//
//	return array;
//}
vector<string> MString::SplitString( MString str, char c )
{
	std::stringstream temp( str.GetStr() );
	std::string segment;
	std::vector<std::string> seglist;

	while ( std::getline( temp, segment, c ) )
		seglist.push_back( segment );

	return seglist;
}
vector<MString> MString::SplitMString( MString str, char c )
{
	std::stringstream temp( str.GetStr() );
	std::string segment;
	std::vector<MString> seglist;

	while ( std::getline( temp, segment, c ) )
		seglist.push_back( segment );

	return seglist;
}