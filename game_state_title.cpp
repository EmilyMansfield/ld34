#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include <string>

#include "game_state_title.hpp"
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
			mSelectedOption = (mSelectedOption + 3) % 2;
			selectOption();
		}
	}
}

void GameStateTitle::handleInput(float dt)
{

}

void GameStateTitle::update(float dt)
{

}

