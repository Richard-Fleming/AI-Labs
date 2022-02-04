// author Peter Lowe
#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>
#include <queue>
#include "Tile.h"

class Game
{
public:
	Game();
	~Game();
	void run();

private:

	void processEvents();
	void update(sf::Time t_deltaTime);
	void render();

	void generateFlowField();
	void generateCost(int row, int col);
	void generatePath();

	sf::RenderWindow m_window;
	bool m_exitGame;

	bool m_showCosts{ false };

	std::array<std::array<Tile*, 50>, 50> m_tiles{ nullptr };
	std::queue<Tile*> m_list{};

	sf::Font m_font;

	Tile* m_start{ nullptr };
	Tile* m_goal{ nullptr };
};

#endif // !GAME

