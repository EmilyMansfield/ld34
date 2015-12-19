#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include <string>

#include "game_state_title.hpp"
#include "game_state_settings.hpp"
#include "util.hpp"
#include "constants.hpp"

#ifdef __ANDROID__
void GameStateSettings::handleEvent(const sf::Event& event)
{
	if(event.type == sf::Event::TouchBegan)
	{
		sf::Vector2i p(event.touch.x, event.touch.y);
		sf::Vector2f touchPos(ld::renderTarget->mapPixelToCoords(p));
		if(mTextBack.getBounds().contains(touchPos))
		{
			std::shared_ptr<GameState> thisState = mState;
			mState.reset(new GameStateTitle(mState, thisState));
		}
		else if(mTextMusic.getBounds().contains(touchPos))
		{
			if(ld::musicAvailable && ld::musicOn == false)
			{
				mTextMusic.setString("Music is on");
				mTextMusic.setOrigin(11 * 5 * 0.5f, 1 * 6 * 0.5f);
				ld::musicOn = true;
			}
			else if(ld::musicAvailable && ld::musicOn == true)
			{
				mTextMusic.setString("Music is off");
				mTextMusic.setOrigin(12 * 5 * 0.5f, 1 * 6 * 0.5f);
				ld::musicOn = false;
			}
		}
	}
}
#else
void GameStateSettings::handleEvent(const sf::Event& event)
{
	if(event.type == sf::Event::KeyPressed)
	{
		if(event.key.code == ld::buttonLeft)
		{
			mSelectedOption = (mSelectedOption + 1) % 2;
			selectOption();
		}
		else if(event.key.code == ld::buttonRight)
		{
			if(mSelectedOption == 0)
			{
				if(ld::musicAvailable && ld::musicOn == false)
				{
					mTextMusic.setString("Music is on");
					mTextMusic.setOrigin(11 * 5 * 0.5f, 1 * 6 * 0.5f);
					ld::musicOn = true;
				}
				else if(ld::musicAvailable && ld::musicOn == true)
				{
					mTextMusic.setString("Music is off");
					mTextMusic.setOrigin(12 * 5 * 0.5f, 1 * 6 * 0.5f);
					ld::musicOn = false;
				}
			}
			else if(mSelectedOption == 1)
			{
				std::shared_ptr<GameState> thisState = mState;
				mState.reset(new GameStateTitle(mState, thisState));
			}
		}
	}
	if(event.type == sf::Event::TextEntered)
	{
		if(event.text.unicode <= 127)
		{
			char c = static_cast<char>(event.text.unicode);
			// Keep only uppercase alphanumeric chars, and convert
			// lowercase to uppercase
			if(c >= 'a' && c <= 'z') c -= 0x20;
			if(((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) &&
				ld::playerName.size() < 12)
			{
				ld::playerName.push_back(c);
				mNameStr.push_back(c);
			}
			else if((c == 127 || c == '\b') && ld::playerName.size() > 0)
			{
				ld::playerName.pop_back();
				mNameStr.pop_back();
			}
			mTextName.setString(mNameStr);
		}
	}
}
#endif /* __ANDROID__ */

void GameStateSettings::handleInput(float dt)
{

}

void GameStateSettings::update(float dt)
{

}

