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
			if(event.key.code == sf::Keyboard::P)
			{
				mSubstate = mPrevstate;
			}
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
			else if(event.key.code == sf::Keyboard::P)
			{
				mPrevstate = mSubstate;
				mSubstate = SubState::PAUSE;
			}
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
				if(mCurrentLevel % 2 == 1)
				{
					mPlayer.setAlpha(mPlayer.numSlots()-1, mTransitionTimer / mTransitionLength);
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
				if(mCurrentLevel % 2 == 1)
				{
					mPlayer.addSlot();
					mPlayer.setAlpha(mPlayer.numSlots()-1, 0);
				}
			}
		}

		// Generate new projectiles if necessary
		mT += dt;
		if(mT >= mNextGen && mSubstate != SubState::TRANSITIONED)
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
			sf::FloatRect prBounds = projectile.bounds();
			// If transitioning, change the colour of all the projectiles
			if(mSubstate == SubState::TRANSITIONING && ld::hueCycling)
			{
				projectile.cycleHue(50.0f*dt);
			}
			projectile.update(dt);
			if(!projectile.isDead() && playerBounds.intersects(prBounds))
			{
				projectile.kill();

				sf::Color c1 = projectile.getCol();
				sf::Color c2 = mPlayer.colOnSide(dirToFacing(projectile.getDir()));
				if(abs(c1.r-c2.r) < 5 && abs(c1.g-c2.g) < 5 && abs(c1.b-c2.b) < 5)
				{
					mPlayer.score += 100;
				}
				else
				{
				}
			}
		}
		// Remove dead projectiles
		mProjectiles.erase(
			std::remove_if(mProjectiles.begin(), mProjectiles.end(), [](Projectile& p) { return p.isDead(); }),
			mProjectiles.end());

		mPlayer.update(dt);

		// Set the score text
		std::string scoreStr = std::to_string(mPlayer.score);
		mTextScore.setString(scoreStr);
		mTextScore.setPosition(ld::gameDim - (scoreStr.size()+1) * 5 * 0.1f, ld::gameDim/15.0f);
		mTextScore.setString(std::to_string(mPlayer.score));
	}
}

