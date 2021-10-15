#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include "MyVector3.h"
#include "Steering.h"
class Seeker
{
public:
	Seeker();
	~Seeker();
	void update(sf::RenderWindow const& t_window);
	Steering getSteering(MyVector3 t_target);
	float getRotation(MyVector3 t_vector);
	float radianToDegrees(float t_radian);

	sf::Sprite m_shape;
	sf::Texture m_texture;
	MyVector3 m_position;
	float const m_MAX_SPEED;
	float const m_MIN_SPEED;
	float m_speed;
};

