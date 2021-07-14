#include <iostream>
#include "Texture2D.h"
#include <SDL.h>
#include <SDL_image.h>

Willem::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_Texture);
}

SDL_Texture* Willem::Texture2D::GetSDLTexture() const
{
	return m_Texture;
}

Willem::Texture2D::Texture2D(SDL_Texture* texture)
{
	m_Texture = texture;
}
