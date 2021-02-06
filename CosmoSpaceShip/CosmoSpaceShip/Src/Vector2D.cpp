#include "Vector2D.h"

Vector2D & Vector2D::Add(const Vector2D & vec)
{
	x += vec.x;
	y += vec.y;
	return *this;
}

Vector2D & Vector2D::Subtract(const Vector2D & vec)
{
	x -= vec.x;
	y -= vec.y;
	return *this;
}

Vector2D & Vector2D::Multiply(const Vector2D & vec)
{
	x *= vec.x;
	y *= vec.y;
	return *this;
}

Vector2D & Vector2D::Divide(const Vector2D & vec)
{
	x /= vec.x;
	y /= vec.y;
	return *this;
}

Vector2D & Vector2D::operator+=(const Vector2D & vec)
{
	return (*this).Add(vec);
}

Vector2D & Vector2D::operator-=(const Vector2D & vec)
{
	return (*this).Subtract(vec);
}

Vector2D & Vector2D::operator*=(const Vector2D & vec)
{
	return (*this).Multiply(vec);
}

Vector2D & Vector2D::operator/=(const Vector2D & vec)
{
	return (*this).Divide(vec);
}

Vector2D & Vector2D::operator*(const int & i)
{
	x *= i;
	y *= i;

	return *this;
}

Vector2D & Vector2D::Zero()
{
	x = 0;
	y = 0;

	return *this;
}

Vector2D Vector2D::GetNormedVector()
{
	float norming_coeff = sqrt(x * x + y * y);
	return Vector2D(x / norming_coeff, y / norming_coeff);
}

Vector2D & operator+(Vector2D & v1, const Vector2D & v2)
{
	return v1.Add(v2);
}

Vector2D & operator-(Vector2D & v1, const Vector2D & v2)
{
	return v1.Subtract(v2);
}
Vector2D & operator*(Vector2D & v1, const Vector2D & v2)
{
	return v1.Multiply(v2);
}
Vector2D & operator/(Vector2D & v1, const Vector2D & v2)
{
	return v1.Divide(v2);
}

std::ostream & operator<<(std::ostream stream, const Vector2D & vec)
{
	stream << "(" << vec.x << "," << vec.y << ")" << std::endl;
	return stream;
}

bool operator==(const Vector2D & v1, const Vector2D & v2)
{
	if (v1.x == v2.x && v1.y == v2.y)
		return true;
	else
		return false;
}

bool operator!=(const Vector2D & v1, const Vector2D & v2)
{
	return !(v1 == v2);
}

bool AlmostEquals(const Vector2D & v1, const Vector2D & v2)
{
	if (abs(v1.x-v2.x)<=3 && abs(v1.y - v2.y) <= 3)
		return true;
	else
		return false;
}

bool NOTAlmostEquals(const Vector2D & v1, const Vector2D & v2)
{
	return !(AlmostEquals(v1, v2));
}


