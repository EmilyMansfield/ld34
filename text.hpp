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
					r.setPosition(ci * 5 + x, 6 + y);
					r.setFillColor(sf::Color(255, 255, 255));
					r.setOutlineThickness(0.0f);
					mRects.push_back(r);
				}
				x += 1;
			}
		}
	}

public:

	Text(const std::string& str) :
		mStr(str)
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
};

#endif /* TEXT_HPP */
