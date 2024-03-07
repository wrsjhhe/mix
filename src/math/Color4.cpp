#include <math/Color4.h>

using namespace mix;


Color4::Color4(float r, float g, float b, float a /*= 1*/) :Color(r, g, b), a(a)
{

}


Color4& Color4::Color4::set(float r, float g, float b, float a /*= 1*/)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;

	return *this;
}

Color4& Color4::Color4::copy(const Color4& color)
{
	this->r = color.r;
	this->g = color.g;
	this->b = color.b;
	this->a = color.a;
	return *this;

}

Color4 Color4::Color4::clone()
{
	return Color4(r, g, b, a);
}