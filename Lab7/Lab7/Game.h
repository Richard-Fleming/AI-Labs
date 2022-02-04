#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>
#include "Fuzzy.h"

class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

private:

	void processEvents();
	void update(sf::Time t_deltaTime);
	void render();
	
	void setupFontAndText();
	void setupSprite();

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Text m_enemyText; // text used for enemy info
	sf::Text m_playerText; // text used for player info
	bool m_exitGame; // control exiting game

	sf::RectangleShape m_line;

	// Fuzzy RTS
	sf::CircleShape m_player;
	sf::CircleShape m_enemy;

	// Enemy Troop Number
	int m_enemyUnits;

	// Enemy Distance
	int m_range;

	// Enemy Size
	double m_tiny;
	double m_small;
	double m_moderate;
	double m_large;

	// Enemy Range
	double m_close;
	double m_medium;
	double m_far;

	// Threat Levels
	double m_low;
	double m_normal;
	double m_high;

	// Player Troops
	int m_playerUnits;

};

#endif // !GAME

