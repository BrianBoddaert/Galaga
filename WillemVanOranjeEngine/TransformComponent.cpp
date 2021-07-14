#include <iostream>
#include "TransformComponent.h"
#include "SceneManager.h"
#include "Scene.h"

using namespace Willem;

TransformComponent::TransformComponent(const Vector3& pos,const Vector3& scale)
	:m_Position{pos}
	, m_Scale{ scale }
{
}
TransformComponent::TransformComponent(const Vector3& pos,float scale)
	: m_Position{ pos }
	, m_Scale{ scale,scale,scale }
{
}
TransformComponent::TransformComponent()
	:m_Position{ 0,0,0 }
{

}

void TransformComponent::SetPosition(const Vector3& pos)
{
	m_Position.x = pos.x;
	m_Position.y = pos.y;
	m_Position.z = pos.z;
}

void TransformComponent::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
	SceneManager::GetInstance().GetCurrentScene()->SortOnZAxis();
}

void TransformComponent::SetPosition(const Vector2& pos)
{
	m_Position.x = pos.x;
	m_Position.y = pos.y;
}

void TransformComponent::SetPosition(const float x, const float y)
{
	m_Position.x = x;
	m_Position.y = y;

}

const Vector3& TransformComponent::GetPosition() const 
{
	return m_Position; 
}

void TransformComponent::SetScale(const float x, const float y, const float z)
{
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;
}


const Vector3& TransformComponent::GetScale() const
{
	return m_Scale;
}