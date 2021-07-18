#pragma once

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
		float x, y;
	};

	Vector3 operator-(const Vector3& a, const Vector3& b);
	bool operator==(const Vector3& a, const Vector3& b);
	Vector3 operator*(const Vector3& a, float scalar);
	Vector2 operator-(const Vector2& a, const Vector2& b);

	Vector2 operator+(const Vector2& a, const Vector2& b);

	Vector2 operator*(const Vector2& a, const Vector2& b);
	Vector2 operator*(const Vector2& a, float scalar);

}

