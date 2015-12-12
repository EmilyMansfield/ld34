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
public:

	Projectile(float dim, const sf::Vector2f& pos, float dir,
		float speed, const sf::Color& col) :
		mDim(dim),
		mBody(sf::Vector2f(dim, dim)),
		mDir(dir),
		mSpeed(speed),
		mCol(col)
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
};

#endif /* PROJECTILE_HPP */
