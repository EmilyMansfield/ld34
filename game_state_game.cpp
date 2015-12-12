#include "game_state_game.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <algorithm>

#include "util.hpp"

void GameStateGame::handleEvent(const sf::Event& event)
{
	if(event.type == sf::Event::KeyPressed)
	{
		if(event.key.code == ld::buttonLeft)
		{
			mPlayer.rotate(-1);
		}
		else if(event.key.code == ld::buttonRight)
		{
			mPlayer.rotate(1);
		}
	}
}

void GameStateGame::handleInput(float dt)
{

}

void GameStateGame::update(float dt)
{
	// Generate new projectiles if necessary
	mT += dt;
	if(mT >= mNextGen)
	{
		mT = 0.0f;
		mNextGen = ld::rand(mGenerationInterval/2.0f, mGenerationInterval*3.0f/2.0f);
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
	// std::cout << "Player is " << playerBounds.left << " " << playerBounds.width << " "
		// << playerBounds.top << " " << playerBounds.height << std::endl;
	for(auto& projectile : mProjectiles)
	{
		projectile.update(dt);
		if(playerBounds.intersects(projectile.bounds()))
		{
			projectile.kill();
		}
	}
	// Remove dead projectiles
	std::remove_if(mProjectiles.begin(), mProjectiles.end(), [](Projectile& p) { return p.isDead(); });

	mPlayer.update(dt);
}

