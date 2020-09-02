#include "Color.h"
using namespace MANA3D;

Color::Color()
{
	r = g = b = a = 1.0f;
}
Color::Color( const float& r, const float& g, const float& b )
{
	Color( r, g, b, 1.0f );
}
Color::Color( const float& r, const float& g, const float& b, const float& a )
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}



Color Color::White = Color( 1.0f, 1.0f, 1.0f, 1.0f );
Color Color::Black = Color( 0.0f, 0.0f, 0.0f, 1.0f );
Color Color::Red = Color( 1.0f, 0.0f, 0.0f, 1.0f );
Color Color::Green = Color( 0.0f, 1.0f, 0.0f, 1.0f );
Color Color::Blue = Color( 0.0f, 0.0f, 1.0f, 1.0f );
Color Color::Yellow = Color( 1.0f, 1.0f, 0.0f, 1.0f );
Color Color::DarkLime = Color( 0.0f, 0.5f, 0.0f, 1.0f );
Color Color::Cyan = Color( 0.0f, 1.0f, 1.0f, 1.0f );
Color Color::Magenta = Color( 1.0f, 0.0f, 1.0f, 1.0f );
Color Color::Silver = Color( 0.75f, 0.75f, 0.75f, 1.0f );
Color Color::Maroon = Color( 0.5f, 0.0f, 0.0f, 1.0f );
Color Color::Olive = Color( 0.5f, 0.5f, 0.0f, 1.0f );
Color Color::Purple = Color( 0.5f, 0.0f, 0.5f, 1.0f );
Color Color::Teal = Color( 0.0f, 0.5f, 0.5f, 1.0f );
Color Color::Navy = Color( 0.0f, 0.0f, 0.5f, 1.0f );