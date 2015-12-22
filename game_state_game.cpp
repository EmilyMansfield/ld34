#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include <string>

#include "game_state_game.hpp"
#include "game_state_scores.hpp"
#include "util.hpp"
#include "constants.hpp"

GameStateGame::GameStateGame(std::shared_ptr<GameState>& state,
	std::shared_ptr<GameState>& prevState) :
	GameState(state, prevState),
	mPlayer(1.0f),
	mNextGen(2.0f),
	mT(0.0f),
	mT2(0.0f),
	mTrailEmissionInterval(0.1f),
	mSubstate(SubState::TRANSITIONING),
	#ifdef __ANDROID__
		mTouchBoundsLeft(0, ld::gameDim*4.0f/5.0f, ld::gameDim*0.5f, ld::gameDim*1.0f/5.0f),
		mTouchBoundsRight(ld::gameDim*0.5f, ld::gameDim*4.0f/5.0f, ld::gameDim*0.5f, ld::gameDim*1.0f/5.0f),
		mTextCCWArrow("<"),
		mTextCWArrow(">"),
	#endif /* __ANDROID__ */
	mTextScore("0"),
	mTextPause("PAUSED"),
	mTextLives("^"),
	mDuration(0.0f),
	mTransitionTimer(0.0),
	mTransitionLength(2.0f),
	mCurrentLevel(-1)
{
	mPlayer.setPosition(ld::gameDim/2.0f, ld::gameDim/2.0f);

	mTextScore.setPosition(ld::gameDim*5.0f/6.0f, ld::gameDim*1.0f/5.0f);
	mTextScore.setScale(0.1f, 0.1f);

	mTextLives.setString(std::string(mPlayer.lives, '^'));
	mTextLives.setPosition(ld::gameDim*5.0f/6.0f, ld::gameDim*0.5f/5.0f);
	mTextLives.setScale(0.1f, 0.1f);

	mTextPause.setPosition(ld::gameDim/2.0f, ld::gameDim/2.0f);
	mTextPause.setOrigin(6 * 5 * 0.5f, 1 * 6 * 0.5f);
	mTextPause.setScale(0.2f, 0.2f);

	#ifndef __ANDROID__
		// Load sounds
		mHitSoundBufGood.loadFromFile(ld::hitSoundGoodPath);
		mHitSoundBufBad.loadFromFile(ld::hitSoundBadPath);
		mDeadSoundBuf.loadFromFile(ld::deadSoundPath);
	#else
		// Set left and right button hints
		mTextCCWArrow.setPosition(ld::gameDim*0.25f, ld::gameDim*4.5f/5.0f);
		mTextCCWArrow.setOrigin(1 * 5 * 0.5f, 1 * 6 * 0.5f);
		mTextCCWArrow.setScale(0.25f, 0.25f);

		mTextCWArrow.setPosition(ld::gameDim*0.75f, ld::gameDim*4.5f/5.0f);
		mTextCWArrow.setOrigin(1 * 5 * 0.5f, 1 * 6 * 0.5f);
		mTextCWArrow.setScale(0.25f, 0.25f);
	#endif /* !__ANDROID__ */

	// Instantly triggers level -1 -> 0 which fades in a new slot
	// Projectiles are not fired until after the projectile is
	// created
	mTransitionTimer = 0;
	mT = -mTransitionLength;
	mPlayer.addSlot();
	mPlayer.setAlpha(mPlayer.numSlots()-1, 0);

	mTextLives.setColor(ld::hsvToRgb(mPlayer.sample()));
	mTextLives.setAlpha(0);
}

float GameStateGame::dirToFacing(float dir)
{
	const static int map[4] = { 2, 1, 0, 3 };
	return map[static_cast<int>(dir / 90.0f)];
}

float GameStateGame::getLevel() const
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

float GameStateGame::getGenerationInterval(int level) const
{
	if(mSubstate == SubState::TRANSITIONING && mCurrentLevel % 2 == 0) return 0.25f;
	static const float difficultyMap[] = {
		1.6, 1.4, 1.2, 1.0, 0.8, 0.65, 0.5
	};
	// Translates to BPM for music, have two beats per note?
	return difficultyMap[level];
}

#ifdef __ANDROID__
void GameStateGame::handleEvent(const sf::Event& event)
{
	if(event.type == sf::Event::TouchBegan)
	{
		sf::Vector2i p(event.touch.x, event.touch.y);
		sf::Vector2f touchPos(ld::renderTarget->mapPixelToCoords(p));
		if(mTouchBoundsLeft.contains(touchPos))
		{
			mPlayer.rotate(-1);
		}
		else if(mTouchBoundsRight.contains(touchPos))
		{
			mPlayer.rotate(1);
		}
	}
}
#else
void GameStateGame::handleEvent(const sf::Event& event)
{
	if(event.type == sf::Event::KeyPressed)
	{
		if(mSubstate == SubState::PAUSE)
		{
			// Only two buttons so no pause :(			
			// if(event.key.code == sf::Keyboard::P)
			// {
			// 	mSubstate = mPrevSubstate;
			// }
		}
		else
		{
			if(event.key.code == ld::buttonLeft)
			{
				mPlayer.rotate(-1);
			}
			else if(event.key.code == ld::buttonRight)
			{
				mPlayer.rotate(1);
			}
			// else if(event.key.code == sf::Keyboard::P)
			// {
			// 	mPrevSubstate = mSubstate;
			// 	mSubstate = SubState::PAUSE;
			// }
		}
	}
}
#endif /* __ANDROID__ */

