#ifndef MANA3D_MSTRING_H
#define MANA3D_MSTRING_H

#include <windows.h>
#include <cwchar>
//#include <math.h>
#include <comdef.h>
#include <string>
#include <vector>
//#include <glm\glm.hpp>


namespace MANA3D
{
	class MString
	{
	private:
		std::string str;

	public:
		MString();
		MString( std::string str );
		MString( char *str );
		MString( const wchar_t* );
		MString( const char* fmt, ... );

		~MString();



		bool operator==( const MString &rh ) const;
		bool operator==( const char *str ) const;

		bool operator!=( const MString &rh ) const;
		bool operator!=( const char *str ) const;

		operator const char*() const;
		operator LPWSTR() const;
		operator const wchar_t*() const;


		const std::string GetStr() const;
		const char* GetCStr() const;
		char* GetCharArray() const;

		char* ToLower();

		bool IsEmpty();

		void Clear();

		template <typename T>
		void Append( T t )
		{
			str.append( t );
		}

		int GetLength();


		// trim from start (in place)
		void TrimStart();
		// trim from end (in place)
		void TrimEnd();
		// trim from both ends (in place)
		void Trim();

	public:
		static LPWSTR ToLPWSTR( const char* str );
		static const wchar_t* ToWChar_T( const char *str );
		static char* ToCharPtr( const wchar_t* wChar );
		static char* ToLower( const char* str );

		static MString GetSubStr( const MString & mainStr, const size_t startIndex, const size_t count );
		static MString EraseSubStr( const MString & mainStr, const MString & toErase );
		static MString EraseSubStrToFirst( const MString & mainStr, const int startIndex, const char toFirstFound );
		static MString EraseSubStrToLast( const MString & mainStr, const int startIndex, const char toLastFound );
		static MString EraseSubStr( const MString & mainStr, const size_t startIndex, const size_t endIndex );
		static size_t GetFirstFoundIndex( const MString & Str, const char firstFoundChar );
		static size_t GetLastFoundIndex( const MString & Str, const char lastFoundChar );

		static std::vector<std::string> SplitString( MString str, char c );
		static std::vector<MString> SplitMString( MString str, char c );
	};
}

#endif
