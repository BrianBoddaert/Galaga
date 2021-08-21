#include "stdafx.h"
#include "TransformComponent.h"
#include "BackgroundComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Minigin.h"
#include "CollisionManager.h"

#include "RenderComponent.h"
#include "AIFlyComponent.h"
#include "ExplosionManager.h"
#include "ControlComponent.h"
#include "GameObject.h"
#include "BombRunState.h"
#include "ShootComponent.h"
#include <string>
#include "AudioClasses.h"

using namespace Willem;

BackgroundComponent::BackgroundComponent()
	:m_Speed{400.0f}
	, m_Transform{ nullptr }
	, m_Offset{ 716.8f}
{}

void BackgroundComponent::Update(float deltaT)
{
	if (!m_Transform)
		m_Transform = m_pGameObject->GetComponent<Willem::TransformComponent>();

	const Vector2 pos = m_Transform->GetPosition();

	if (pos.y > Minigin::GetWindowSurface()->h)
		m_Transform->SetPosition(0, -m_Offset);
	else
		m_Transform->SetPosition(pos + Vector2{0,m_Speed*deltaT});


}