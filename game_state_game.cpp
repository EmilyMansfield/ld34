#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include <string>

#include "game_state_game.hpp"
#include "util.hpp"
#include "constants.hpp"

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
				// Trail is in a fan of small angle behind the projectile,
				// equivalent to negative velocity in front of the projectile
				// North = (0, 90) => (1pi/4, 3pi/4)
				// West = (90, 180) => (3pi/4, 5pi/4)
				// South = (180, 270) => (5pi/4, 7pi/4)
				// East = (270, 360) => (7pi/4, 9pi/4)
				float a = projectile.getDir() / 90.0f * M_PI / 2.0f + M_PI / 4.0f;
				float b = a + M_PI / 2.0f;
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
				projectile.kill();

				sf::Color c1 = projectile.getCol();
				sf::Color c2 = mPlayer.colOnSide(dirToFacing(projectile.getDir()));
				if(abs(c1.r-c2.r) < 5 && abs(c1.g-c2.g) < 5 && abs(c1.b-c2.b) < 5)
				{
					mPlayer.score += 100;
					mHitSound.setBuffer(mHitSoundBufGood);
					mHitSound.play();
				}
				else
				{
					if(mPlayer.lives >= 1) mPlayer.lives -= 1;
					if(mPlayer.lives <= 0)
					{
						// Kill the player
						mDeadSound.setBuffer(mDeadSoundBuf);
						mDeadSound.play();
						mSubstate = SubState::DYING;
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
						mHitSound.setBuffer(mHitSoundBufBad);
						mHitSound.play();
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
		mTextScore.setPosition(ld::gameDim - (scoreStr.size()+1) * 5 * 0.1f, ld::gameDim/15.0f);

		// Set the lives text
		std::string livesStr(mPlayer.lives, '^');
		mTextLives.setString(livesStr);
		mTextLives.setPosition(ld::gameDim - (livesStr.size()+1) * 5 * 0.1f, 0.0f);
	}
}

