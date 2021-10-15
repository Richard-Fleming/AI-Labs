#pragma once
#include <SFML/Graphics.hpp>
#include "MyVector3.h"
#include <iostream>
#include "Steering.h"
class Wanderer
{
public:
	Wanderer();
	~Wanderer();

	void update(sf::RenderWindow const& t_window);
	Steering getSteering(MyVector3 t_target);

	sf::Sprite m_shape;
	sf::Texture m_texture;
	MyVector3 m_velocity;
	MyVector3 m_position;
	MyVector3 targetPosition;
	sf::Clock directionTimer;

	float const m_MAX_SPEED;
	float m_angleRadians;
	float m_turn;
	float m_rotation;
	float targetRotation;
	float m_rotateSpeed;
	float m_timer;
};

