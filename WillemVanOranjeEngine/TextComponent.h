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
		void Render(const Vector2& pos, const Vector2& scale = { 1,1 }) const override;

		void SetText(const std::string& text);

		void UpdateTexture();
		explicit TextComponent(const std::string& text, Font* font, float wrap = 1.0f);
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
		std::shared_ptr<Texture2D> m_pTexture;
	};
}
