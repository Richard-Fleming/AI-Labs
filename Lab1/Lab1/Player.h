#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
class Player
{
public:
	Player();
	~Player();
	void increase();
	void decrease();
	void update(sf::RenderWindow const& t_window);

	sf::Sprite m_player;
	sf::Texture m_texture;
	sf::Vector2f m_velocity;
	sf::Vector2f m_position;
};

