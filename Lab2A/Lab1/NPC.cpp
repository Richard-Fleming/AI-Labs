#include "NPC.h"

NPC::NPC() :
	m_position{ 0,0 }
{
	if (!m_texture.loadFromFile("ASSETS/asteroid.png"))
	{
		std::cout << "Image load failed" << std::endl;
	}
	m_npc.setTexture(m_texture);
	m_npc.setOrigin(m_npc.getScale().x / 2, m_npc.getScale().y / 2);

	srand(time(nullptr));
}

NPC::~NPC()
{
}

void NPC::update(sf::RenderWindow const& t_window)
{
	m_npc.setPosition(m_position += m_velocity);
	if (m_position.x > t_window.getSize().x + m_npc.getScale().x)
	{
		m_position.x = 0 - m_npc.getScale().x / 2;
	}
	else if (m_position.x < 0 - m_npc.getScale().x)
	{
		m_position.x = t_window.getSize().x + m_npc.getScale().x / 2;
	}

	if (m_position.y > t_window.getSize().y + m_npc.getScale().y)
	{
		m_position.y = 0 - m_npc.getScale().y / 2;
	}
	else if (m_position.y < 0 - m_npc.getScale().y)
	{
		m_position.y = t_window.getSize().y + m_npc.getScale().y / 2;
	}
}
