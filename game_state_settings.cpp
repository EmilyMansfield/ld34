#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include <string>

#include "game_state_title.hpp"
#include "game_state_settings.hpp"
#include "util.hpp"
#include "constants.hpp"

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
}

void GameStateSettings::handleInput(float dt)
{

}

void GameStateSettings::update(float dt)
{

}

