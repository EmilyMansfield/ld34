#ifndef GAME_STATE_TITLE_HPP
#define GAME_STATE_TITLE_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "game_state.hpp"
#include "constants.hpp"
#include "text.hpp"

class GameStateTitle : public GameState
{
private:

	Text mTextTitle;
	Text mTextPlay;
	Text mTextSettings;

public:

	GameStateTitle(std::shared_ptr<GameState> state,
		std::shared_ptr<GameState> prevState) :
		GameState(state, prevState),
		mTextTitle(ld::gameName),
		mTextPlay("Play"),
		mTextSettings("Settings")
	{
		mTextTitle.setPosition(ld::gameDim/2.0f, ld::gameDim/5.0f);
		mTextTitle.setOrigin(ld::gameName.size() * 5 * 0.5f, 1 * 6 * 0.5f);
		mTextTitle.setScale(0.3f, 0.3f);
		mTextPlay.setPosition(ld::gameDim/2.0f, ld::gameDim/2.0f);
		mTextPlay.setOrigin(4 * 5 * 0.5f, 1 * 6 * 0.5f);
		mTextPlay.setScale(0.2f, 0.2f);
		mTextSettings.setPosition(ld::gameDim/2.0f, ld::gameDim*3/4.0f);
		mTextSettings.setOrigin(8 * 5 * 0.5f, 1 * 6 * 0.5f);
		mTextSettings.setScale(0.2f, 0.2f);
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
