#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include <string>

#include "game_state_scores.hpp"
#include "game_state_game.hpp"
#include "util.hpp"
#include "constants.hpp"

GameStateScores::GameStateScores(std::shared_ptr<GameState>& state,
	std::shared_ptr<GameState>& prevState,
	unsigned long score) :
	GameState(state, prevState),
	mTextTitle("Leaderboard"),
	mTextRestart("Restart"),
	mTextBoardSetter("Showing Yours"),
	mTextQuit("Quit"),
	mTextBoardSetterStr("Showing Yours"),
	mShowingTopScores(false),
	mScore(score)
	#ifndef __ANDROID__
		,mSelectedOption(0)
	#endif /* !__ANDROID__ */
{
	mTextTitle.setOrigin(11 * 5 * 0.5f, 1 * 6 * 0.5f);
	mTextTitle.setScale(0.2f, 0.2f);

	mTextRestart.setOrigin(7 * 5 * 0.5f, 1 * 6 * 0.5f);
	mTextRestart.setScale(0.1f, 0.1f);

	mTextBoardSetter.setOrigin(mTextBoardSetterStr.size() * 5 * 0.5f, 1 * 6 * 0.5f);
	mTextBoardSetter.setScale(0.1f, 0.1f);

	mTextQuit.setOrigin(4 * 5 * 0.5f, 1 * 6 * 0.5f);
	mTextQuit.setScale(0.1f, 0.1f);

	onResize();

	#ifndef __ANDROID__
		mTextRestart.setColor(ld::hsvToRgb(
			ld::rand(0.0f, 360.0f),
			ld::saturation,
			ld::value));
	#endif /* !__ANDROID__ */

	submitScore(ld::playerName, mScore);
	// Preload both sets of scores
	getScores(true);
	getScores(false);
}

#ifndef __ANDROID__
	void GameStateScores::select(Text* ptr)
	{
		ptr->setColor(ld::hsvToRgb(
			ld::rand(0.0f, 360.0f),
			ld::saturation,
			ld::value));
	}

	void GameStateScores::deselect(Text* ptr)
	{
		ptr->setColor(sf::Color(255, 255, 255));
	}

	void GameStateScores::selectOption()
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
#endif /* !__ANDROID__ */

std::string GameStateScores::createSubmission(const std::string& name, unsigned long score)
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

bool GameStateScores::submitScore(const std::string& name, unsigned long score)
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

bool GameStateScores::getScores(bool topScores)
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
			tText.setPosition(ld::gameDimFullWidth*0.5f, ld::gameDimFullHeight*(0.9f+i*0.14f)/5.0f);
			tText.setScale(0.08f, 0.08f);
			#ifdef __ANDROID__
				tText.update();
			#endif /* __ANDROID__ */
			if(topScores)
				mTopScores.push_back(score);
			else
				mRelativeScores.push_back(score);
		}

		return true;
	}

	return false;
}

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

void GameStateScores::draw(sf::RenderTarget& target, sf::RenderStates states) const
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

void GameStateScores::onResize()
{
	mTextTitle.setPosition(ld::gameDimFullWidth/2.0f, ld::gameDimFullHeight*0.5f/5.0f);
	mTextRestart.setPosition(ld::gameDimFullWidth/2.0f, ld::gameDimFullHeight*4.0f/5.0f);
	mTextBoardSetter.setPosition(ld::gameDimFullWidth/2.0f, ld::gameDimFullHeight*4.4f/5.0f);
	mTextQuit.setPosition(ld::gameDimFullWidth/2.0f, ld::gameDimFullHeight*4.8f/5.0f);

	#ifdef __ANDROID__
		mTextTitle.update();
		mTextRestart.update();
		mTextBoardSetter.update();
		mTextQuit.update();
	#endif /* __ANDROID__ */

	for(size_t i = 0; i < mTopScores.size(); ++i)
	{
		std::get<2>(mTopScores[i]).setPosition(ld::gameDimFullWidth*0.5f, ld::gameDimFullHeight*(0.9f+i*0.28f)/5.0f);
		#ifdef __ANDROID__
			std::get<2>(mTopScores[i]).update();
		#endif /* __ANDROID__ */
	}
	for(size_t i = 0; i < mRelativeScores.size(); ++i)
	{
		std::get<2>(mRelativeScores[i]).setPosition(ld::gameDimFullWidth*0.5f, ld::gameDimFullHeight*(0.9f+i*0.28f)/5.0f);
		#ifdef __ANDROID__
			std::get<2>(mRelativeScores[i]).update();
		#endif /* __ANDROID__ */
	}
}
