#pragma once
#include "SFML/Graphics.hpp"

class Tile
{
private:
	sf::Vector2i m_path{};
	sf::RectangleShape m_rect;
	sf::Text m_costText;

public:
	Tile();

	bool m_blank{ true };
	bool m_wall{ false };
	bool m_goal{ false };
	bool m_start{ false };
	bool m_pathTile{ false };
	float m_width;
	int m_cost{};
	int m_integaration{};
	sf::Vector2i m_rowCol{};

	void setText(sf::Font& t_font);
	void render(sf::RenderWindow& t_window, bool& t_showCost);

	sf::RectangleShape getRect() { return m_rect; }

	void setPos(float x, float y);

	void updateText() { m_costText.setString(std::to_string(m_cost)); }
	void updateColor();

	float manhattanDist(const sf::Vector2f& t_posA, const sf::Vector2f& t_posB);

};

