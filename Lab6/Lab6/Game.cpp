#include "Game.h"
#include <iostream>



Game::Game() :
	m_window{ sf::VideoMode{ 800, 600, 32 }, "Flow Field" },
	m_exitGame{false} //when true game will exit
{
	m_font.loadFromFile("ASSETS/monofonto.otf");

	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			m_tiles.at(i).at(j) = new Tile();
			m_tiles.at(i).at(j)->setText(m_font);
			m_tiles.at(i).at(j)->m_rowCol = sf::Vector2i{ i,j };
			m_tiles.at(i).at(j)->setPos(m_tiles.at(i).at(j)->m_width * i, m_tiles.at(i).at(j)->m_width * j);
			m_tiles.at(i).at(j)->updateColor();
		}
	}

	// set initial goal and start
	m_tiles.at(0).at(0)->m_start = true;
	m_tiles.at(0).at(0)->m_blank = false;
	m_start = m_tiles.at(0).at(0);
	m_tiles.at(0).at(0)->updateColor();

	m_tiles.at(49).at(49)->m_goal = true;
	m_tiles.at(49).at(49)->m_blank = false;
	m_goal = m_tiles.at(49).at(49);
	m_tiles.at(49).at(49)->updateColor();

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
		}

		// Costs Toggle
		if (sf::Event::KeyReleased == event.type)
		{
			if (sf::Keyboard::C == event.key.code)
			{
				if (m_showCosts)
				{
					m_showCosts = false;
				}
				else
				{
					m_showCosts = true;;
				}
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
	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			if (m_tiles.at(i).at(j)->getRect().getGlobalBounds().contains(m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window))))
			{
				// Wall Placement
				if (m_tiles.at(i).at(j)->m_blank && sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
				{
					m_tiles.at(i).at(j)->m_blank = false;
					m_tiles.at(i).at(j)->m_wall = true;
				}
				else if (m_tiles.at(i).at(j)->m_wall && sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
				{
					m_tiles.at(i).at(j)->m_blank = true;
					m_tiles.at(i).at(j)->m_wall = false;
				}
			}
		}
	}

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
	m_window.clear(sf::Color::White);

	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			m_tiles.at(i).at(j)->render(m_window, m_showCosts);
		}
	}

	m_window.display();
}

void Game::generateFlowField()
{
}
