#include "Agent.h"

// constructor with no Agent parameters
Agent::Agent()
{}

// constructor with default agent parameters
Agent::Agent(string name, int numStates, int numActions)
{
    this->name = name;
	this->numStates = numStates;
	this->numActions = numActions;
	initialiseQvalues();
}

// constructor with specified agent parameters
Agent::Agent(string name, int numStates, int numActions, float alpha, float gamma, float epsilon)
{
    this->name = name;
    this->numStates = numStates;
	this->numActions = numActions;
	this->alpha = alpha;
	this->gamma = gamma;
	this->epsilon = epsilon;
	initialiseQvalues();
}

Agent::~Agent()
{}

void Agent::initialiseQvalues()
{
    std::cout << "Initialising Q values for Agent: " << name << std::endl;
    qTable = new float* [numStates];
    for (int i = 0; i < numStates; i++)
        qTable[i] = new float[numActions];
    for (int i = 0; i < numStates; i++)
    {
        for (int j = 0; j < numActions; j++)
        {
            qTable[i][j] = 0.0f;
        }
    }
}

void Agent::updateQValue(int previousState, int selectedAction, int currentState, float reward)
{
    // implementation of Q-learning Temporal Difference update rule
    float oldQ = qTable[previousState-1][selectedAction];
    float maxQ = getMaxQValue(currentState);
    float newQ = oldQ + alpha * (reward + gamma * maxQ - oldQ);
    qTable[previousState-1][selectedAction] = newQ;
}

int Agent::selectAction(int state) 
{
    // epsilon-greedy action selection strategy implementation
    int selectedAction = -1;
    double randomValue = (double)rand() / RAND_MAX;

    if (debug) {
        std::cout << "Agent: " << name << " selecting action, epsilon = " << epsilon << ", randomValue = " << randomValue << std::endl;
    }

    if (randomValue < epsilon) { // select a random action with probability epsilon
        selectedAction = selectRandomAction();
        if (debug) {
            std::cout << "Agent: " << name << " selected action " << selectedAction << " at random" << std::endl;
        }
    }
    else { // else select the most valuable action
        selectedAction = getMaxValuedAction(state);
        if (debug) {
            std::cout << "Agent: " << name << " selected action " << selectedAction << " greedily" << std::endl;
        }
    }
    return selectedAction;
}

int Agent::selectRandomAction()
{
    // select a random action between 1 and (numActions)
    return (rand() % (numActions-1)) + 1;
}

int Agent::getMaxValuedAction(int state)
{
    // greedy action selection implementation
    // return the index of the most valuable action for a particular state
    int maxIndex = -1;
    float maxValue = -FLT_MAX;
    for (int action = 0; action < numActions; action++) {
        if (qTable[state-1][action] > maxValue) {
            maxIndex = action;
            maxValue = qTable[state-1][action];
        }
    }
    return maxIndex;
}

float Agent::getMaxQValue(int state)
{
    // return the Q value of the most valuable action for a particular state
    int maxIndex = getMaxValuedAction(state);
    return qTable[state-1][maxIndex];
}

void Agent::enableDebugging() 
{
    this->debug = true;
}

void Agent::disableDebugging() 
{
    this->debug = false;
}

// returns a copy of the agent's Q values table
vector<vector<float>> Agent::copyQTable()
{
    vector<vector<float>> copy(numStates);
    for (int s = 0; s < numStates; s++) 
    {
        copy[s] = vector<float>(numActions);
        for (int a = 0; a < numActions; a++) 
        {
            float f = qTable[s][a];
            copy[s][a] = f;
        }
    }
    return copy;
}

