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
extern const sf::Color backgroundCol;
extern const char* title;
extern const sf::Keyboard::Key buttonLeft;
extern const sf::Keyboard::Key buttonRight;
extern const unsigned int fontPixmap[];
extern const float levelTimers[];
extern const bool hueCycling;
extern const std::string gameName;
extern const char* musicPath;
extern const char* hitSoundGoodPath;
extern const char* hitSoundBadPath;
}

#endif /* CONSTANTS_HPP */
