// authors Richard Fleming and Enna Conlon

#include "Game.h"
#include <iostream>



Game::Game() :
	m_window{ sf::VideoMode{ sf::VideoMode::getDesktopMode().width / 2, sf::VideoMode::getDesktopMode().height / 2, 32 }, "AI Lab 1" },
	m_exitGame{ false } //when true game will exit
{
	srand(time(nullptr));
	m_npc.m_velocity.x = rand() % 20 - 10;
	m_npc.m_velocity.y = rand() % 20 - 10;

	m_player.m_velocity.x = rand() % 20 - 10;
	m_player.m_velocity.y = rand() % 20 - 10;

	m_player.m_position.x = m_player.m_player.getScale().x;
	m_player.m_position.y = m_window.getSize().y / 2;

	m_npc.m_position.x = m_window.getSize().x - m_npc.m_npc.getScale().x;
	m_npc.m_position.y = m_window.getSize().y / 2;
}


Game::~Game()
{
}

void Game::startingUpdate(sf::Time t_deltaTime)
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
			if (sf::Keyboard::Up == event.key.code)
			{
				m_player.increase();
			}
			if (sf::Keyboard::Down == event.key.code)
			{
				m_player.decrease();
			}
		}
		// check if the event is a mouse button release
		if (sf::Event::MouseButtonReleased == event.type)
		{
			
		}
	}
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	m_player.update(m_window);
	m_npc.update(m_window);
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
	m_window.clear(sf::Color::Black);
	m_window.draw(m_player.m_player);
	m_window.draw(m_npc.m_npc);
	m_window.display();
}

