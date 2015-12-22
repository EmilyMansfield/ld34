#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include <string>

#include "game_state_title.hpp"
#include "game_state_settings.hpp"
#include "game_state_game.hpp"
#include "util.hpp"
#include "constants.hpp"

GameStateTitle::GameStateTitle(std::shared_ptr<GameState>& state,
	std::shared_ptr<GameState>& prevState) :
	GameState(state, prevState),
	mTextTitle(ld::gameName),
	mTextPlay("Play"),
	mTextSettings("Settings"),
	mSelectedOption(0)
{
	mTextTitle.setOrigin(ld::gameName.size() * 5 * 0.5f, 1 * 6 * 0.5f);
	mTextTitle.setScale(0.3f, 0.3f);

	mTextPlay.setOrigin(4 * 5 * 0.5f, 1 * 6 * 0.5f);
	mTextPlay.setScale(0.2f, 0.2f);

	mTextSettings.setOrigin(8 * 5 * 0.5f, 1 * 6 * 0.5f);
	mTextSettings.setScale(0.2f, 0.2f);

	onResize();

	mTextPlay.setColor(ld::hsvToRgb(
		ld::rand(0.0f, 360.0f),
		ld::saturation,
		ld::value));

	#ifdef __ANDROID__
		mTextTitle.update();
		mTextPlay.update();
		mTextSettings.update();
	#endif /* __ANDROID__ */
}

void GameStateTitle::select(Text* ptr)
{
	ptr->setColor(ld::hsvToRgb(
		ld::rand(0.0f, 360.0f),
		ld::saturation,
		ld::value));
}

void GameStateTitle::deselect(Text* ptr)
{
	ptr->setColor(sf::Color(255, 255, 255));
}

void GameStateTitle::selectOption()
{
	Text* selectedText = nullptr;
	if(mSelectedOption == 0) selectedText = &mTextPlay;
	else if(mSelectedOption == 1) selectedText = &mTextSettings;
	if(selectedText != nullptr)
	{
		deselect(&mTextPlay);
		deselect(&mTextSettings);
		select(selectedText);
	}
}

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

void GameStateTitle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mTextTitle, states);
	target.draw(mTextPlay, states);
	target.draw(mTextSettings, states);
}

void GameStateTitle::onResize()
{
	mTextTitle.setPosition(ld::gameDimFullWidth/2.0f, ld::gameDimFullHeight*1.5f/5.0f);
	mTextPlay.setPosition(ld::gameDimFullWidth/2.0f, ld::gameDimFullHeight*3.0f/5.0f);
	mTextSettings.setPosition(ld::gameDimFullWidth/2.0f, ld::gameDimFullHeight*4.0f/5.0f);
}
