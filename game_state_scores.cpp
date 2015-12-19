#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include <string>

#include "game_state_scores.hpp"
#include "game_state_game.hpp"
#include "util.hpp"
#include "constants.hpp"

#ifdef __ANDROID__
void GameStateScores::handleEvent(const sf::Event& event)
{
	if(event.type == sf::Event::TouchBegan)
	{
		sf::Vector2i p(event.touch.x, event.touch.y);
		sf::Vector2f touchPos(ld::renderTarget->mapPixelToCoords(p));
		if(mTextRestart.getBounds().contains(touchPos))
		{
			std::shared_ptr<GameState> thisState = mState;
			mState.reset(new GameStateGame(mState, thisState));
		}
		else if(mTextBoardSetter.getBounds().contains(touchPos))
		{
			if(mShowingTopScores)
			{
				mShowingTopScores = false;
				mTextBoardSetterStr = "Showing Yours";
				mTextBoardSetter.setString(mTextBoardSetterStr);
				mTextBoardSetter.setOrigin(mTextBoardSetterStr.size() * 5 * 0.5f, 1 * 6 * 0.5f);
			}
			else
			{
				mShowingTopScores = true;
				mTextBoardSetterStr = "Showing Top";
				mTextBoardSetter.setString(mTextBoardSetterStr);
				mTextBoardSetter.setOrigin(mTextBoardSetterStr.size() * 5 * 0.5f, 1 * 6 * 0.5f);
			}
		}
		else if(mTextQuit.getBounds().contains(touchPos))
		{
			mState.reset();
		}
	}
}
#else
void GameStateScores::handleEvent(const sf::Event& event)
{
	if(event.type == sf::Event::KeyPressed)
	{
		if(event.key.code == ld::buttonLeft)
		{
			mSelectedOption = (mSelectedOption + 1) % 3;
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
				if(mShowingTopScores)
				{
					mShowingTopScores = false;
					mTextBoardSetterStr = "Showing Yours";
					mTextBoardSetter.setString(mTextBoardSetterStr);
					mTextBoardSetter.setOrigin(mTextBoardSetterStr.size() * 5 * 0.5f, 1 * 6 * 0.5f);
				}
				else
				{
					mShowingTopScores = true;
					mTextBoardSetterStr = "Showing Top";
					mTextBoardSetter.setString(mTextBoardSetterStr);
					mTextBoardSetter.setOrigin(mTextBoardSetterStr.size() * 5 * 0.5f, 1 * 6 * 0.5f);
				}
			}
			else if(mSelectedOption == 2)
			{
				std::shared_ptr<GameState> thisState = mState;
				mState.reset();
			}
		}
	}
}
#endif /* __ANDROID__ */

void GameStateScores::handleInput(float dt)
{

}

void GameStateScores::update(float dt)
{

}

