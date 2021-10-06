#include "Player.h"

Player::Player() :
m_position{0,0}
{
	if (!m_texture.loadFromFile("ASSETS/ufo.png"))
	{
		std::cout << "Image load failed" << std::endl;
	}
	m_player.setTexture(m_texture);
	m_player.setOrigin(m_player.getScale().x / 2, m_player.getScale().y / 2);

	srand(time(nullptr));
}

Player::~Player()
{
}

void Player::increase()
{
	m_velocity.x *= 1.5f;
	m_velocity.y *= 1.5f;
}

void Player::decrease()
{
	m_velocity.x /= 1.5f;
	m_velocity.y /= 1.5f;
}

void Player::update(sf::RenderWindow const & t_window)
{
	m_player.setPosition(m_position += m_velocity);
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
