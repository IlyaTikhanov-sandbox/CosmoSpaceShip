#pragma once

#include "ECS.h"
#include "../AssetManager.h"
#include "../TextureManager.h"
#include "../Game.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class UILabel : public Component
{
public:
	UILabel(){}
	UILabel(int Xpos, int Ypos, std::string text, std::string font, SDL_Color& color) :
		labelText(text), labelFont(font), textColor(color)
	{
		position.x = Xpos;
		position.y = Ypos;

		SetLabelText(labelText,labelFont);
	}
	UILabel(const UILabel& other)
	{
		position.x = other.getPosition().x;
		position.y = other.getPosition().y;

		SetLabelText(other.getLabelText(), other.getLabelFont());
	}
	//UILabel(UILabel&& other)
	//{
	//	position.x = other.getPosition().x;
	//	position.y = other.getPosition().y;
	//
	//	SetLabelText(other.getLabelText(), other.getLabelFont());
	//
	//	other.nullTexture();
	//}
	~UILabel()
	{

	}

	void SetLabelText_Color(std::string text, std::string font, SDL_Color color)
	{
		SDL_Surface* surf = TTF_RenderText_Blended(Game::assets->GetFont(font), text.c_str(), color);
		labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
		SDL_FreeSurface(surf);

		SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
	}

	void SetLabelText(std::string text, std::string font)
	{
		SDL_Surface* surf = TTF_RenderText_Blended(Game::assets->GetFont(font), text.c_str(), textColor);
		labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
		SDL_FreeSurface(surf);

		SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
	}

	void SetLabelColor(SDL_Color color)
	{
		SDL_Surface* surf = TTF_RenderText_Blended(Game::assets->GetFont(labelFont), labelText.c_str(), color);
		labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
		SDL_FreeSurface(surf);

		SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
	}

	void draw() override
	{
		SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
	}

	SDL_Rect    getPosition()  const { return position;  }
	std::string getLabelText() const { return labelText; }
	std::string getLabelFont() const { return labelFont; }
	SDL_Color   getTextColor() const { return textColor; }
	
	void        nullTexture() { labelTexture = nullptr; }

private:
	SDL_Rect position;
	std::string labelText;
	std::string labelFont;
	SDL_Color textColor;
	SDL_Texture* labelTexture;

};
