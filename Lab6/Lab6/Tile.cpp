#include "Tile.h"

Tile::Tile()
{
	m_width = 20.0f;
	m_cost = 1000;
	m_integaration = 1000;
	m_rect.setSize(sf::Vector2f{ m_width, m_width });
	m_rect.setPosition(sf::Vector2f{});
	m_rect.setOutlineThickness(1.0f);
	m_rect.setOutlineColor(sf::Color::Black);
}

void Tile::setText(sf::Font& t_font)
{
	m_costText.setFont(t_font);
	m_costText.setCharacterSize(10u);
	m_costText.setFillColor(sf::Color::White);
	m_costText.setString(std::to_string(m_cost));
}

void Tile::render(sf::RenderWindow& t_window, bool& t_showCost)
{
	t_window.draw(m_rect);
	if (t_showCost)
	{
		t_window.draw(m_costText);
	}
}

void Tile::setPos(float x, float y)
{
	m_rect.setPosition(x, y);
	m_costText.setPosition(x, y);
}

void Tile::updateColor()
{
	if (m_blank)
	{
		m_rect.setFillColor(sf::Color::Blue);
	}
	else if (m_goal)
	{
		m_rect.setFillColor(sf::Color::Green);
	}
	else if (m_start)
	{
		m_rect.setFillColor(sf::Color::Red);
	}
	else if (m_wall)
	{
		m_rect.setFillColor(sf::Color::Black);
	}
	else if (m_pathTile)
	{
		m_rect.setFillColor(sf::Color::Yellow);
	}
}

float Tile::manhattanDist(const sf::Vector2f& t_posA, const sf::Vector2f& t_posB)
{
	sf::Vector2i dist{};
	dist.x = t_posB.x - t_posA.x;
	dist.y = t_posB.y - t_posA.y;

	if (dist.x < 0)
	{
		dist.x = -dist.x;
	}
	if (dist.y < 0)
	{
		dist.y = -dist.y;
	}

	return dist.x + dist.y;
}
