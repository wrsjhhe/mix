#include <math/Vector3.h>

using namespace mix;

Vector3::Vector3() noexcept :m_impl(0.f, 0.f, 0.f)
{

}

Vector3::Vector3(float ix) : m_impl(ix, ix, ix)
{

}

Vector3::Vector3(float ix, float iy, float iz) noexcept : m_impl(ix, iy, iz)
{

}

Vector3::Vector3(_In_reads_(3) const float* pArray) noexcept : m_impl(pArray)
{

}

Vector3::Vector3(const Vector3& v) noexcept
{
	m_impl.x = v.m_impl.x;
	m_impl.y = v.m_impl.y;
	m_impl.y = v.m_impl.y;
}

float Vector3::x() const
{
	return this->m_impl.x;
}

float Vector3::y() const
{
	return this->m_impl.y;
}

float Vector3::z() const
{
	return this->m_impl.z;
}

Vector3& Vector3::setX(float value)
{
	m_impl.x = value;
	return *this;
}

Vector3& Vector3::setY(float value)
{
	m_impl.y = value;
	return *this;
}

Vector3& Vector3::setZ(float value)
{
	m_impl.z = value;
	return *this;
}

Vector3& Vector3::add(const Vector3& v)
{
	const XMVECTOR v1 = XMLoadFloat3(&m_impl);
	const XMVECTOR v2 = XMLoadFloat3(&v.m_impl);
	const XMVECTOR X = XMVectorAdd(v1, v2);
	XMStoreFloat3(&m_impl, X);
	//m_impl.x += v.m_impl.x;
	//m_impl.y += v.m_impl.y;
	//m_impl.z += v.m_impl.z;
	
	return *this;
}

Vector3& Vector3::sub(const Vector3& v)
{
	const XMVECTOR v1 = XMLoadFloat3(&m_impl);
	const XMVECTOR v2 = XMLoadFloat3(&v.m_impl);
	const XMVECTOR X = XMVectorSubtract(v1, v2);
	XMStoreFloat3(&m_impl, X);
	//m_impl.x -= v.m_impl.x;
	//m_impl.y -= v.m_impl.y;
	//m_impl.z -= v.m_impl.z;
	
	return *this;
}
