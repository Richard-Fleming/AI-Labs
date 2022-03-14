
#include "Game.h"
#include <iostream>
#include <list>

using namespace std;

Game::Game() :
	m_window{ sf::VideoMode{ 700, 700, 32 }, "Droid Behaviour Tree with Reinforcement Learning" },
	m_exitGame{ false } //when true game will exit
{
	/* initialize random seed: */
	srand(time(NULL));

	setupFontAndText(); // load font 
	setupGrid();		// Prepare the grid world before we create the Droids
	setupDroids();		// Setup the Droids
}

Game::~Game()
{
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.f / 60.f); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event t_event;
	while (m_window.pollEvent(t_event))
	{
		if (sf::Event::Closed == t_event.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == t_event.type) //user pressed a key
		{
			processKeys(t_event);
		}
//		if (sf::Event::MouseButtonPressed == t_event.type || sf::Event::MouseButtonReleased == t_event.type) //user pressed a key
		{
			// I always call processMouse for the case when the middle mouse button is held down to draw obstacles.
			processMouse(t_event);
		}
	}
}

/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Event::Closed == t_event.type) // window message
	{
		m_window.close();
	}
	if (sf::Event::KeyPressed == t_event.type) //user key press
	{
		if (sf::Keyboard::Escape == t_event.key.code)
		{
			m_exitGame = true;
		}
		if (sf::Keyboard::T == t_event.key.code) {
		}
		if (sf::Keyboard::C == t_event.key.code) {
		}
		if (sf::Keyboard::F == t_event.key.code) {
		}
		if (sf::Keyboard::P == t_event.key.code) {
		}
		if (sf::Keyboard::Num1 == t_event.key.code) {
		}
	}
}

/// <summary>
/// deal with mouse button presses from the user
/// </summary>
/// <param name="t_event">mouse press event</param>
void Game::processMouse(sf::Event t_event)
{
	//check for mouse click.
		if (t_event.type == sf::Event::MouseButtonReleased && t_event.mouseButton.button == sf::Mouse::Left)
		{
			//Gets mouse coordinates
			sf::Vector2i mouseCoords = sf::Mouse::getPosition(m_window);
			//			std::cout << "Mouse X: " << mouseCoords.x << " " << "Mouse Y: " << mouseCoords.y <<  std::endl;

		}
		else if (t_event.type == sf::Event::MouseButtonReleased && t_event.mouseButton.button == sf::Mouse::Right)
		{
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
		{
		}
		else if (t_event.type == sf::Event::MouseButtonReleased && t_event.mouseButton.button == sf::Mouse::Middle)
		{
		}
}


/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
//	test();
	if (m_exitGame)
	{
		m_window.close();
	}
	updateDroids();

}
/// <summary>
/// Just a general test method for messing around.
/// </summary>
void Game::test()
{
	std::cout << "Running a test ! " <<  std::endl;

	for (Droid* d : m_droids)
	{
		sf::Vector2i XY = sf::Vector2i(1, 10);
//		int state = d->getStateNoFromXY(XY, gridWorld);

//		XY = d->getXYFromState(state, gridWorld);
	}

}

void Game::updateDroids()
{
	for (Droid* d : m_droids)
	{
		d->update(gridWorld);
	}
}

void Game::render()
{
	m_window.clear(sf::Color::White);
	drawGrid(m_window);
	drawDroids(m_window);
	drawHealthPills(m_window);

	m_window.display();

}

/// <summary>
/// Create a list of Droids
/// </summary>
void Game::setupDroids()
{
	Routine* emptyBrain = new Routine();

	// Example Droid demonstrating Reinforcement Learning
	Droid *d1 = new Droid("D1", 1, 1, 5000, 0, 3, gridWorld);
	Routine* moveTo1 = new MoveTo(1, 1, gridWorld);		// Just set any initial behaviour
														// In this version the real MoveTo behaviour will be determined by the position of the goalNo (Healthpill #)
	d1->setBehaviour(moveTo1);
	d1->setBrain(emptyBrain);
	d1->learning = false;	// Set this to true to activate the Q-learning process
							// The resultant Q-values will to saved out to a file called "qTable_<<name-of-droid>>.csv" e..g "sTable_D1.csv"
	d1->goalNo = 2;			// Potentially we could have multiple goals in different locations
	d1->loadFromFile = true;

	m_droids.push_back(d1);

	Droid* d2 = new Droid("D2", 10, 1, 5000, 0, 3, gridWorld);

	d2->setBehaviour(moveTo1);
	d2->setBrain(emptyBrain);
	d2->learning = false;	// Set this to true to activate the Q-learning process
							// The resultant Q-values will to saved out to a file called "qTable_<<name-of-droid>>.csv" e..g "sTable_D1.csv"
	d2->goalNo = 1;			// Potentially we could have multiple goals in different locations
	d2->loadFromFile = true;

	m_droids.push_back(d2);
	gridWorld.m_gridDroids = m_droids;	//So we can access them when inside the behaviours.

}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFontAndText()
{
}

/// <summary>
/// Run through the grid and setup obstacles
/// </summary>
void Game::initGridObjects()
{
	int hpx = rand() % gridWorld.gridSize + 1;
	int hpy = rand() % gridWorld.gridSize + 1;
	HealthPill* h1 = new HealthPill(20, 20, 50, true, gridWorld);
	m_healthPills.push_back(h1);
	h1 = new HealthPill(10, 18, 50, true, gridWorld);
	m_healthPills.push_back(h1);

	gridWorld.m_gridHealthPills = m_healthPills;	//So we can access them when inside the behaviours.
	for (int i = 0; i < 11; i++)
	{
		gridWorld.nodes[13][i].containsObstacle = true;
		gridWorld.nodes[13][i].setColor(sf::Color::Black);
	}
	for (int i = 7; i < 16; i++)
	{
		gridWorld.nodes[i][10].containsObstacle = true;
		gridWorld.nodes[i][10].setColor(sf::Color::Black);
	}
}


/// <summary>
/// Initialsie the grid at startup.
/// Add in obstacles here.
/// </summary>
void Game::setupGrid()
{
	initGridObjects(); // HealthPills etc

}

/// <summary>
/// Draw the grid cells
/// </summary>
void Game::drawGrid(sf::RenderWindow& t_window)
{
	gridWorld.draw(m_window);
}

void Game::drawDroids(sf::RenderWindow& t_window)
{
	for (Droid* d : m_droids)
	{
		d->draw(t_window, gridWorld);
	}
}

void Game::drawHealthPills(sf::RenderWindow& t_window)
{
	for (HealthPill* hp : m_healthPills)
	{
		hp->draw(t_window, gridWorld);
	}
}
