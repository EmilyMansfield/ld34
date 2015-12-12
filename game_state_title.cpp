#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include <string>

#include "game_state_title.hpp"
#include "game_state_game.hpp"
#include "util.hpp"
#include "constants.hpp"

void GameStateTitle::handleEvent(const sf::Event& event)
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
				std::shared_ptr<GameState> thisState = mState;
				mState.reset(new GameStateGame(mState, thisState));
			}
		}
	}
}

void GameStateTitle::handleInput(float dt)
{

}

void GameStateTitle::update(float dt)
{

}

