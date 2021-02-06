#pragma once

#include <iostream>

class Vector2D
{
public:
	float x;
	float y;

	Vector2D() :x(0.0f), y(0.0f) {}
	Vector2D(float x, float y) :x(x), y(y) {}
	Vector2D(Vector2D const & vec):x(vec.x),y(vec.y) {}

	Vector2D & Add(const Vector2D & vec);
	Vector2D & Subtract(const Vector2D & vec);
	Vector2D & Multiply(const Vector2D & vec);
	Vector2D & Divide(const Vector2D & vec);

	friend Vector2D & operator+(Vector2D & v1, const Vector2D & v2);
	friend Vector2D & operator-(Vector2D & v1, const Vector2D & v2);
	friend Vector2D & operator*(Vector2D & v1, const Vector2D & v2);
	friend Vector2D & operator/(Vector2D & v1, const Vector2D & v2);

	Vector2D & operator+=(const Vector2D & vec);
	Vector2D & operator-=(const Vector2D & vec);
	Vector2D & operator*=(const Vector2D & vec);
	Vector2D & operator/=(const Vector2D & vec);

	Vector2D& operator*(const int& i);
	Vector2D& Zero();

	Vector2D GetNormedVector();

	friend std::ostream& operator<<(std::ostream stream, const Vector2D & vec);
	friend bool operator==(const Vector2D &v1, const Vector2D &v2);
	friend bool operator!=(const Vector2D &v1, const Vector2D &v2);
	friend bool AlmostEquals(const Vector2D &v1, const Vector2D &v2); //Almost Equals (~=)
	friend bool NOTAlmostEquals(const Vector2D &v1, const Vector2D &v2); //(!~= )
};
