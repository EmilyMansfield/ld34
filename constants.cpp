#include "constants.hpp"

const unsigned int ld::width = 800;
const unsigned int ld::height = 800;
float ld::gameDim = 15.0f;
const float ld::saturation = 0.85f;
const float ld::value = 0.97f;
const sf::Color ld::playerCol(0x33, 0x33, 0x33);
const char* ld::title = "ld34";
const sf::Keyboard::Key ld::buttonLeft = sf::Keyboard::Left;
const sf::Keyboard::Key ld::buttonRight = sf::Keyboard::Right;
const unsigned int ld::fontPixmap[] = {
	0x3f1ad6b1, 0x3fbdef7b, 0x3f1ec6f1, 0x3f1bc6f1, 0x3f78d6b5, 0x3f1bc7b1, 
	0x3f1ac7b1, 0x3f7bdef1, 0x3f1ac6b1, 0x3f78d6b1, 0x3f58d6b1, 0x3f1ac6b9, 
	0x3f1ef7b1, 0x3f1ad6b9, 0x3f1ec7b1, 0x3fdec7b1, 0x3f1ad7b1, 0x3f5ac6b5, 
	0x3fbdef7b, 0x3f1bdef3, 0x3f5ae6b5, 0x3f1ef7bd, 0x3ea52955, 0x3f5ad6b9, 
	0x3f1ad6b1, 0x3fd8d6b1, 0x3e1ad6b1, 0x3f5cd6b9, 0x3f1bc7b1, 0x3fbdef71, 
	0x3f1ad6b5, 0x3fbad6b5, 0x3f55294e, 0x3f5aeeb5, 0x3fbdd6b5, 0x3f1eeef1, 
	0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffffff, 0x0
};
const float ld::levelTimers[] = {
	10.0, 30.0f, 60.0f, 120.0f, 180.0f, 240.0f, 300.0f
};
const bool ld::hueCycling = false;
