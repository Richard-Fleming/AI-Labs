/// 
/// @author Dr. Oisin Cawley
/// @date February 2019

#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 


#include "Game.h"
#include <iostream>
#include <vector>
#include <numeric>
#include <fstream>
#include <sstream>

using namespace std;

// Updates as milliseconds
static double const S_PER_UPDATE = 5;

/// <summary>
/// Default constructor
/// 
/// 
/// </summary>
Game::Game() :
	m_window(sf::VideoMode(Game::width, Game::height), "AI for Flappy Bird - ANN with GA"),
	m_exitGame{ false } //when true game will exit

{
	m_window.setFramerateLimit(10); //Lets start slow so we can see whats going on.

	if (!font.loadFromFile("Assets\\ariblk.ttf"))
	{
		std::cout << "could not load font" << std::endl;
	}

	text.setFont(font);
	text.setString("Evolutionary Details");
	// in pixels, not points!
	text.setCharacterSize(10);
	text.setPosition(sf::Vector2f(10, 10));
	text.setFillColor(sf::Color::White);
	text.setOutlineThickness(10);

}

/// <summary>
/// Default destructor
/// 
/// 
/// </summary>
Game::~Game()
{
}

void Game::init()
{
	srand(time(NULL));

	for (int i = 0; i < pillarCount; i++)
	{
		//		pillarSet[i] = Pillar();
		pillarSet[i].init(width / 2.5 + (i * pillarGap));
	}
	population.createPopulation(birdCount);
	if (ai)
	{
		if (birdCount == 1)
		{
			loadGABrain(0);	// Load network weights from file
		}
		else if (birdCount > 1)
		{
			for (int b = 0; b < birdCount; b++)
			{
				population.birdSet[b].y = rand() % Game::height;	// Since we have more than 1 AI bird we will reset it to a random veetical postion
				int i = rand() % 2 + 1;
				if (i == 1)
					loadGABrain(b);	// Load network weights from GA file
				else
					loadTFBrain(b);	// Load network weights from Tensorflow file
			}
		}
	}
	loop();
}

/// <summary>
/// Load the neural netork from a file.
/// The file is the one saved when trained via the GA.
/// Just loading those weights into the Bird's neural network.
/// </summary>
void Game::loadGABrain(int bird)
{
	vector<string> row;
	string line, word, temp;
	int layer = 0;
	int index = 0;
	float weight = 0.0f;

	population.birdSet[bird].brain.source = 0;

	myWeightsFile.open("flappy_ga_weights.csv");
	while (!myWeightsFile.eof())
	{
		// There are 5 lines in the file
		// First 4 are the 5 weights for the inputs to the hidden layer and the bias
		// Last row ar the 4 weights for the inputs to the output layer and the bias
		for (int i = 0; i < 4; i++)
		{
			row.clear();
			// read an entire row and 
			// store it in a string variable 'line' 
			getline(myWeightsFile, line);
			std::cout << line << std::endl;

			// used for breaking words 
			stringstream s(line);
			// read every column data of a row and 
			// store it in a string variable, 'word'
			index = 0;
			while (getline(s, word, ','))
			{
				weight = stof(word);
				// Make sure to load the weights into the correct place in the neural network.
				// The 6th value is the Bias
				if (index == 5)
					population.birdSet[bird].brain.biases[i] = weight;
				else
					population.birdSet[bird].brain.weightsLayer1[index][i] = weight;

				std::cout << "Inputs to Hidden: " << weight << std::endl;
				index = index + 1;
			}
		}
		// Last row
		getline(myWeightsFile, line);
		std::cout << line << std::endl;

		stringstream s(line);
		index = 0;
		while (getline(s, word, ','))
		{
			weight = stof(word);
			// The 5th value is the Bias
			if (index == 4)
				population.birdSet[bird].brain.biases[4] = weight;
			else
				population.birdSet[bird].brain.weightsLayer2[index] = weight;
			std::cout << "Hidden to Outputs: " << weight << std::endl;
			index++;
		}
	}
	myWeightsFile.close();
}

