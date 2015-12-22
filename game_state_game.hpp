#ifndef GAME_STATE_GAME_HPP
#define GAME_STATE_GAME_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>

#include "game_state.hpp"
#include "player.hpp"
#include "projectile.hpp"
#include "constants.hpp"
#include "text.hpp"
#include "particles.hpp"

class GameStateGame : public GameState
{
private:

	enum class SubState {PLAY, PAUSE, TRANSITIONING, TRANSITIONED, DYING};

	Player mPlayer;
	float mNextGen;
	float mT;
	float mT2;
	float mTrailEmissionInterval;
	std::vector<Projectile> mProjectiles;
	ParticleTracker mParticles;
	SubState mSubstate;
	// SubState mPrevSubstate; // Used for resuming

	#ifndef __ANDROID__
		sf::SoundBuffer mHitSoundBufGood;
		sf::SoundBuffer mHitSoundBufBad;
		sf::SoundBuffer mDeadSoundBuf;
		sf::Sound mHitSound;
		sf::Sound mDeadSound;
	#endif /* !__ANDROID__ */

	#ifdef __ANDROID__
		sf::FloatRect mTouchBoundsLeft;
		sf::FloatRect mTouchBoundsRight;
		Text mTextCCWArrow;
		Text mTextCWArrow;
	#endif /* __ANDROID__ */

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
	float dirToFacing(float dir);
	float getLevel() const;
	float getGenerationInterval(int level) const;

public:

	GameStateGame(std::shared_ptr<GameState>& state,
		std::shared_ptr<GameState>& prevState);

	virtual void handleEvent(const sf::Event& event);
	virtual void handleInput(float dt);
	virtual void update(float dt);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual bool fullscreen()
	{
		return false;
	}
};

#endif /* GAME_STATE_GAME_HPP */