void GameStateGame::handleInput(float dt)
{

}

void GameStateGame::update(float dt)
{
	static int lastDir = 0;
	static sf::Vector3f lastCol;

	if(mSubstate == SubState::PAUSE)
	{	
	}
	else
	{	
		mDuration += dt;

		// Handle level transitioning
		if(mSubstate == SubState::TRANSITIONING)
		{
			// Continue level transition
			if(getLevel() > mCurrentLevel)
			{
				mTransitionTimer += dt;
				// Odd level so fade in new slot
				if(abs(mCurrentLevel) % 2 == 1)
				{
					mPlayer.setAlpha(mPlayer.numSlots()-1, mTransitionTimer / mTransitionLength);
					// Adjust alpha of lives too if its the first slot
					if(mCurrentLevel < 0)
					{
						mTextLives.setAlpha(mTransitionTimer / mTransitionLength);
					}
				}
				if(mTransitionTimer >= mTransitionLength)
				{
					mSubstate = SubState::TRANSITIONED;
					mTransitionTimer = 0.0f;
					mCurrentLevel = getLevel();
				}
			}
		}
		else if(mSubstate == SubState::TRANSITIONED)
		{
			mTransitionTimer += dt;
			if(mTransitionTimer >= mTransitionLength / 2.0f)
			{
				// End transition and return to gameplay
				mSubstate = SubState::PLAY;
			}
		}
		else if(mSubstate == SubState::PLAY)
		{
			if(getLevel() > mCurrentLevel)
			{
				mSubstate = SubState::TRANSITIONING;
				mTransitionTimer = 0;
				// Odd level so add new slot
				if(abs(mCurrentLevel) % 2 == 1)
				{
					mPlayer.addSlot();
					mPlayer.setAlpha(mPlayer.numSlots()-1, 0);
				}
			}
		}
		else if(mSubstate == SubState::DYING)
		{
			mTransitionTimer += dt;
			if(mTransitionTimer >= mTransitionLength)
			{
				std::shared_ptr<GameState> thisState = mState;
				mState.reset(new GameStateScores(mState, thisState, mPlayer.score));
				return;
			}
		}

		// Generate new projectiles if necessary
		mT += dt;
		mT2 += dt;
		if(mT >= mNextGen && mSubstate != SubState::TRANSITIONED && mSubstate != SubState::DYING)
		{
			mT = 0.0f;
			mNextGen = getGenerationInterval(mCurrentLevel);
			// Randomly generate an appearance direction and position
			// Bias the direction towards 90 degree rotations from the
			// previous direction
			// If level transitioning then keep the direction the same
			float dir = 0.0f;
			if(mSubstate == SubState::TRANSITIONING && mCurrentLevel % 2 == 0) dir = lastDir;
			else if(mSubstate == SubState::PLAY)
			{
				dir = floor(ld::rand(0, 4)) * 90.0f;
				if(lastDir == dir && rand() % 2 == 0) dir = fmod(dir + 90.0f, 360.0f);
			}
			lastDir = dir;
			sf::Vector2f pos;
			if(dir < 90.0f) // North so place at south
				pos = sf::Vector2f(ld::gameDim / 2.0f, ld::gameDim + 1.0f);
			else if(dir < 180.0f) // West so place at east
				pos = sf::Vector2f(ld::gameDim + 1.0f, ld::gameDim / 2.0f);
			else if(dir < 270.0f) // South so place at north
				pos = sf::Vector2f(ld::gameDim / 2.0f, -1.0f);
			else // East so place at west
				pos = sf::Vector2f(-1.0f, ld::gameDim / 2.0f);
			sf::Vector3f col = mPlayer.sample();
			if(mSubstate == SubState::TRANSITIONING) col = lastCol;
			lastCol = col;
			// Create the projectile
			mProjectiles.push_back(Projectile(1.0f/3.0f, pos, dir, 3.0f, col));
		}

		// Cycle player hue if transitioning
		if(mSubstate == SubState::TRANSITIONING && ld::hueCycling)
		{
			mPlayer.cycleHue(50.0f*dt);
			lastCol.x = fmod(lastCol.x + 50.0f*dt, 360.0f);
		}

		// Handle behaviour of projectiles
		sf::FloatRect playerBounds = mPlayer.bounds();
		for(auto& projectile : mProjectiles)
		{
			// Spawn trail particles
			if(mT2 > mTrailEmissionInterval)
			{
				// Not anymore!
				// Trail is in a fan of small angle behind the projectile,
				// equivalent to negative velocity in front of the projectile
				// North = (0, 90) => (1pi/4, 3pi/4)
				// West = (90, 180) => (3pi/4, 5pi/4)
				// South = (180, 270) => (5pi/4, 7pi/4)
				// East = (270, 360) => (7pi/4, 9pi/4)
				// float a = projectile.getDir() / 90.0f * M_PI / 2.0f + M_PI / 4.0f;
				// float b = a + M_PI / 2.0f;
				mParticles.spawn(
					1,		// n
					0.0f,	// v
					0,		// a
					0,		// b
					1.0f/3.0f,	// dim
					0.2f,	// lifetime
					projectile.getCol(),
					projectile.getPosition(),
					false	// No rotation
					);
			}

			sf::FloatRect prBounds = projectile.bounds();
			// If transitioning, change the colour of all the projectiles
			if(mSubstate == SubState::TRANSITIONING && ld::hueCycling)
			{
				projectile.cycleHue(50.0f*dt);
			}
			projectile.update(dt);
			if(!projectile.isDead() && playerBounds.intersects(prBounds) && mSubstate != SubState::DYING)
			{
				sf::Color c1 = projectile.getCol();
				sf::Color c2 = mPlayer.colOnSide(dirToFacing(projectile.getDir()));
				if(abs(c1.r-c2.r) < 5 && abs(c1.g-c2.g) < 5 && abs(c1.b-c2.b) < 5)
				{
					mPlayer.score += 100;
					#ifndef __ANDROID__
						mHitSound.setBuffer(mHitSoundBufGood);
						mHitSound.play();
					#endif /* __ANDROID__ */
				}
				else
				{
					if(mPlayer.lives >= 1) mPlayer.lives -= 1;
					if(mPlayer.lives <= 0)
					{
						// Kill the player
						#ifndef __ANDROID__
							mDeadSound.setBuffer(mDeadSoundBuf);
							mDeadSound.play();
						#endif /* __ANDROID__ */
						mSubstate = SubState::DYING;
						mTransitionTimer = 0.0f;
						mParticles.spawn(9-mPlayer.numSlots(),
							1.7f,
							0.0f,
							2.0f*M_PI,
							1.0f/3.0f,
							0.6f,
							ld::playerCol,
							mPlayer.getPosition());
						for(size_t i = 0; i < mPlayer.numSlots(); ++i)
						{
							mParticles.spawn(1,
								1.7f,
								0.0f,
								2.0f*M_PI,
								1.0f/3.0f,
								0.6f,
								mPlayer.getCol(i),
								mPlayer.getPosition());
						}
					}
					else
					{
						#ifndef __ANDROID__
							mHitSound.setBuffer(mHitSoundBufBad);
							mHitSound.play();
						#endif /* __ANDROID__ */
					}

				}

				// Calculate possible particle projection angle to avoid intersection
				// with the player
				// North = 0 => (pi, 2pi)
				// East =  1 => (-pi/2, pi/2)
				// South = 2 => (0, pi)
				// West =  3 => (pi/2, 3pi/2)
				const static float angleMapA[] = {1.0f, -0.5f, 0.0f, 0.5f};
				const static float angleMapB[] = {2.0f,  0.5f, 1.0f, 1.5f};
				float a = angleMapA[static_cast<int>(dirToFacing(projectile.getDir()))] * M_PI;
				float b = angleMapB[static_cast<int>(dirToFacing(projectile.getDir()))] * M_PI;
				// Spawn particles
				mParticles.spawn(
					5,		// n
					1.7f,	// v
					a,		// a
					b,		// b
					0.15f,	// dim
					0.4f,	// lifetime
					c1,
					projectile.getPosition());

				projectile.kill();
			}
		}
		// Remove dead projectiles
		mProjectiles.erase(
			std::remove_if(mProjectiles.begin(), mProjectiles.end(), [](Projectile& p) { return p.isDead(); }),
			mProjectiles.end());

		// Reset trail particle timer
		if(mT2 > mTrailEmissionInterval)
			mT2 = 0.0f;

		// Update the player
		mPlayer.update(dt);

		// Update the particles
		mParticles.update(dt);

		// Set the score text
		std::string scoreStr = std::to_string(mPlayer.score);
		mTextScore.setString(scoreStr);
		mTextScore.setPosition(ld::gameDim - (scoreStr.size()+1) * 5 * 0.1f, ld::gameDim*0.75f/5.0f);

		// Set the lives text
		std::string livesStr(mPlayer.lives, '^');
		mTextLives.setString(livesStr);
		mTextLives.setPosition(ld::gameDim - (livesStr.size()+1) * 5 * 0.1f, ld::gameDim*0.5f/5.0f);
	}
}

void GameStateGame::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(mSubstate != SubState::DYING)
	{
		target.draw(mPlayer, states);
	}
	// Helps if I draw the damn things...
	for(auto& projectile : mProjectiles) target.draw(projectile, states);
	target.draw(mParticles, states);

	target.draw(mTextScore, states);
	target.draw(mTextLives, states);

	#ifdef __ANDROID__
		target.draw(mTextCCWArrow, states);
		target.draw(mTextCWArrow, states);
	#endif /* __ANDROID__ */

	if(mSubstate == SubState::PAUSE)
	{
		target.draw(mTextPause, states);
	}
}
