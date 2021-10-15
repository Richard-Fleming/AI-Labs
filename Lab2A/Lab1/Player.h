#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
class Player
{
public:
	Player();
	~Player();
	void increase();
	void decrease();
	void update(sf::RenderWindow const& t_window, sf::Time dt);
	void increaseRotation();
	void decreaseRotation();

	sf::Sprite m_player;
	sf::Texture m_texture;
	sf::Vector2f m_velocity;
	sf::Vector2f m_position;
	float m_speed;
	float m_rotateSpeed;
	float const m_MAX_SPEED;
	float const m_PI;
	float m_angleRadians;
	float m_turn;
	float m_rotation;
};

