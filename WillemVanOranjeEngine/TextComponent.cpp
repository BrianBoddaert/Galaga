#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>

#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "RenderComponent.h"

Willem::TextComponent::TextComponent(GameObject* gameObject, const std::string& text, Font* font, const SDL_Color& color, float wrap)
	:Component{ gameObject }
	, m_NeedsUpdate(true)
	, m_Text(text)
	, m_pFont(font)
	, m_Wrap{ wrap }
	, m_Color{color}
{ 
	UpdateTexture();
}

void Willem::TextComponent::Update(float)
{
	if (m_NeedsUpdate)
	{
		UpdateTexture();
	}
}

void Willem::TextComponent::UpdateTexture()
{
	int width{};
	int height{};

	SDL_Surface* surf;
	TTF_SetFontStyle(m_pFont->GetFont(), TTF_STYLE_BOLD );

	if (m_Wrap)
	{
		TTF_SizeText(m_pFont->GetFont(), m_Text.c_str(), &width, &height);
		surf = TTF_RenderText_Blended_Wrapped(m_pFont->GetFont(), m_Text.c_str(), m_Color, Uint32(width * m_Wrap));
	}
	else
	{
		surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_Color);
	}

	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);
	//m_pTexture = std::make_shared<Texture2D>(texture);
	m_NeedsUpdate = false;

	RenderComponent* renderComp = m_pGameObject->GetComponent<RenderComponent>();
	if (renderComp)
		renderComp->SetTexture(new Texture2D(texture));
}

// This implementation uses the dirty flag pattern
void Willem::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}


