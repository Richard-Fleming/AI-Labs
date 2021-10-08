#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
class NPC
{
public:
	NPC();
	~NPC();

	void update(sf::RenderWindow const& t_window);

	sf::Sprite m_npc;
	sf::Texture m_texture;
	sf::Vector2f m_velocity;
	sf::Vector2f m_position;
};

