#pragma once
#include <cmath>

namespace Willem
{

	struct Vector3
	{
		Vector3(float X, float Y, float Z)
			:x{ X }
			, y{ Y }
			, z{ Z }
		{}



		Vector3() {};
		float x, y, z;

		Vector3 operator+=(const Vector3& b);
		Vector3 operator-=(const Vector3& b);
	};


	struct Vector2
	{
		Vector2(float X, float Y)
			:x{ X }
			, y{ Y }
		{}
		Vector2(Vector3 v)
			:x{ v.x }
			, y{ v.y }
		{}

		Vector2() {};

		Vector2& Normalize()
		{
			float sum = sqrt(x * x + y * y);
			x = abs(x) / sum;
			y = abs(y) / sum;

			return *this;
		}
		float Magnitude()
		{
			float absX = abs(x);
			float absY = abs(y);

			return sqrt(absX * absX + absY * absY);			
		}

		float x, y;
	};

	Vector3 operator-(const Vector3& a, const Vector3& b);
	bool operator==(const Vector3& a, const Vector3& b);
	Vector3 operator*(const Vector3& a, float scalar);
	Vector3 operator+(const Vector3& a, const Vector3& b);



	Vector2 operator-(const Vector2& a, const Vector2& b);

	Vector2 operator+(const Vector2& a, const Vector2& b);

	Vector2 operator*(const Vector2& a, const Vector2& b);
	Vector2 operator*(const Vector2& a, float scalar);

}

