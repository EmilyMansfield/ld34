#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SFML/Graphics.hpp>

namespace ld
{
// Screw bad practice
extern const unsigned int width;
extern const unsigned int height;
extern float gameDim;
extern const float saturation;
extern const float value;
extern const sf::Color playerCol;
extern const char* title;
extern const sf::Keyboard::Key buttonLeft;
extern const sf::Keyboard::Key buttonRight;
extern const char* fontName;
extern const unsigned int fontPixmap[];
}

#endif /* CONSTANTS_HPP */
