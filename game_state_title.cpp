#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include <string>

#include "game_state_title.hpp"
#include "game_state_settings.hpp"
#include "game_state_game.hpp"
#include "util.hpp"
#include "constants.hpp"

#ifdef __ANDROID__
void GameStateTitle::handleEvent(const sf::Event& event)
{
	if(event.type == sf::Event::TouchBegan)
	{
		// Get touched position and check which menu option contains
		// that point
		sf::Vector2i p(event.touch.x, event.touch.y);
		sf::Vector2f touchPos(ld::renderTarget->mapPixelToCoords(p));
		if(mTextPlay.getBounds().contains(touchPos))
		{
			std::shared_ptr<GameState> thisState = mState;
			mState.reset(new GameStateGame(mState, thisState));
		}
		else if(mTextSettings.getBounds().contains(touchPos))
		{
			std::shared_ptr<GameState> thisState = mState;
			mState.reset(new GameStateSettings(mState, thisState));
		}
	}
}
#else
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
			else if(mSelectedOption == 1)
			{
				std::shared_ptr<GameState> thisState = mState;
				mState.reset(new GameStateSettings(mState, thisState));
			}
		}
	}
}
#endif /* __ANDROID__ */

void GameStateTitle::handleInput(float dt)
{

}

void GameStateTitle::update(float dt)
{

}

