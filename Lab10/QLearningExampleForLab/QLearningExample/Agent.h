#ifndef AGENT_H
#define AGENT_H

#include <SFML/Graphics.hpp>
#include "Grid.h"
#include <iostream>

using namespace std;

class Agent
{

public:
	string name;
	// Q table parameters	
	int numStates;
	int numActions;
	float** qTable;

	// agent parameters, default values
	float alpha = 0.1f;     // learning rate
	float gamma = 0.9f;     // discount factor
	float epsilon = 0.1f;   // exploration rate
	bool debug = false;

	// constructor with no Agent parameters
	Agent();
	// constructor with default agent parameters
	Agent(string name, int numStates, int numActions);
	// constructor with specified agent parameters
	Agent(string name, int numStates, int numActions, float alpha, float gamma, float epsilon);
	~Agent();

	void initialiseQvalues();
	void updateQValue(int previousState, int selectedAction, int currentState, float reward);
	int selectAction(int state);
	int selectRandomAction();
	int getMaxValuedAction(int state);
	float getMaxQValue(int state);
	void enableDebugging();
	void disableDebugging();
	vector<vector<float>> copyQTable();




};

#endif