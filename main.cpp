#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <memory>
#include <cstdlib>
#include <ctime>

#include "game_state.hpp"
#include "game_state_game.hpp"
#include "game_state_title.hpp"
#include "constants.hpp"

void updateView(const sf::Window& window, sf::View& view)
{
	double w = window.getSize().x, h = window.getSize().y;
	if(w < h)
	{
		view.setViewport(sf::FloatRect(0.0f, 0.5f*(1 - w/h), 1.0f, w/h));
	}
	else if(w > h)
	{
		view.setViewport(sf::FloatRect(0.5f*(1 - h/w), 0.0f, h/w, 1.0f));
	}
}

void updateFullscreenView(const sf::Window& window, sf::View& view)
{
	double w = window.getSize().x, h = window.getSize().y;
	if(w < h)
	{
		ld::gameDimFullWidth = ld::gameDim;
		ld::gameDimFullHeight = ld::gameDim * h/w;
		view.setSize(ld::gameDimFullWidth, ld::gameDimFullHeight);
		view.setCenter(ld::gameDimFullWidth*0.5f, ld::gameDimFullHeight*0.5f);
		view.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
	}
	else if(w > h)
	{
		ld::gameDimFullWidth = ld::gameDim * w/h;
		ld::gameDimFullHeight = ld::gameDim;
		view.setSize(ld::gameDimFullWidth, ld::gameDimFullHeight);
		view.setCenter(ld::gameDimFullWidth*0.5f, ld::gameDimFullHeight*0.5f);
		view.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
	}
}

int main()
{
	// Will use 'proper' C++ random if have time
	srand(time(nullptr));

	// Non-resizeable window
	#ifdef __ANDROID__
		sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "");
	#else
		sf::RenderWindow window(sf::VideoMode(ld::width, ld::height), 
			ld::title, sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);

		// Disable key repeating
		window.setKeyRepeatEnabled(false);

		// Clamp framerate
		window.setVerticalSyncEnabled(true);
	#endif /* __ANDROID__ */

	// Window viewport
	sf::View fullscreenView;
	sf::View view(sf::FloatRect(0, 0, ld::gameDim, ld::gameDim));
	bool fullscreen = true;
	updateView(window, view);
	updateFullscreenView(window, fullscreenView);
	window.setView(fullscreenView);

	#ifdef __ANDROID__
		ld::renderTarget = &window;
	#endif /* __ANDROID__ */

	#ifndef __ANDROID__
		// Start the music
		sf::Music music;
		if(music.openFromFile(ld::musicPath))
		{
			ld::musicAvailable = true;
			ld::musicOn = true;
			music.setLoop(true);
			music.setVolume(40.0f);
			music.play();
		}
	#endif /* !__ANDROID__ */

	// Pointer to current game state
	std::shared_ptr<GameState> state;

	// Load initial game state
	// state.reset(new GameStateGame(state, state));
	state.reset(new GameStateTitle(state, state));

	// Frame time
	sf::Clock clock;

	while(window.isOpen())
	{
		float dt = clock.restart().asSeconds();

		#ifndef __ANDROID__
			// Check music
			if(ld::musicAvailable && !ld::musicOn && music.getStatus() == sf::SoundSource::Playing)
			{
				ld::musicOn = false;
				music.stop();
			}
			else if(ld::musicAvailable && ld::musicOn && music.getStatus() == sf::SoundSource::Stopped)
			{
				ld::musicOn = true;
				music.play();
			}
		#endif /* !__ANDROID__ */

		// Handle events
		sf::Event event;
		while(window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if(event.type == sf::Event::Resized)
			{
				if(fullscreen)
				{
					updateFullscreenView(window, fullscreenView);
					window.setView(fullscreenView);
				}
				else
				{
					updateView(window, view);
					window.setView(view);
				}
			}
			if(state != nullptr) state->handleEvent(event);
		}

		// Handle view type change
		if(state->fullscreen() && !fullscreen)
		{
			updateFullscreenView(window, fullscreenView);
			window.setView(fullscreenView);
			fullscreen = true;
		}
		else if(!state->fullscreen() && fullscreen)
		{
			updateView(window, view);
			window.setView(view);
			fullscreen = false;
		}

		// Update window
		if(state != nullptr) state->handleInput(dt);
		if(state != nullptr) state->update(dt);

		// Draw window
		window.clear(ld::backgroundCol);
		if(state != nullptr) window.draw(*state);
		window.display();

		if(state == nullptr) window.close();
	}

	return 0;
}
