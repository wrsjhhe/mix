#pragma once

#include <math/Color.h>
namespace mix {
	class Color4 : public Color {
	public:
		float a{ 1.f };

		Color4() = default;

		Color4(float r, float g, float b, float a = 1);

		Color4& set(float r, float g, float b, float a = 1);

		Color4& copy(const Color4& color);

		Color4 clone();

	};
}