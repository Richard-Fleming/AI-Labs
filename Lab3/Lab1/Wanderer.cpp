#include "Wanderer.h"

Wanderer::Wanderer() :
	m_MAX_SPEED{10.0f},
	m_position{ 0,0,0 },
	m_angleRadians{0.0f},
	m_rotateSpeed{0.0f},
	m_rotation{0.0f},
	m_turn{3.0f},
	m_timer{2.0f}
{
	if (!m_texture.loadFromFile("ASSETS/alien.png"))
	{
		std::cout << "Image load failed" << std::endl;
	}
	m_shape.setTexture(m_texture);
	m_shape.setOrigin(m_shape.getScale().x / 2, m_shape.getScale().y / 2);

	srand(time(nullptr));
	
}

Wanderer::~Wanderer()
{
}

void Wanderer::update(sf::RenderWindow const& t_window)
{
	m_shape.setPosition(m_position += m_velocity);
	if (m_position.x > t_window.getSize().x + m_shape.getScale().x)
	{
		m_position.x = 0 - m_shape.getScale().x / 2;
	}
	else if (m_position.x < 0 - m_shape.getScale().x)
	{
		m_position.x = t_window.getSize().x + m_shape.getScale().x / 2;
	}

	if (m_position.y > t_window.getSize().y + m_shape.getScale().y)
	{
		m_position.y = 0 - m_shape.getScale().y / 2;
	}
	else if (m_position.y < 0 - m_shape.getScale().y)
	{
		m_position.y = t_window.getSize().y + m_shape.getScale().y / 2;
	}
}

Steering Wanderer::getSteering(MyVector3 t_target)
{
	Steering steering;
	m_rotation += rand() % 2 - 1 * m_turn;
	return steering;
}