/// <summary>
/// Train the neural netork from training data in a file.
/// Doing the training in Python with Tensorflow.
/// Just loading those weights into the Brid's neural network.
/// </summary>
void Game::loadTFBrain(int bird)
{
	vector<string> row;
	string line, word, temp;
	int layer = 0;
	int index = 0;
	float weight = 0.0f;

	population.birdSet[bird].brain.source = 1;

//	myWeightsFile.open("flappy_tf_weights-test.csv");
//	myWeightsFile.open("flappy_tf_weights-21k-Acc76point84.csv");
	myWeightsFile.open("flappy_tf_weights-70k-Acc78point44.csv");

	while (!myWeightsFile.eof())
	{
		// First 5 lines are the weights for the inputs to the hidden layer;
		for (int i = 0; i < 5; i++)
		{
			row.clear();
			// read an entire row and 
			// store it in a string variable 'line' 
			getline(myWeightsFile, line);
			std::cout << line << std::endl;

			// used for breaking words 
			stringstream s(line);
			// read every column data of a row and 
			// store it in a string variable, 'word'
			index = 0;
			while (getline(s, word, ','))
			{
				weight = stof(word);
				population.birdSet[bird].brain.weightsLayer1[i][index] = weight;
				std::cout << "Inputs to Hidden: " << weight << std::endl;
				index = index + 1;
			}
		}
		// Next 4 lines are the bias weights to the hidden layer;
		for (int i = 0; i < 4; i++)
		{
			getline(myWeightsFile, line);
			stringstream s(line);
			while (getline(s, word, ','))
			{
				weight = stof(word);
				population.birdSet[bird].brain.biases[i] = weight;
				std::cout << "Bias for neuron " << i + 1 << ": " << weight << std::endl;
			}
		}
		// Next 4 lines are the input weights to the output layer;
		for (int i = 0; i < 4; i++)
		{
			getline(myWeightsFile, line);
			stringstream s(line);
			while (getline(s, word, ','))
			{
				weight = stof(word);
				population.birdSet[bird].brain.weightsLayer2[i] = weight;
				std::cout << "Hidden to Outputs: " << weight << std::endl;
			}
		}
		// Last line is the bias weight to the output neuron
		for (int i = 0; i < 1; i++)
		{
			getline(myWeightsFile, line);
			stringstream s(line);
			while (getline(s, word, ','))
			{
				weight = stof(word);
				population.birdSet[bird].brain.biases[4] = weight;
				std::cout << "Bias for output neuron: " << weight << std::endl;
			}
		}
	}
	myWeightsFile.close();
}


