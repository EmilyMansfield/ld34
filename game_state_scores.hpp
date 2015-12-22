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

	void select(Text* ptr);
	void deselect(Text* ptr);
	void selectOption();

	std::string createSubmission(const std::string& name, unsigned long score);
	bool submitScore(const std::string& name, unsigned long score);
	bool getScores(bool topScores);

public:

	GameStateScores(std::shared_ptr<GameState>& state,
		std::shared_ptr<GameState>& prevState,
		unsigned long score);

	virtual void handleEvent(const sf::Event& event);
	virtual void handleInput(float dt);
	virtual void update(float dt);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void onResize();
};

#endif /* GAME_STATE_SCORES_HPP */
