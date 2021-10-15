#include "Player.h"

Player::Player() :
	m_MAX_SPEED{10.0f},
	m_PI{ 3.14159265359f },
	m_position{0,0},
	m_speed{0.0f},
	m_rotation{0.0f},
	m_rotateSpeed{0.0f},
	m_angleRadians{0.0f},
	m_turn{3.0f}
{
	if (!m_texture.loadFromFile("ASSETS/ufo.png"))
	{
		std::cout << "Image load failed" << std::endl;
	}
	m_player.setTexture(m_texture);
	m_player.setOrigin(m_player.getGlobalBounds().width / 2, m_player.getGlobalBounds().height / 2);

	srand(time(nullptr));
}

Player::~Player()
{
}

void Player::increase()
{
	
	if (m_rotateSpeed <= m_MAX_SPEED)
	{
		m_rotateSpeed = m_rotateSpeed + 0.5;
	}
}

void Player::decrease()
{
	if (m_rotateSpeed > 0)
	{
		m_rotateSpeed = m_rotateSpeed - 0.5;
	}
}

void Player::update(sf::RenderWindow const & t_window, sf::Time dt)
{
	m_player.setRotation(m_rotation);

	m_angleRadians = m_rotation * (3.14 / 180);
	m_velocity.x = cosf(m_angleRadians) * m_rotateSpeed;
	m_velocity.y = sinf(m_angleRadians) * m_rotateSpeed;

	m_position += m_velocity;


	if (m_position.x > t_window.getSize().x + m_player.getScale().x)
	{
		m_position.x = 0 - m_player.getScale().x / 2;
	}
	else if (m_position.x < 0 - m_player.getScale().x)
	{
		m_position.x = t_window.getSize().x + m_player.getScale().x / 2;
	}

	if (m_position.y > t_window.getSize().y + m_player.getScale().y)
	{
		m_position.y = 0 - m_player.getScale().y / 2;
	}
	else if (m_position.y < 0 - m_player.getScale().y)
	{
		m_position.y = t_window.getSize().y + m_player.getScale().y / 2;
	}


	m_player.setPosition(m_position);
}

void Player::increaseRotation()
{
	m_rotation += m_turn;
	if (m_rotation == 360.0)
	{
		m_rotation = 0;
	}
}

void Player::decreaseRotation()
{
	m_rotation -= m_turn;
	if (m_rotation == 0.0)
	{
		m_rotation = 359.0;
	}
}
