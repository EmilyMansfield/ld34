#ifndef TEXT_HPP
#define TEXT_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "constants.hpp"

class Text : public sf::Transformable, public sf::Drawable
{
private:

	std::string mStr;
	std::vector<sf::RectangleShape> mRects;
	sf::Color mCol;
	#ifdef __ANDROID__
		sf::RectangleShape mRect;
		sf::FloatRect mBounds;
	#endif /* __ANDROID__ */

public:

	void update()
	{
		mRects.clear();

		// To construct the string, iterate over each character
		// and map it to a pixmap location
		for(size_t ci = 0; ci < mStr.size(); ++ci)
		{
			// TODO: Lookup table
			char c = mStr[ci];
			size_t i = 0;
			if(c >= '0' && c <= '9') i = c - '0';
			else if(c == ' ') i = 36;
			else if(c >= 'A' && c <= 'Z') i = c - 'A' + 10;
			else if(c >= 'a' && c <= 'z') i = c - 'a' + 10;
			else if(c == '.') i = 37;
			else if(c == ',') i = 38;
			else if(c == '!') i = 39;
			else if(c == '?') i = 40;
			else if(c == '@') i = 41;
			else if(c == '^') i = 42;
			else if(c == '(' || c == '[') i = 43;
			else if(c == ')' || c == ']') i = 44;
			else i = 36;

			// Get pixmap representation
			unsigned int pix = ld::fontPixmap[i];

			// Iterate over bits and convert to a bunch of rectangles
			float x = 0, y = 0;
			for(int b = 0; b < 30; ++b)
			{
				if(b % 5 == 0)
				{
					x = 0;
					y += 1;
				}
				if(((pix >> b) & 1) == 0)
				{
					// Bits are reversed, so place a rectangle here
					sf::RectangleShape r(sf::Vector2f(1.0f, 1.0f));
					r.setPosition(ci * 5 + x, y);
					r.setFillColor(mCol);
					r.setOutlineThickness(0.0f);
					mRects.push_back(r);
				}
				x += 1;
			}
		}

		#ifdef __ANDROID__
			mRect = sf::RectangleShape(sf::Vector2f(5 * mStr.size(), 6));
			mRect.setOrigin(getOrigin());
			mRect.setPosition(getPosition());
			mRect.setScale(getScale());
			mRect.setRotation(getRotation());
			mRect.setFillColor(ld::hsvToRgb(ld::rand(0, 360), ld::saturation, ld::value));
			mBounds = mRect.getGlobalBounds();
		#endif /* __ANDROID__ */
	}


	Text() {} // For a tuple when reading scores
	Text(const std::string& str, const sf::Color& col = sf::Color::White) :
		mStr(str),
		mCol(col)
	{			
		update();
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		for(auto& rect : mRects) target.draw(rect, states);
	}

	void setString(const std::string& str)
	{
		mStr = str;
		update();
	}

	void setColor(const sf::Color& col)
	{
		mCol = col;
		for(auto& r : mRects) r.setFillColor(col);
	}

	void setAlpha(float a)
	{
		if(a > 1.0) a = 1.0f;
		if(a < 0.0) a = 0.0f;
		mCol.a = 255 * a;
		setColor(mCol);
	}

	#ifdef __ANDROID__
		sf::FloatRect& getBounds()
		{
			return mBounds;
		}
	#endif /* __ANDROID__ */
};

#endif /* TEXT_HPP */