/// <summary>
/// The game loop, listens for key press events and handles window closing,
/// calls the update and draw methods
/// </summary>
void Game::loop()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.f / 40.f); // 60 fps
	if (player)
		timePerFrame = sf::seconds(1.f / 30.f); // 60 fps

	sf::Int32 lag = 0;
	int num = 0;

	while (m_window.isOpen())
	{
		sf::Event event;
		while (m_window.pollEvent(event))
		{
			switch (event.type)
			{
			// window closed
			case sf::Event::Closed:		//Window message
				if (myTrainingfile.is_open())
					myTrainingfile.close();
				m_window.close();
				break;
			// key pressed
			case sf::Event::KeyPressed:
				if (sf::Keyboard::Escape == event.key.code)
				{
					m_exitGame = true;
				}
				// Moved this into the update function since it doesn't get called quickly enough inside this pollEvent.
//				if ((sf::Keyboard::Space == event.key.code) && (player))
//				{
//					population.birdSet[0].jump = true;
//				}
				if (sf::Keyboard::S == event.key.code)
				{
					showBirdStats = !showBirdStats;
				}
				if (sf::Keyboard::R == event.key.code)
				{
					render = !render;
				}
				if (sf::Keyboard::E == event.key.code)
				{
					population.elitism = !population.elitism;
				}
				if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num9)
				{
					currentSpeed = event.key.code - sf::Keyboard::Num0;
					int frameRate = (event.key.code - sf::Keyboard::Num0);
					timePerFrame = sf::seconds(1.f / (40.f * frameRate));
//					m_window.setFramerateLimit(frameRate);
				}
				// M for save the weights of any bird brain still alive.
				// Should really only be one at this stage.
				if (sf::Keyboard::M == event.key.code)
				{
					std::ostringstream os;
					string w = "";
					string weights = "";

					std::cout << "======================" << std::endl;
					myTrainingfile.open("flappy_ga_weights.csv");
					for (int b = 0; b < birdCount; b++)
					{
						if (population.birdSet[b].alive)
						{
							for (int i = 0; i < population.birdSet[b].brain.numHidden; i++)
							{
								for (int j = 0; j < population.birdSet[b].brain.numInputs; j++)
								{
									w = to_string(population.birdSet[b].brain.weightsLayer1[j][i]);
									weights = weights + w + ",";
								}
								// Add the Bias
								w = to_string(population.birdSet[b].brain.biases[i]);
								weights = weights + w + ",";
								std::cout << "Layer 1 Weights for Bird " << b << " neuron " << i << " " << weights << std::endl;
								myTrainingfile << weights << endl;
								weights = "";
							}
							weights = "";
							for (int i = 0; i < population.birdSet[b].brain.numHidden; i++)
							{
								w = to_string(population.birdSet[b].brain.weightsLayer2[i]);
								weights = weights + w + ",";
							}
							// Add the Bias
							w = to_string(population.birdSet[b].brain.biases[4]);
							weights = weights + w + ",";
							std::cout << "Layer 2 Weights for Bird " << b << " " << weights << std::endl;
							myTrainingfile << weights << endl;
							weights = "";
						}
					}
				}
				// Start capturing the environment inputs and bird brain decisions for training data.
				if (sf::Keyboard::C == event.key.code)
				{
					string w = "";
					if (!capture)
					{
						capture = true;
						std::cout << "======================" << std::endl;
						myTrainingfile.open("training_data.csv");
					}
					else
					{
						capture = false;	// Turn it off
						if (myTrainingfile.is_open())
							myTrainingfile.close();
					}
				}
			default:
				break;
			}
		}
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			update(S_PER_UPDATE);
		}
		if (render) draw();
	}
}


/// <summary>
/// Update the game also update the collision with the screen and wrap entities around
/// TODO: make sure the collision off the screen is handled INSIDE the object class, create
/// a static window variable public to all objects
/// 
/// 
/// </summary>
void Game::update(float dt)
{
	if (m_exitGame)
	{
		m_window.close();
	}

	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) && (player))
	{
		population.birdSet[0].jump = true;
	}

	for (int i = 0; i < pillarCount; i++)
	{
		pillarSet[i].update(dt, pillarSet);

	}
	int birdsAlive = 0;
	birdsAlive = population.update(pillarSet[index]);
	if (birdsAlive == 0)
	{
		if (player || ai)
			m_exitGame = true;
		else if (learn)		// If the GA is running we want to start again.
		{
			capture = false;
			resetPillars();
			population.evolve();
			index = 0; // The nearest pillar to the birds
		}
	}
	else if (capture)	// Capture training data.
	{
		string tempString = "";

		for (int b = 0; b < birdCount; b++)
		{
			if (population.birdSet[b].alive)
			{
				tempString = to_string(pillarSet[index].x) + "," + to_string(pillarSet[index].upperH) + "," + to_string(pillarSet[index].lowerY);
				tempString = tempString + "," + to_string(population.birdSet[b].x) + "," + to_string(population.birdSet[b].y);

				std::cout << tempString << std::endl;

				myTrainingfile << tempString;
				if (population.birdSet[b].jump)
					myTrainingfile << ",flap\n";
				else
					myTrainingfile << ",glide\n";
			}
		}
	}
	if (ai)		// Lets see what decisions the network is making
	{
		string tempString = "";
		tempString = to_string(pillarSet[index].x) + "," + to_string(pillarSet[index].upperH) + "," + to_string(pillarSet[index].lowerY);
		tempString = tempString + "," + to_string(population.birdSet[0].x) + "," + to_string(population.birdSet[0].y);
		tempString = tempString + (population.birdSet[0].jump ? ",flap" : ",glide");
		std::cout << tempString << std::endl;

	}
	// So for the case where player has pressed space and we have captured it for training we must reset the flag.
	if (player)
		population.birdSet[0].jump = false;

}

