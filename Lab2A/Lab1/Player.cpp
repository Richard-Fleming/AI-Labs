#include "Player.h"

Player::Player() :
	m_MAX_SPEED{100.0f},
	m_PI{ 3.14159265359f },
	m_position{0,0},
	m_speed{0.0f},
	m_rotation{0.0f}
{
	if (!m_texture.loadFromFile("ASSETS/ufo.png"))
	{
		std::cout << "Image load failed" << std::endl;
	}
	m_player.setTexture(m_texture);
	m_player.setOrigin(m_player.getLocalBounds().width / 2, m_player.getLocalBounds().height / 2);

	srand(time(nullptr));
}

Player::~Player()
{
}

void Player::increase()
{
	if (m_speed < m_MAX_SPEED)
	{
		m_speed += 1.0f;
	}
}

void Player::decrease()
{
	if (m_speed > 0)
	{
		m_speed -= 1.0f;
	}
}

void Player::update(sf::RenderWindow const & t_window, sf::Time dt)
{
	m_player.setPosition((m_position.x + cos((m_rotation * m_PI) / 180.0f) * m_speed * (dt.asMilliseconds())), (m_position.y + sin((m_rotation * m_PI) / 180.0f) * m_speed * (dt.asMilliseconds())));

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
}

void Player::increaseRotation()
{
	m_rotation += 1;
	if (m_rotation == 360.0)
	{
		m_rotation = 0;
	}
}

void Player::decreaseRotation()
{
	m_rotation -= 1;
	if (m_rotation == 0.0)
	{
		m_rotation = 359.0;
	}
}
