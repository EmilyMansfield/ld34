#ifndef GAME_STATE_GAME_HPP
#define GAME_STATE_GAME_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "game_state.hpp"
#include "player.hpp"
#include "projectile.hpp"
#include "constants.hpp"
#include "text.hpp"

class GameStateGame : public GameState
{
private:

	Player mPlayer;
	float mNextGen;
	float mT;
	std::vector<Projectile> mProjectiles;

	Text mTextScore;

	float mDuration;

	// Projectile angle to player side is
	// north = (0,90)    = 0 => south = 2
	// west =  (90,180)  = 1 => east  = 1
	// south = (180,270) = 2 => north = 0
	// east =  (270,360) = 3 => west  = 3
	float dirToFacing(float dir)
	{
		const static int map[4] = { 2, 1, 0, 3 };
		return map[static_cast<int>(dir / 90.0f)];
	}

	float getGenerationInterval() const
	{
		// Simple difficulty progression
		if(mDuration < 10)
			return 2.0;
		else if(mDuration < 30)
			return 1.2;
		else if(mDuration < 60)
			return 0.8;
		else
			return 0.5;
	}

public:

	GameStateGame(std::shared_ptr<GameState> state,
		std::shared_ptr<GameState> prevState) :
		GameState(state, prevState),
		mPlayer(1.0f),
		mNextGen(2.0f),
		mT(0.0f),
		mTextScore("0"),
		mDuration(0.0f)
	{
		mPlayer.setPosition(ld::gameDim/2.0f, ld::gameDim/2.0f);
		mTextScore.setPosition(ld::gameDim*5.0f/6.0f, ld::gameDim/15.0f);
		mTextScore.setScale(0.1f, 0.1f);
	}

	virtual void handleEvent(const sf::Event& event);
	virtual void handleInput(float dt);
	virtual void update(float dt);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(mPlayer, states);
		// Helps if I draw the damn things...
		for(auto& projectile : mProjectiles) target.draw(projectile, states);
		target.draw(mTextScore, states);
	}
};

#endif /* GAME_STATE_GAME_HPP */
