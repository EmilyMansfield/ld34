#ifndef GAME_STATE_SCORES_HPP
#define GAME_STATE_SCORES_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <cmath>
#include <vector>
#include <iostream>
#include <string>

#include "game_state.hpp"
#include "constants.hpp"
#include "util.hpp"
#include "text.hpp"

class GameStateScores : public GameState
{
private:

	Text mTextTitle;
	Text mTextRestart;
	Text mTextQuit;

	int mSelectedOption;

	unsigned long mScore;

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
		if(mSelectedOption == 0) selectedText = &mTextRestart;
		else if(mSelectedOption == 1) selectedText = &mTextQuit;
		if(selectedText != nullptr)
		{
			deselect(&mTextRestart);
			deselect(&mTextQuit);
			select(selectedText);
		}
	}

	std::string createSubmission(const std::string& name, unsigned long score)
	{
		// Add some 'encryption'. Please don't take this as a challenge or anything
		std::string packet = ld::leaderboardAuthentication +
			"n: " + name + "; " + "s: " + std::to_string(score);
		if(ld::leaderboardKeyLength < 0) return "";

		std::string msg = "[";
		for(size_t i = 0; i < packet.size(); ++i)
		{
			msg += std::to_string(static_cast<int>(packet[i]) ^ ld::leaderboardKey[i % ld::leaderboardKeyLength]);
			if(i < packet.size() - 1) msg += ",";
		}
		msg += "]";

		return msg;
	}

public:

	GameStateScores(std::shared_ptr<GameState>& state,
		std::shared_ptr<GameState>& prevState,
		unsigned long score) :
		GameState(state, prevState),
		mTextTitle("Leaderboard"),
		mTextRestart("Restart"),
		mTextQuit("Quit"),
		mSelectedOption(0),
		mScore(score)
	{
		mTextTitle.setPosition(ld::gameDim/2.0f, 0.0f);
		mTextTitle.setOrigin(11 * 5 * 0.5f, 1 * 6 * 0.5f);
		mTextTitle.setScale(0.2f, 0.2f);

		mTextRestart.setPosition(ld::gameDim/2.0f, ld::gameDim*4.0f/5.0f);
		mTextRestart.setOrigin(7 * 5 * 0.5f, 1 * 6 * 0.5f);
		mTextRestart.setScale(0.1f, 0.1f);

		mTextQuit.setPosition(ld::gameDim/2.0f, ld::gameDim*4.5f/5.0f);
		mTextQuit.setOrigin(4 * 5 * 0.5f, 1 * 6 * 0.5f);
		mTextQuit.setScale(0.1f, 0.1f);

		mTextRestart.setColor(ld::hsvToRgb(
			ld::rand(0.0f, 360.0f),
			ld::saturation,
			ld::value));

		std::cout << createSubmission("Dan", 1500) << std::endl;
	}

	virtual void handleEvent(const sf::Event& event);
	virtual void handleInput(float dt);
	virtual void update(float dt);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(mTextTitle, states);
		target.draw(mTextRestart, states);
		target.draw(mTextQuit, states);
	}
};

#endif /* GAME_STATE_SCORES_HPP */
