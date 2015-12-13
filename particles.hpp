#ifndef PARTICLES_HPP
#define PARTICLES_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

#include "util.hpp"

class Particle : public sf::Drawable, public sf::Transformable
{
	friend class ParticleTracker;

private:

	// Size of particle (squares only)
	float mDim;
	// Linear velocity
	sf::Vector2f mV;
	// Angular velocity (rads per sec)
	float mW;
	// Current lifetime of the particle in seconds
	float mLifetime;
	// Maximum lifetime (for alpha transitions)
	float mMaxLifetime;

	// Particle appearance
	sf::RectangleShape mBody;
	sf::Color mCol;

	void update(float dt)
	{
		if(mLifetime > 0.0f)
		{
			mLifetime -= dt;
			move(dt * mV);
			rotate(dt * mW);
			// Simple linear fade
			mCol.a = 255.0f * (mLifetime / mMaxLifetime);
			mBody.setFillColor(mCol);
		}
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(mBody, states);
	}

public:

	Particle(float dim, const sf::Vector2f& v,
		float w, float lifetime, const sf::Color& col) :
		mDim(dim),
		mV(v),
		mW(w),
		mLifetime(lifetime),
		mMaxLifetime(lifetime),
		mBody(sf::Vector2f(dim, dim)),
		mCol(col)
	{
		mBody.setFillColor(col);
		mBody.setOutlineThickness(0.0f);
	}

	bool isDead() const
	{
		return mLifetime <= 0.0f;
	}
};

class ParticleTracker : public sf::Drawable
{
private:

	std::vector<Particle> mParticles;

public:

	ParticleTracker() {}

	// Spawn n particles of size dim with linear velocity v
	// in a random direction, random angular velocity from
	// pi/2 to 4pi, color col, from position pos, with
	// lifetime lifetime
	void spawn(size_t n, float v, float dim, const sf::Color& col,
		const sf::Vector2f pos, float lifetime)
	{
		for(size_t i = 0; i < n; ++i)
		{
			float theta = ld::rand(0, 2*M_PI);
			Particle p(dim,
				sf::Vector2f(v*cos(theta), v*sin(theta)),
				ld::rand(M_PI/2, 4*M_PI),
				lifetime,
				col);
			p.setPosition(pos);
			mParticles.push_back(p);
		}
	}

	void update(float dt)
	{
		// Update each particle
		for(auto& p : mParticles)
		{
			p.update(dt);
		}
		// Delete the dead ones
		mParticles.erase(
			std::remove_if(mParticles.begin(), mParticles.end(), [](Particle& p) { return p.isDead(); }),
			mParticles.end());
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// Draw each particle. Called after update, so all particles are alive
		for(auto& p : mParticles)
		{
			target.draw(p, states);
		}
	}
};

#endif /* PARTICLES_HPP */
