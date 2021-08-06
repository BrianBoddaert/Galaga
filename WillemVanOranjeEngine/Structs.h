#pragma once
#include <cmath>

#define PI 3.14159265358979323846264338327950288f
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

		Vector2 Normalize() const
		{
			float sum = Magnitude();
			float X = x;
			float Y = y;

			X = abs(X) / sum;
			Y = abs(Y) / sum;

			if (x < 0)
				X *= -1;

			if (y < 0)
				Y *= -1;

			return Vector2(X,Y);
		}
		float Magnitude() const
		{
			float absX = abs(x);
			float absY = abs(y);

			return sqrt(absX * absX + absY * absY);			
		}

		float DirectionToLocalOrientationAngle() const
		{
			float angle = atan2(y, x);
			// My angle to sprite can't handle negative numbers so I do this.
			if (angle < 0)
				angle = float((PI * 2.0) - abs(angle));
			// This is done because the angle 0 is set upwards instead of to the right. ( angle 0 starts at -90)
			angle += float(PI / 2.0);
			return angle;
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

