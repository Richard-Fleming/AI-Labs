#pragma once

#include <SFML/Graphics.hpp>
#include <time.h>
#include "Pillar.h"
#include "Bird.h"
#include "Population.h"
#include <fstream>

///
/// Game class is the main loop of the game that updates and draws the game
///
class Game
{

public:
	static const int width = 1500;
	static const int height = 800;
	static const int pillarGap = 600;	// The gap between pillars
	static const int pillarCount = 5;

	static const bool learn = false;		// True if learning to play via the GA. 
										// Don't forget to set ai=false, player=false and birdCount to a high number below.
										// Also remember that Elitism defaults to false, so pressing 'E' will enable it.
	static const bool player = false;	// True if human wants to play. Don't forget to set birdCount=1 below.
	static const bool ai = true;		// True if we are loading a neural network and the AI plays.
	static const int birdCount = 20;		// Our population size for learning via the GA is something like 20. Experiment with that number.
										// Set to 1 if we are in player mode or just loading the GA Brain.
										// If AI is true and this is > 1 then each AI bird gets randomly assigned a GA or Tensorflow trained brain.
	bool capture = false;
	bool render = true;
	std::ofstream myTrainingfile;
	std::ifstream myWeightsFile;

	sf::Text text;
	int currentSpeed = 0;
	int bestSoFar = 0;

	//Default constructor
	Game();
	//Default destructor
	~Game();
	//function ran when the game starts
	void init();
	//Load the trained neural network from file
	void loadGABrain(int bird);
	//Load the trained neural network from file
	void loadTFBrain(int bird);
	//train the neural network
	void trainBrain();
	//the game loop method
	void loop();
	//update game logic
	void update(float dt);
	//draw the game elements
	void draw();
	void resetPillars();
	sf::RenderWindow m_window;



private:
	sf::Vector2f m_mousePos;
	sf::Font font;

	sf::VertexArray lines;

	bool m_exitGame; // control exiting game
	bool showBirdStats = true;


	Pillar pillarSet[pillarCount];
	int index = 0; //The pillar the bird is approaching.

	Population population; //For the Genetic Algorithm.


};

