#pragma once
#include "Component.h"
#include <glm/vec2.hpp>
#include "Structs.h"

namespace Willem
{

	class TransformComponent : public Component
	{
	public:
		TransformComponent(const Vector3& pos, const Vector3& scale = { 1,1,1 });
		TransformComponent(const Vector3& pos, float scale = 1.0f);
		TransformComponent();
		const Vector3& GetPosition() const;
		void SetPosition(const Vector3& pos);
		void SetPosition(float x, float y, float z);

		void SetPosition(const Vector2& pos);
		void SetPosition(float x, float y);

		const Vector3& GetScale() const;
		void SetScale(float x, float y, float z);
	private:
		Vector3 m_Position;
		Vector3 m_Scale;

	};
}
