#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

class Projectile : public sf::Transformable, public sf::Drawable
{
private:

	float mDim;
	sf::RectangleShape mBody;
	// Angle anticlockwise from vertical in degrees
	float mDir;
	// Speed of movement in game (arbitrary...) units
	float mSpeed;
	// Color of projectile. Should correspond to one of
	// the player's colours
	sf::Color mCol;
	// Used for deleting intersected projectiles
	bool mDead;

public:

	Projectile(float dim, const sf::Vector2f& pos, float dir,
		float speed, const sf::Color& col) :
		mDim(dim),
		mBody(sf::Vector2f(dim, dim)),
		mDir(dir),
		mSpeed(speed),
		mCol(col),
		mDead(false)
	{
		setPosition(pos);
		mBody.setFillColor(col);
		mBody.setOutlineThickness(0.0f);
		mBody.setOrigin(mDim/2.0f, mDim/2.0f);
	}

	void update(float dt)
	{
		// Angle ccw from vertical where y down is positive and
		// x right is positive so
		// x = -sin(angle)
		// y = -cos(angle)
		move(-sin(ld::toRad(mDir)) * dt * mSpeed,
			-cos(ld::toRad(mDir)) * dt * mSpeed);
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(mBody, states);
	}

	// Return bounds of projectile
	sf::FloatRect bounds() const
	{
		sf::Transform trans;
		trans.translate(getPosition()-mBody.getOrigin());
		return trans.transformRect(mBody.getLocalBounds());
	}

	// Mark as dead
	void kill()
	{
		mDead = true;
	}
	bool isDead() const
	{
		return mDead;
	}

	// Get the colour of the projectile
	sf::Color getCol() const
	{
		return mCol;
	}

	// Get direction of the projectile
	float getDir() const
	{
		return mDir;
	}
};

#endif /* PROJECTILE_HPP */
