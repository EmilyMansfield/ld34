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
	Text mTextName;
	Text mTextAbout;
	Text mTextAbout2;
	Text mTextAuthor;
	Text mTextAuthor2;

	int mSelectedOption;

	std::string mNameStr;

	void select(Text* ptr);
	void deselect(Text* ptr);
	void selectOption();

public:

	GameStateSettings(std::shared_ptr<GameState>& state,
		std::shared_ptr<GameState>& prevState);

	virtual void handleEvent(const sf::Event& event);
	virtual void handleInput(float dt);
	virtual void update(float dt);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void onResize();
};

#endif /* GAME_STATE_SETTINGS_HPP */
