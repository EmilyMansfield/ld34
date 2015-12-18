#ifndef GAME_STATE_TITLE_HPP
#define GAME_STATE_TITLE_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "game_state.hpp"
#include "constants.hpp"
#include "util.hpp"
#include "text.hpp"

class GameStateTitle : public GameState
{
private:

	Text mTextTitle;
	Text mTextPlay;
	Text mTextSettings;

	int mSelectedOption;

	void select(Text* ptr)
	{
		ptr->setColor(ld::hsvToRgb(
			ld::rand(0.0f, 360.0f),
			ld::saturation,
			ld::value));
	}

	void deselect(Text* ptr)
	{
		ptr->setColor(sf::Color(255, 255, 255));
	}

	void selectOption()
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

public:

	GameStateTitle(std::shared_ptr<GameState>& state,
		std::shared_ptr<GameState>& prevState) :
		GameState(state, prevState),
		mTextTitle(ld::gameName),
		mTextPlay("Play"),
		mTextSettings("Settings"),
		mSelectedOption(0)
	{
		mTextTitle.setPosition(ld::gameDim/2.0f, ld::gameDim/5.0f);
		mTextTitle.setOrigin(ld::gameName.size() * 5 * 0.5f, 1 * 6 * 0.5f);
		mTextTitle.setScale(0.3f, 0.3f);

		mTextPlay.setPosition(ld::gameDim/2.0f, ld::gameDim*2.5/5.0f);
		mTextPlay.setOrigin(4 * 5 * 0.5f, 1 * 6 * 0.5f);
		mTextPlay.setScale(0.2f, 0.2f);

		mTextSettings.setPosition(ld::gameDim/2.0f, ld::gameDim*3.5/5.0f);
		mTextSettings.setOrigin(8 * 5 * 0.5f, 1 * 6 * 0.5f);
		mTextSettings.setScale(0.2f, 0.2f);

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

	virtual void handleEvent(const sf::Event& event);
	virtual void handleInput(float dt);
	virtual void update(float dt);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(mTextTitle, states);
		target.draw(mTextPlay, states);
		target.draw(mTextSettings, states);
	}
};

#endif /* GAME_STATE_TITLE_HPP */
