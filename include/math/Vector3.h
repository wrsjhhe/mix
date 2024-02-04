#pragma once

#include <DirectXMath.h>


namespace mix
{
	using namespace DirectX;
	class Vector3
	{
	public:
		Vector3() noexcept;
		explicit Vector3(float ix);
		Vector3(float ix, float iy, float iz) noexcept;
		explicit Vector3(_In_reads_(3) const float* pArray) noexcept;
		Vector3(const Vector3& V) noexcept;

		float x() const;
		float y() const;
		float z() const;

		Vector3& setX(float value);

		Vector3& setY(float value);

		Vector3& setZ(float value);

		Vector3& add(const Vector3& v);

		Vector3& sub(const Vector3& v);

	private:

		XMFLOAT3 m_impl;
	};
}