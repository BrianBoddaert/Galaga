#pragma once
#include "Component.h"
#include "Structs.h"
namespace Willem
{
	class Font;
	class Texture2D;
	class TextComponent final : public Component
	{
	public:
		void Update(float deltaT) override;

		void SetText(const std::string& text);

		void UpdateTexture();


		explicit TextComponent(GameObject* gameObject, const std::string& text, Font* font, const SDL_Color& color = { 255,255,255 }, float wrap = 1.0f);
		virtual ~TextComponent() override = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
	private:
		bool m_NeedsUpdate;
		float m_Wrap;
		std::string m_Text;
		std::shared_ptr<Font> m_pFont;
		const SDL_Color m_Color;
	};
}
