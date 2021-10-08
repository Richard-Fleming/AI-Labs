// authors Richard Fleming and Enna Conlon
#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "NPC.h"

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


	void startingUpdate(sf::Time t_deltaTime);


	sf::RenderWindow m_window; // main SFML window
	bool m_exitGame; // control exiting game
	Player m_player;
	NPC m_npc;

};

#endif // !GAME

