#include "player.hpp"

sf::Color hsvToRgb(float h, float s, float v)
{
	// Thanks Wikipedia
	float c = v * s;
	float h2 = h / 60.0f;
	float x = c*(1-fabs(remainder(h2, 2.0f)-1));
	sf::Color offset(v-c, v-c, v-c);
	if(h < 1) return sf::Color(c,x,0) + offset;
	else if(h < 2) return sf::Color(x,c,0) + offset;
	else if(h < 3) return sf::Color(0,c,x) + offset;
	else if(h < 4) return sf::Color(0,x,c) + offset;
	else if(h < 5) return sf::Color(x,0,c) + offset;
	else if(h < 6) return sf::Color(c,0,x) + offset;
	else return sf::Color(0,0,0);
}

sf::Color hsvToRgb(const sf::Vector3f& hsv)
{
	return hsvToRgb(hsv.x, hsv.y, hsv.z);
}
