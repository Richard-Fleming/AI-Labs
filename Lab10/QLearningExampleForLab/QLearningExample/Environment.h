#ifndef RLENV_H
#define RLENV_H

#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "Agent.h"
#include <string>

using namespace std;

class Environment
{

public:
	// agent parameters, default values
	float alpha = 0.1f;     // learning rate
	float gamma = 0.9f;     // discount factor
	float epsilon = 0.1f;   // exploration rate
//	float** qTable;
	vector<vector<float>> copy; // Will hold a copy of the final qTable for the Droid to refer to
	int numStates = 0;
	int numActions = 4;
	char actionLabels[4][10] = { "North","East","South","West" };
	int xDimension = 1;
	int yDimension = 1;

	bool debug = false;

	int numEpisodes = 100000;
	int maxTimesteps = 5000;
	int goalNo = 1;	// We may have multiple goals.
	bool goalReached = false;
	sf::Vector2i goalLocationXY = sf::Vector2i(1, 1);
	sf::Vector2i agentStartXY = sf::Vector2i(1, 1);
	float goalReward = 10.0f;
	float stepPenalty = -1.0f;

	// Agent parameters
	Agent agent;
	sf::Vector2i currentAgentCoords = sf::Vector2i(-1, -1); // agent's current x,y position (state vector)
	sf::Vector2i previousAgentCoords = sf::Vector2i(-1, -1); // agent's previous x,y position (state vector)
	bool alphaDecays = false;
	float alphaDecayRate = 0.9f;
	bool epsilonDecays = false;
	float epsilonDecayRate = 0.9f;

	// data logging
	vector<int> movesToGoal;

	Environment();
	Environment(Grid& grid);
	~Environment();
	void setupAgent(string name);
	void setGoalLocationXY(int x, int y);
	void setAgentStartXY(int x, int y);
	void doExperiment(string name, Grid& grid);
	void doEpisode(string name, Grid& grid);
	void doTimestep(Grid& grid);
	sf::Vector2i getXYFromStateNo(int stateNo, Grid& grid);
	int getStateNoFromXY(sf::Vector2i state, Grid& grid);
	sf::Vector2i getNextStateXY(sf::Vector2i currentStateXY, int action, Grid& grid);
	float calculateReward(sf::Vector2i previousAgentCoords, int selectedAction, sf::Vector2i currentAgentCoords);
	void decayAlpha();
	void decayEpsilon();
	void enableDebugging();
	void disableDebugging();
	int getNumStates(Grid& grid);
	string qTableAsString(float** qTable, Grid& grid);



};

#endif