#ifndef GAME_STATE_GAME_HPP
#define GAME_STATE_GAME_HPP

#include <SFML/System.hpp>

#include "game_state.hpp"
#include "player.hpp"
#include "projectile.hpp"
#include "constants.hpp"

class GameStateGame : public GameState
{
private:

	Player mPlayer;
	float mGenerationInterval;
	float mNextGen;
	float mT;
	std::vector<Projectile> mProjectiles;

public:

	GameStateGame(std::shared_ptr<GameState> state,
		std::shared_ptr<GameState> prevState) :
		GameState(state, prevState),
		mPlayer(1.0f),
		mGenerationInterval(2.0f),
		mNextGen(2.0f),
		mT(0.0f)
	{
		mPlayer.setPosition(ld::gameDim/2.0, ld::gameDim/2.0);
	}

	virtual void handleEvent(const sf::Event& event);
	virtual void handleInput(float dt);
	virtual void update(float dt);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(mPlayer, states);
		// Helps if I draw the damn things...
		for(auto& projectile : mProjectiles) target.draw(projectile, states);
	}
};

#endif /* GAME_STATE_GAME_HPP */
