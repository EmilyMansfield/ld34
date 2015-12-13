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
extern const char* deadSoundPath;
extern const std::string textAbout;
extern const std::string textAbout2;
extern const std::string textAuthor;
extern const std::string textAuthor2;
extern bool musicAvailable;
extern bool musicOn;
extern const int leaderboardKey[];
extern const int leaderboardKeyLength;
extern const std::string leaderboardAuthentication;
}

#endif /* CONSTANTS_HPP */
