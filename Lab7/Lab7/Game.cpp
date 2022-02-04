#include "Game.h"
#include <iostream>



Game::Game() :
	m_window{ sf::VideoMode{ 1600, 1200, 32 }, "SFML Game" },
	m_exitGame{false}, //when true game will exit
	m_enemyUnits{0},
	m_range{0},
	m_playerUnits{0}
{
	setupFontAndText(); // load font 
	setupSprite(); // load texture
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
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if ( sf::Event::Closed == event.type) // window message
		{
			m_window.close();
		}
		if (sf::Event::KeyPressed == event.type) //user key press
		{
			if (sf::Keyboard::Escape == event.key.code)
			{
				m_exitGame = true;
			}
			if (sf::Keyboard::Space == event.key.code)
			{
				m_enemyUnits = static_cast<double>(rand() % 30 + 1);
				m_range = static_cast<double>(rand() % 25 + 5);

				m_tiny = Fuzzy::FuzzyTriangle(m_enemyUnits, -10, 0, 10);
				m_small = Fuzzy::FuzzyTrapezoid(m_enemyUnits, 2.5, 10, 15, 20);
				m_moderate = Fuzzy::FuzzyTrapezoid(m_enemyUnits, 15, 20, 25, 30);
				m_large = Fuzzy::FuzzyGrade(m_enemyUnits, 25, 30);

				m_close = Fuzzy::FuzzyTriangle(m_range, -30, 0, 30);
				m_medium = Fuzzy::FuzzyTrapezoid(m_range, 10, 30, 50, 70);
				m_far = Fuzzy::FuzzyGrade(25, 50, 70);

				m_low = Fuzzy::FuzzyOR(Fuzzy::FuzzyOR(Fuzzy::FuzzyAND(m_medium, m_tiny), Fuzzy::FuzzyAND(m_medium, m_small)), Fuzzy::FuzzyAND(m_far, Fuzzy::FuzzyNOT(m_large)));
				m_normal = Fuzzy::FuzzyOR(Fuzzy::FuzzyOR(Fuzzy::FuzzyAND(m_close, m_tiny), Fuzzy::FuzzyAND(m_medium, m_moderate)), Fuzzy::FuzzyAND(m_far, m_large));
				m_high = Fuzzy::FuzzyOR(Fuzzy::FuzzyAND(m_close, Fuzzy::FuzzyNOT(m_tiny)), Fuzzy::FuzzyAND(m_medium, m_large));

				m_enemyText.setString("There are " + std::to_string(m_enemyUnits) + " Enemy Units at a Range of " + std::to_string(m_range) + " Units");

				m_playerUnits = std::ceil((m_low * 10 + m_normal * 30 + m_high * 50) / (m_low + m_normal + m_high));
				m_playerText.setString("The number of Units that should be deployed in response is " + std::to_string(m_playerUnits) + " Units");
			}
		}
	}
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}
}

/// <summary>
/// draw the frame and then switch bufers
/// </summary>
void Game::render()
{
	m_window.clear();
	m_window.draw(m_line);
	m_window.draw(m_enemyText);
	m_window.draw(m_playerText);

	for (int i = 0; i < m_playerUnits; i++)
	{
		m_player.setPosition(100 + (i * 25), 900);
		m_window.draw(m_player);
	}
	for (int i = 0; i < m_enemyUnits; i++)
	{
		m_enemy.setPosition(100 + (i * 25), 300);
		m_window.draw(m_enemy);
	}
	m_window.display();
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFontAndText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	m_enemyText.setFont(m_ArialBlackfont);
	m_enemyText.setPosition(80.0f, 550.0f);
	m_enemyText.setCharacterSize(30);
	m_enemyText.setOutlineColor(sf::Color::Red);
	m_enemyText.setFillColor(sf::Color::Black);
	m_enemyText.setOutlineThickness(3.0f);

	m_playerText.setFont(m_ArialBlackfont);
	m_playerText.setPosition(80.0f, 650.0f);
	m_playerText.setCharacterSize(30);
	m_playerText.setOutlineColor(sf::Color::Green);
	m_playerText.setFillColor(sf::Color::Black);
	m_playerText.setOutlineThickness(3.0f);

}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprite()
{
	m_player.setFillColor(sf::Color::Green);
	m_player.setRadius(10);

	m_enemy.setFillColor(sf::Color::Red);
	m_enemy.setRadius(10);

	m_line.setFillColor(sf::Color::White);
	m_line.setSize(sf::Vector2f(1600, 40));
	m_line.setPosition(0, 600);
}
