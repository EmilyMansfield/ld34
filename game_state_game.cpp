#include "game_state_game.hpp"
#include <SFML/Graphics.hpp>

void GameStateGame::handleEvent(const sf::Event& event)
{
	if(event.type == sf::Event::KeyPressed)
	{
		if(event.key.code == ld::buttonLeft)
		{
			mPlayer.rotate(-1);
		}
		else if(event.key.code == ld::buttonRight)
		{
			mPlayer.rotate(1);
		}
		else if(event.key.code == sf::Keyboard::Space)
		{
			mPlayer.addSlot();
		}
	}
}

void GameStateGame::handleInput(float dt)
{

}

void GameStateGame::update(float dt)
{
	mPlayer.update(dt);
}

