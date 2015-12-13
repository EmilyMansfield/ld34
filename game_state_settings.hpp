#ifndef GAME_STATE_SETTINGS_HPP
#define GAME_STATE_SETTINGS_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "game_state.hpp"
#include "constants.hpp"
#include "util.hpp"
#include "text.hpp"

class GameStateSettings : public GameState
{
private:

	Text mTextTitle;
	Text mTextMusic;
	Text mTextBack;
	Text mTextAbout;
	Text mTextAuthor;

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
		if(mSelectedOption == 0) selectedText = &mTextMusic;
		else if(mSelectedOption == 1) selectedText = &mTextBack;
		if(selectedText != nullptr)
		{
			deselect(&mTextMusic);
			deselect(&mTextBack);
			select(selectedText);
		}
	}

public:

	GameStateSettings(std::shared_ptr<GameState>& state,
		std::shared_ptr<GameState>& prevState) :
		GameState(state, prevState),
		mTextTitle("Settings"),
		mTextMusic("Music is unavailable"),
		mTextBack("Back"),
		mTextAbout(ld::textAbout),
		mTextAuthor(ld::textAuthor),
		mSelectedOption(0)
	{
		mTextTitle.setPosition(ld::gameDim/2.0f, 0.0f);
		mTextTitle.setOrigin(8 * 5 * 0.5f, 1 * 6 * 0.5f);
		mTextTitle.setScale(0.2f, 0.2f);

		mTextMusic.setPosition(ld::gameDim/2.0f, ld::gameDim*1.5/5.0f);
		if(ld::musicAvailable == true)
		{
			mTextMusic.setString("Music is on");
			mTextMusic.setOrigin(11 * 5 * 0.5f, 1 * 6 * 0.5f);
			mTextMusic.setScale(0.2f, 0.2f);
		}
		else
		{
			mTextMusic.setOrigin(20 * 5 * 0.5f, 1 * 6 * 0.5f);
			mTextMusic.setScale(0.1f, 0.1f);
		}

		mTextBack.setPosition(ld::gameDim/2.0f, ld::gameDim*2.5/5.0f);
		mTextBack.setOrigin(4 * 5 * 0.5f, 1 * 6 * 0.5f);
		mTextBack.setScale(0.2f, 0.2f);

		selectOption();
	}

	virtual void handleEvent(const sf::Event& event);
	virtual void handleInput(float dt);
	virtual void update(float dt);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(mTextTitle, states);
		target.draw(mTextMusic, states);
		target.draw(mTextBack, states);
	}
};

#endif /* GAME_STATE_SETTINGS_HPP */
