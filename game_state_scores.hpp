#ifndef GAME_STATE_SCORES_HPP
#define GAME_STATE_SCORES_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <utility>
#include <tuple>

#include "game_state.hpp"
#include "constants.hpp"
#include "util.hpp"
#include "text.hpp"

class GameStateScores : public GameState
{
private:

	Text mTextTitle;
	Text mTextRestart;
	Text mTextBoardSetter;
	Text mTextQuit;

	std::string mTextBoardSetterStr;
	bool mShowingTopScores;
	std::vector<std::tuple<std::string, unsigned long, Text>> mTopScores;
	std::vector<std::tuple<std::string, unsigned long, Text>> mRelativeScores;

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
		else if(mSelectedOption == 1) selectedText = &mTextBoardSetter;
		else if(mSelectedOption == 2) selectedText = &mTextQuit;
		if(selectedText != nullptr)
		{
			deselect(&mTextRestart);
			deselect(&mTextBoardSetter);
			deselect(&mTextQuit);
			select(selectedText);
		}
	}

	std::string createSubmission(const std::string& name, unsigned long score)
	{
		// Add some 'encryption'. Please don't take this as a challenge or anything
		std::string packet = ld::leaderboardAuthentication +
			"n: " + name + "; " + "s: " + std::to_string(score);
		if(ld::leaderboardKeyLength <= 0) return "";

		std::string msg = "[";
		for(size_t i = 0; i < packet.size(); ++i)
		{
			msg += std::to_string(static_cast<int>(packet[i]) ^ ld::leaderboardKey[i % ld::leaderboardKeyLength]);
			if(i < packet.size() - 1) msg += ",";
		}
		msg += "]";

		return msg;
	}

	bool submitScore(const std::string& name, unsigned long score)
	{
		if(ld::leaderboardKeyLength <= 0) return false;

		// POST was too hard to figure out, so submitting scores uses GET
		// Take that, standards
		std::string requestStr("/submit?input=" + createSubmission(name, score));
		sf::Http::Request request(requestStr);
		sf::Http http(ld::leaderboardUrl);
		sf::Http::Response response = http.sendRequest(request);

		return response.getStatus() == sf::Http::Response::Ok;
	}

	bool getScores(bool topScores)
	{
		sf::Http::Response response;
		sf::Http http(ld::leaderboardUrl);
		if(topScores)
		{
			sf::Http::Request request("/retrieve");
			response = http.sendRequest(request);
		}
		else
		{
			sf::Http::Request request("/byscore?score=" + std::to_string(mScore) + "&name=" + ld::playerName);
			response = http.sendRequest(request);
		}

		if(response.getStatus() == sf::Http::Response::Ok)
		{
			// std::string body = reponse.getBody();
			std::istringstream iss(response.getBody());
			// Very bad extraction incoming
			std::vector<std::string> v{
				std::istream_iterator<std::string>(iss),
				std::istream_iterator<std::string>()};

			// Relative scores have placement of given score first
			unsigned int place = 0;
			if(!topScores)
			{
				place = std::stoi(v[0]);
				v.erase(v.begin());
				// If there aren't 20 elements there aren't 10 total places
				if(v.size() < 20)
				{
					// So the first element in the list is the first actual element
					place = 0;
				}
			}
			bool hasSelectedPlayer = false;
			for(int i = 0; i < v.size(); i += 2)
			{
				std::tuple<std::string, unsigned long, Text> score;
				std::string& tName = std::get<0>(score);
				unsigned long& tScore = std::get<1>(score);
				Text& tText = std::get<2>(score);
				tName = v[i];
				tScore = std::stoi(v[i+1]);

				tName.erase(
					std::remove_if(
						tName.begin(),
						tName.end(),
						[](char c) { return c < '0' || (c > '9' && c < 'A') || c > 'Z'; }),
					tName.end());

				// Expand to 12 chars
				std::string textStr = tName;
				if(textStr.size() < 12) textStr.append(12-tName.size(), ' ');

				// Format text
				textStr = std::to_string(i/2 + 1 + place) + ". " + textStr + " " + std::to_string(tScore);
				tText.setString(textStr);
				tText.setColor(sf::Color::White);
				if(tName == ld::playerName && tScore == mScore && !hasSelectedPlayer)
				{
					tText.setColor(ld::hsvToRgb(
						ld::rand(0.0f, 360.0f),
						ld::saturation,
						ld::value));
					hasSelectedPlayer = true;
				}
				tText.setOrigin(textStr.size() * 5 * 0.5f, 1 * 6 * 0.5f);
				tText.setPosition(ld::gameDim/2.0f, ld::gameDim*(0.8f+i*0.14f)/5.0f);
				tText.setScale(0.08f, 0.08f);
				if(topScores)
					mTopScores.push_back(score);
				else
					mRelativeScores.push_back(score);
			}

			return true;
		}

		return false;
	}

public:

	GameStateScores(std::shared_ptr<GameState>& state,
		std::shared_ptr<GameState>& prevState,
		unsigned long score) :
		GameState(state, prevState),
		mTextTitle("Leaderboard"),
		mTextRestart("Restart"),
		mTextBoardSetter("Showing Yours"),
		mTextQuit("Quit"),
		mTextBoardSetterStr("Showing Yours"),
		mShowingTopScores(false),
		mSelectedOption(0),
		mScore(score)
	{
		mTextTitle.setPosition(ld::gameDim/2.0f, 0.0f);
		mTextTitle.setOrigin(11 * 5 * 0.5f, 1 * 6 * 0.5f);
		mTextTitle.setScale(0.2f, 0.2f);

		mTextRestart.setPosition(ld::gameDim/2.0f, ld::gameDim*4.0f/5.0f);
		mTextRestart.setOrigin(7 * 5 * 0.5f, 1 * 6 * 0.5f);
		mTextRestart.setScale(0.1f, 0.1f);

		mTextBoardSetter.setPosition(ld::gameDim/2.0f, ld::gameDim*4.25f/5.0f);
		mTextBoardSetter.setOrigin(mTextBoardSetterStr.size() * 5 * 0.5f, 1 * 6 * 0.5f);
		mTextBoardSetter.setScale(0.1f, 0.1f);

		mTextQuit.setPosition(ld::gameDim/2.0f, ld::gameDim*4.5f/5.0f);
		mTextQuit.setOrigin(4 * 5 * 0.5f, 1 * 6 * 0.5f);
		mTextQuit.setScale(0.1f, 0.1f);

		mTextRestart.setColor(ld::hsvToRgb(
			ld::rand(0.0f, 360.0f),
			ld::saturation,
			ld::value));

		submitScore(ld::playerName, mScore);
		// Preload both sets of scores
		getScores(true);
		getScores(false);
	}

	virtual void handleEvent(const sf::Event& event);
	virtual void handleInput(float dt);
	virtual void update(float dt);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(mTextTitle, states);
		target.draw(mTextRestart, states);
		target.draw(mTextBoardSetter, states);
		target.draw(mTextQuit, states);
		if(mShowingTopScores)
		{
			for(auto score : mTopScores)
			{
				target.draw(std::get<2>(score), states);
			}
		}
		else
		{
			for(auto score : mRelativeScores)
			{
				target.draw(std::get<2>(score), states);
			}
		}
	}
};

#endif /* GAME_STATE_SCORES_HPP */
