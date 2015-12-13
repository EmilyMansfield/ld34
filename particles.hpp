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

	void spawn(
		size_t n,	// Number of particles
		float v,	// Magnitude of linear velocity
		float a,	// Angular bounds of velocity direction
		float b,	//
		float dim,	// Width and height of each particle
		float lifetime,
		const sf::Color& col,
		const sf::Vector2f pos)
	{
		for(size_t i = 0; i < n; ++i)
		{
			float theta = ld::rand(a, b);
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