/// <summary>
/// Draw the game entities
/// 
/// 
/// </summary>
void Game::draw()
{
	m_window.clear();
	for (int i = 0; i < pillarCount; i++)
	{
		pillarSet[i].draw(m_window);
		// Need to keep track of which is the nearest pillar in fornt of the bird so we can draw lines to it.
		// Throw in a bit of colouring. Also helps with the debug.
		if (pillarSet[i].x < population.birdSet[0].x)
		{
			pillarSet[i].colour = sf::Color::Red;
			index = (i + 1) % pillarCount;
		}
		else
		{
			pillarSet[i].colour = sf::Color::Cyan;
		}
	}
	// The green one is the pillar coming up.
	pillarSet[index].colour = sf::Color::Green;
	//	std::cout << "Next pillar: " << index << std::endl;

	text.setFillColor(sf::Color::White);
	string AIorPlayer = ai ? "AI" : player ? "PLAYER" : "LEARNING";
	text.setString("Mode: " + AIorPlayer);
	text.setPosition(sf::Vector2f(10, 10));
	m_window.draw(text);

	text.setFillColor(sf::Color::White);
	text.setString("S to toggle Bird stats");
	text.setPosition(sf::Vector2f(10, 30));
	m_window.draw(text);

	text.setString("1 - 9 sets game speed (" + std::to_string(currentSpeed) + ")");
	text.setPosition(sf::Vector2f(10, 50));
	m_window.draw(text);

	string TorF;
	if (learn)
	{
		TorF = population.elitism ? "ON" : "OFF";
		text.setString("E to toggle Elitism: " + TorF);
		text.setPosition(sf::Vector2f(10, 70));
		m_window.draw(text);

		text.setString("Gereration: " + std::to_string(population.generation));
		text.setPosition(sf::Vector2f(10, 110));
		m_window.draw(text);
	}

	TorF = population.birdSet[0].brain.useBiases ? "ON" : "OFF";
	text.setString("Bias included: " + TorF);
	text.setPosition(sf::Vector2f(10, 90));
	m_window.draw(text);


	if (bestSoFar < population.birdSet[population.getFittest()].fitness)
		bestSoFar = population.birdSet[population.getFittest()].fitness;
	text.setString("Highest Fitness: " + std::to_string(bestSoFar));
	text.setPosition(sf::Vector2f(10, 130));
	m_window.draw(text);

	string source = "";

	for (int i = 0; i < birdCount; i++)
	{
		if (population.birdSet[i].brain.source == 0)
			source = " [GA]";
		else
			source = " [BP]";

		if (population.birdSet[i].alive)
		{
			population.birdSet[i].draw(m_window, pillarSet[index]);
			text.setString("Bird " + std::to_string(i + 1) + " Fitness: " + std::to_string(population.birdSet[i].score) + source);
		}
		else
		{
			text.setString("Bird " + std::to_string(i + 1) + " -DEAD-" + source);
		}
		text.setFillColor(population.birdSet[i].colour);
		text.setPosition(sf::Vector2f(10, 160 + i * 30));
		// Drawing text to the screen seems to really slow things down, so this will allow us to have maximmum learning speed.
		if (showBirdStats)
			m_window.draw(text);

	}
	m_window.display();
}

void Game::resetPillars()
{
	for (int i = 0; i < Game::pillarCount; i++)
	{
		//		pillarSet[i] = Pillar();
		pillarSet[i].init(width / .95 + (i * pillarGap));
	}

}

