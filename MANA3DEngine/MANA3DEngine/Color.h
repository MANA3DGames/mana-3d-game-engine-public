#ifndef COLOR_H
#define COLOR_H

namespace MANA3D
{
	class Color
	{
	public:
		Color();
		Color( const float& r, const float& g, const float& b );
		Color( const float& r, const float& g, const float& b, const float& a );

		float r;
		float g;
		float b;
		float a;

	public: // Static 

		static Color White;
		static Color Black;
		static Color Red;
		static Color Green;
		static Color Blue;
		static Color Yellow;
		static Color DarkLime;
		static Color Cyan;
		static Color Magenta;
		static Color Silver;
		static Color Maroon;
		static Color Olive;
		static Color Purple;
		static Color Teal;
		static Color Navy;
	};
}

#endif // !COLOR_H
