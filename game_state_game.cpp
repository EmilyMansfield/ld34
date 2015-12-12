#include "game_state_game.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <string>

#include "util.hpp"
#include "constants.hpp"

void GameStateGame::handleEvent(const sf::Event& event)
{
	if(event.type == sf::Event::KeyPressed)
	{
		if(mPaused)
		{
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
			else if(event.key.code == sf::Keyboard::Space)
			{
				mPlayer.addSlot();
			}
			else if(event.key.code == sf::Keyboard::S)
			{
				std::cout << mPlayer.score << std::endl;
			}
			else if(event.key.code == sf::Keyboard::D)
			{
				std::cout << mProjectiles.size() << std::endl;
			}			
		}
		if(event.key.code == sf::Keyboard::P)
		{
			mPaused = !mPaused;
		}
	}
}

void GameStateGame::handleInput(float dt)
{

}

void GameStateGame::update(float dt)
{
	if(mPaused)
	{	
	}
	else
	{	
		mDuration += dt;

		// Generate new projectiles if necessary
		mT += dt;
		if(mT >= mNextGen)
		{
			mT = 0.0f;
			mNextGen = getGenerationInterval();
			// Randomly generate an appearance direction and position
			float dir = floor(ld::rand(0, 4)) * 90.0f;
			sf::Vector2f pos;
			if(dir < 90.0f) // North so place at south
				pos = sf::Vector2f(ld::gameDim / 2.0f, ld::gameDim + 1.0f);
			else if(dir < 180.0f) // West so place at east
				pos = sf::Vector2f(ld::gameDim + 1.0f, ld::gameDim / 2.0f);
			else if(dir < 270.0f) // South so place at north
				pos = sf::Vector2f(ld::gameDim / 2.0f, -1.0f);
			else // East so place at west
				pos = sf::Vector2f(-1.0f, ld::gameDim / 2.0f);
			// Create the projectile
			mProjectiles.push_back(Projectile(1.0f/3.0f, pos, dir, 3.0f, mPlayer.sample()));
		}

		sf::FloatRect playerBounds = mPlayer.bounds();
		for(auto& projectile : mProjectiles)
		{
			sf::FloatRect prBounds = projectile.bounds();
			projectile.update(dt);
			if(!projectile.isDead() && playerBounds.intersects(prBounds))
			{
				projectile.kill();

				if(projectile.getCol() == mPlayer.colOnSide(dirToFacing(projectile.getDir())))
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

