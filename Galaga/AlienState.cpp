#include "stdafx.h"
#include "AlienState.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include <SDL.h>

using namespace Willem;

void AlienState::AdjustSpritesToFitDirection()
{
	RenderComponent* renderComp = m_pGameObject->GetComponent<RenderComponent>();
	SDL_Rect srcRect = renderComp->GetSrcRect();
	int initialOffsetWidth = 1;
	int offsetWidth = 2;
	int spriteIndex = int(std::round(m_RotationRadians / float(M_PI / 8.0)));

	spriteIndex = spriteIndex % 16;

	srcRect.x = initialOffsetWidth + (spriteIndex * srcRect.w) + (spriteIndex * offsetWidth);
	renderComp->SetSrcRect(srcRect);
}