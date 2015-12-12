#ifndef GAME_STATE_GAME_HPP
#define GAME_STATE_GAME_HPP

#include "game_state.hpp"
#include "player.hpp"

class GameStateGame : public GameState
{
private:

	Player mPlayer;

public:

	GameStateGame(std::shared_ptr<GameState> state,
		std::shared_ptr<GameState> prevState) :
		GameState(state, prevState),
		mPlayer(1.0f)
	{
		mPlayer.setPosition(15.0f/2.0, 15.0f/2.0);
	}

	virtual void handleEvent(const sf::Event& event);
	virtual void handleInput(float dt);
	virtual void update(float dt);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(mPlayer, states);
	}
};

#endif /* GAME_STATE_GAME_HPP */
