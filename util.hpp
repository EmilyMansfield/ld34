#ifndef UTIL_HPP
#define UTIL_HPP

#include <SFML/Graphics.hpp>

namespace ld
{
// Return a random float between a and b
float rand(float a, float b);
// Convert a hsv colour to an SFML-compatible rgb version
sf::Color hsvToRgb(float h, float s, float v);
sf::Color hsvToRgb(const sf::Vector3f& hsv);
}

#endif /* UTIL_HPP */
