#include <iostream> // std::cout
#include <sstream> // stringstream
#include <memory> // smart pointers
#include <vector>
#include <windows.h>

#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"

void Willem::Renderer::Init(SDL_Window * window)
{
	m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
}

void Willem::Renderer::Render() const
{
	SDL_RenderClear(m_Renderer);

	SceneManager::GetInstance().Render();
	
	SDL_RenderPresent(m_Renderer);
}

void Willem::Renderer::Destroy()
{
	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void ScaleSDLRect(SDL_Rect& rect, const Willem::Vector2& scale)
{
	float width = (float)rect.w;
	float height = (float)rect.h;

	rect.w = int(width *= scale.x);
	rect.h = int(height *= scale.y);
}


void Willem::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const Willem::Vector2& scale) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);

	ScaleSDLRect(dst, scale);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void Willem::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height, const Willem::Vector2& scale) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);

	ScaleSDLRect(dst, scale);

	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void Willem::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const SDL_Rect& src, const Willem::Vector2& scale) const
{
	//160 HEIGHT
	// 32 x 32 
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	//SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	dst.w = src.w;
	dst.h = src.h;
	
	ScaleSDLRect(dst, scale);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
}

void Willem::Renderer::RenderTexture(const Texture2D& texture, SDL_Rect& dst, const SDL_Rect& src, const Willem::Vector2& scale) const
{
	dst.w = src.w;
	dst.h = src.h;
	ScaleSDLRect(dst, scale);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
}