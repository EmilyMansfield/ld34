#ifndef GAME_STATE_GAME_HPP
#define GAME_STATE_GAME_HPP

#include "game_state.hpp"

class GameStateGame : public GameState
{

public:

	GameStateGame(std::shared_ptr<GameState> state,
		std::shared_ptr<GameState> prevState) :
		GameState(state, prevState)
	{
	}

	virtual void handleEvent(const sf::Event& event);
	virtual void handleInput(float dt);
	virtual void update(float dt);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{

	}
};

#endif /* GAME_STATE_GAME_HPP */
