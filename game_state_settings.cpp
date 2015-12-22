#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include <string>

#include "game_state_title.hpp"
#include "game_state_settings.hpp"
#include "util.hpp"
#include "constants.hpp"

GameStateSettings::GameStateSettings(std::shared_ptr<GameState>& state,
	std::shared_ptr<GameState>& prevState) :
	GameState(state, prevState),
	mTextTitle("Settings"),
	mTextMusic("Music is unavailable"),
	mTextBack("Back"),
	mTextName("Name " + ld::playerName),
	mTextAbout(ld::textAbout),
	mTextAbout2(ld::textAbout2),
	mTextAuthor(ld::textAuthor),
	mTextAuthor2(ld::textAuthor2),
	mNameStr("Name " + ld::playerName)
	#ifndef __ANDROID__
		,mSelectedOption(0)
	#endif /* !__ANDROID__ */
{
	mTextTitle.setOrigin(8 * 5 * 0.5f, 1 * 6 * 0.5f);
	mTextTitle.setScale(0.2f, 0.2f);

	if(ld::musicAvailable && ld::musicOn)
	{
		mTextMusic.setString("Music is on");
		mTextMusic.setOrigin(11 * 5 * 0.5f, 1 * 6 * 0.5f);
		mTextMusic.setScale(0.1f, 0.1f);
	}
	else if(ld::musicAvailable)
	{	
		mTextMusic.setString("Music is off");
		mTextMusic.setOrigin(12 * 5 * 0.5f, 1 * 6 * 0.5f);
		mTextMusic.setScale(0.1f, 0.1f);
	}
	else
	{
		mTextMusic.setOrigin(20 * 5 * 0.5f, 1 * 6 * 0.5f);
		mTextMusic.setScale(0.1f, 0.1f);
	}

	mTextName.setString(mNameStr);
	mTextName.setOrigin(mNameStr.size() * 5 * 0.5f, 1 * 6 * 0.5f);
	mTextName.setScale(0.1f, 0.1f);

	mTextBack.setOrigin(4 * 5 * 0.5f, 1 * 6 * 0.5f);
	mTextBack.setScale(0.1f, 0.1f);

	mTextAbout.setOrigin(ld::textAbout.size() * 5 * 0.5f, 1 * 6 * 0.5f);
	mTextAbout.setScale(0.1f, 0.1f);
	mTextAbout2.setOrigin(ld::textAbout2.size() * 5 * 0.5f, 1 * 6 * 0.5f);
	mTextAbout2.setScale(0.1f, 0.1f);

	mTextAuthor.setOrigin(ld::textAuthor.size() * 5 * 0.5f, 1 * 6 * 0.5f);
	mTextAuthor.setScale(0.1f, 0.1f);
	mTextAuthor2.setOrigin(ld::textAuthor2.size() * 5 * 0.5f, 1 * 6 * 0.5f);
	mTextAuthor2.setScale(0.1f, 0.1f);

	onResize();
	#ifndef __ANDROID__
		selectOption();
	#endif /* !__ANDROID__ */
}

#ifndef __ANDROID__
	void GameStateSettings::select(Text* ptr)
	{
		ptr->setColor(ld::hsvToRgb(
			ld::rand(0.0f, 360.0f),
			ld::saturation,
			ld::value));
	}

	void GameStateSettings::deselect(Text* ptr)
	{
		ptr->setColor(sf::Color(255, 255, 255));
	}

	void GameStateSettings::selectOption()
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
#endif /* !__ANDROID__ */

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

void GameStateSettings::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mTextTitle, states);
	target.draw(mTextMusic, states);
	target.draw(mTextBack, states);
	target.draw(mTextName, states);
	target.draw(mTextAbout, states);
	target.draw(mTextAbout2, states);
	target.draw(mTextAuthor, states);
	target.draw(mTextAuthor2, states);
}

void GameStateSettings::onResize()
{
	mTextTitle.setPosition(ld::gameDimFullWidth/2.0f, ld::gameDimFullHeight*0.5f/5.0f);
	mTextMusic.setPosition(ld::gameDimFullWidth/2.0f, ld::gameDimFullHeight*1.5/5.0f);
	mTextName.setPosition(ld::gameDimFullWidth/2.0f, ld::gameDimFullHeight*2.0f/5.0f);
	mTextBack.setPosition(ld::gameDimFullWidth/2.0f, ld::gameDimFullHeight*2.5f/5.0f);
	mTextAbout.setPosition(ld::gameDimFullWidth/2.0f, ld::gameDimFullHeight*3.0f/5.0f);
	mTextAbout2.setPosition(ld::gameDimFullWidth/2.0f, ld::gameDimFullHeight*3.5f/5.0f);
	mTextAuthor.setPosition(ld::gameDimFullWidth/2.0f, ld::gameDimFullHeight*4.0f/5.0f);
	mTextAuthor2.setPosition(ld::gameDimFullWidth/2.0f, ld::gameDimFullHeight*4.5f/5.0f);

	#ifdef __ANDROID__
		mTextTitle.update();
		mTextMusic.update();
		mTextName.update();
		mTextBack.update();
		mTextAbout.update();
		mTextAbout2.update();
		mTextAuthor.update();
		mTextAuthor2.update();
	#endif /* __ANDROID__ */
}
