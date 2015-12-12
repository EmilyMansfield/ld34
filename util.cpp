#include "util.hpp"

#include <cstdlib>
#include <cmath>
#include <SFML/Graphics.hpp>

float ld::rand(float a, float b)
{
	return a + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (b-a);
}

sf::Color ld::hsvToRgb(float h, float s, float v)
{
	// Thanks Wikipedia
	float c = v * s;
	float h2 = h / 60.0f;
	float x = c*(1-fabs(fmod(h2, 2.0f)-1));
	sf::Color offset(
		255.0f * (v-c),
		255.0f * (v-c),
		255.0f * (v-c));
	     if(h2 < 1) return sf::Color(255.0f*c,255.0f*x,255.0f*0) + offset;
	else if(h2 < 2) return sf::Color(255.0f*x,255.0f*c,255.0f*0) + offset;
	else if(h2 < 3) return sf::Color(255.0f*0,255.0f*c,255.0f*x) + offset;
	else if(h2 < 4) return sf::Color(255.0f*0,255.0f*x,255.0f*c) + offset;
	else if(h2 < 5) return sf::Color(255.0f*x,255.0f*0,255.0f*c) + offset;
	else if(h2 < 6) return sf::Color(255.0f*c,255.0f*0,255.0f*x) + offset;
	else return sf::Color(0,0,0);
}

sf::Color ld::hsvToRgb(const sf::Vector3f& hsv)
{
	return hsvToRgb(hsv.x, hsv.y, hsv.z);
}
