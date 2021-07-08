#pragma once

#include "UILabel.h"
#include <map>

enum MenuStatus
{
	None,
	Active,
	Finished,
	GameClosed
};

enum MenuButtons
{
	Play,
	Exit
};

class Menu
{
public:

	Menu()
	{
		m_currentMenuStatus = MenuStatus::Active;

		SDL_Color white = { 255,255,255,255 };

		UILabel play = UILabel(PLAY_WIDTH / 2, PLAY_HEIGHT / 2, "Play", "courier", white);

		m_currentButton = 0;
		m_contentMenu.emplace("play",play);
	}

	~Menu();

	UILabel getLabelbyKey(std::string key) { return m_contentMenu[key]; }

	bool isMenuActive()     { return m_currentMenuStatus == MenuStatus::Active; }
	bool isMenuGameClosed() { return m_currentMenuStatus == MenuStatus::GameClosed; }
	void changeMenuStatus(MenuStatus newStatus) { m_currentMenuStatus = MenuStatus(newStatus); }

	void resetCurrentButton() { m_currentButton = 0; }

	int getCurrentButton()   { return m_currentButton; }
	int getSizeMenuButtons() { return m_sizeMenuButtons; }
	void keyUp()
	{
		--m_currentButton;
		if (m_currentButton < 0)
			m_currentButton = m_sizeMenuButtons - 1;
	}
	void keyDown()
	{
		++m_currentButton;
		if (m_currentButton == m_sizeMenuButtons)
			m_currentButton = m_currentButton % m_sizeMenuButtons;
	}

private:
	std::map<std::string, UILabel> m_contentMenu;
	MenuStatus m_currentMenuStatus;

	int m_currentButton = -1;
	int m_sizeMenuButtons = 2;
};