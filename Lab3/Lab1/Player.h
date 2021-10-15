#pragma once
#include <SFML/Graphics.hpp>
#include "MyVector3.h"
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
	MyVector3 m_velocity;
	MyVector3 m_position;
	float m_speed;
	float m_rotateSpeed;
	float const m_MAX_SPEED;
	float const m_PI;
	float m_angleRadians;
	float m_turn;
	float m_rotation;
};

