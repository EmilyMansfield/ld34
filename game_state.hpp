#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

class GameState : public sf::Drawable
{
public:
	
	virtual void handleEvent(const sf::Event& event) = 0;
	virtual void handleInput(float dt) = 0;
	virtual void update(float dt) = 0;
	virtual void draw(const sf::RenderTarget& target, sf::RenderStates states) const = 0;

	virtual ~GameState();	
};

#endif /* GAME_STATE_HPP */
