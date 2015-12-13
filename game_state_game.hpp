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

	enum class SubState {PLAY, PAUSE, TRANSITIONING, TRANSITIONED};

	Player mPlayer;
	float mNextGen;
	float mT;
	std::vector<Projectile> mProjectiles;
	SubState mSubstate;
	// SubState mPrevSubstate; // Used for resuming

	Text mTextScore;
	Text mTextPause;
	Text mTextLives;

	float mDuration;

	float mTransitionTimer;
	float mTransitionLength;
	int mCurrentLevel;

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

	float getLevel() const
	{
		if(mDuration < ld::levelTimers[0])
			return 0;
		else if(mDuration < ld::levelTimers[1])
			return 1;
		else if(mDuration < ld::levelTimers[2])
			return 2;
		else if(mDuration < ld::levelTimers[3])
			return 3;
		else if(mDuration < ld::levelTimers[4])
			return 4;
		else if(mDuration < ld::levelTimers[5])
			return 5;
		else
			return 6;
	}

	float getGenerationInterval(int level) const
	{
		if(mSubstate == SubState::TRANSITIONING && mCurrentLevel % 2 == 0) return 0.25f;
		static const float difficultyMap[] = {
			1.6, 1.2, 0.8, 0.5
		};
		return difficultyMap[(level+1)/2];
	}

public:

	GameStateGame(std::shared_ptr<GameState>& state,
		std::shared_ptr<GameState>& prevState) :
		GameState(state, prevState),
		mPlayer(1.0f),
		mNextGen(2.0f),
		mT(0.0f),
		mSubstate(SubState::PLAY),
		mTextScore("0"),
		mTextPause("PAUSED"),
		mTextLives("^"),
		mDuration(0.0f),
		mTransitionTimer(0.0),
		mTransitionLength(2.0f),
		mCurrentLevel(0)
	{
		mPlayer.setPosition(ld::gameDim/2.0f, ld::gameDim/2.0f);

		mTextScore.setPosition(ld::gameDim*5.0f/6.0f, ld::gameDim/15.0f);
		mTextScore.setScale(0.1f, 0.1f);

		mTextLives.setString(std::string(mPlayer.lives, '^'));
		mTextLives.setPosition(ld::gameDim*5.0f/6.0f, 0.0f);
		mTextLives.setScale(0.1f, 0.1f);

		mTextPause.setPosition(ld::gameDim/2.0f, ld::gameDim/2.0f);
		mTextPause.setOrigin(6 * 5 * 0.5f, 1 * 6 * 0.5f);
		mTextPause.setScale(0.2f, 0.2f);
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
		target.draw(mTextLives, states);

		if(mSubstate == SubState::PAUSE)
		{
			target.draw(mTextPause, states);
		}
	}
};

#endif /* GAME_STATE_GAME_HPP */
