#pragma once
#include "Component.h"
#include "Texture2D.h"
#include <SDL.h>

namespace Willem
{
	class RenderComponent : public Component
	{
	public:
		RenderComponent(Texture2D* texture);
		RenderComponent();
		RenderComponent(const SDL_Rect& src);

		~RenderComponent()override;

		RenderComponent(const RenderComponent&) = delete;
		RenderComponent& operator=(const RenderComponent&) = delete;
		RenderComponent(RenderComponent&&) = delete;
		RenderComponent& operator= (RenderComponent&&) = delete;

		 void Update(float deltaT) override;
		 void Render(const Vector2& pos, const Vector2& scale) const;
		 Texture2D* GetTexture();
		 void SetTexture(Texture2D* texture);
		 const SDL_Rect& GetSrcRect() const;
		 void SetSrcRect(const SDL_Rect&);

		 const Vector2& GetSpritePixelSize() { return m_SpritePixelSize; };
		 const Vector2 GetSpritePixelSizeScaled();
	private:

		const Vector2 m_SpritePixelSize;

		Texture2D* m_pTexture{ nullptr };

		SDL_Rect m_SrcRect;

	};

}