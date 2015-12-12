#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <cstdlib>
#include <ctime>

#include "game_state.hpp"
#include "game_state_game.hpp"
#include "constants.hpp"

int main()
{
	// Will use 'proper' C++ random if have time
	srand(time(nullptr));

	// Non-resizeable window
	sf::RenderWindow window(sf::VideoMode(ld::width, ld::height), 
		ld::title, sf::Style::Titlebar | sf::Style::Close);

	// Disable key repeating
	window.setKeyRepeatEnabled(false);

	// Window viewport
	sf::View view(sf::FloatRect(0, 0, ld::gameDim, ld::gameDim));
	view.setCenter(ld::gameDim/2.0f, ld::gameDim/2.0f);
	window.setView(view);

	// Pointer to current game state
	std::shared_ptr<GameState> state;

	// Load initial game state
	state.reset(new GameStateGame(state, state));

	// Frame time
	sf::Clock clock;

	while(window.isOpen())
	{
		float dt = clock.restart().asSeconds();

		// Handle events
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
			{
				window.close();
			}
			if(state != nullptr) state->handleEvent(event);
		}
		// Update window
		if(state != nullptr) state->handleInput(dt);
		if(state != nullptr) state->update(dt);

		// Draw window
		window.clear(sf::Color::Black);
		if(state != nullptr) window.draw(*state);
		window.display();
	}

	return 0;
}
