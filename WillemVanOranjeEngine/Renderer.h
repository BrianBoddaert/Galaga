#pragma once
#include "Singleton.h"
#include <SDL.h>
#include "Structs.h"
struct SDL_Window;
struct SDL_Renderer;

namespace Willem
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y, const Vector2& scale = { 1,1 }) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height, const Vector2& scale = { 1,1 }) const;
		void RenderTexture(const Texture2D& texture, const float x, const float y, const SDL_Rect& src, const Vector2& scale = { 1,1 }) const;
		void RenderTexture(const Texture2D& texture, SDL_Rect& dst, const SDL_Rect& src, const Vector2& scale = { 1,1 }) const;

		SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }
	private:
		SDL_Renderer* m_Renderer{};
	};
}

