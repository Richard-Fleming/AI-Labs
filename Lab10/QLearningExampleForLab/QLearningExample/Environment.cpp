#include "Environment.h"
#include "HealthPill.h"

Environment::Environment()
{
}

Environment::Environment(Grid& grid)
{
	this->xDimension = grid.gridSize;
	this->yDimension = grid.gridSize;
	numStates = getNumStates(grid);
}

Environment::~Environment()
{}

void Environment::setupAgent(string name) 
{
	agent = Agent(name, numStates, numActions, alpha, gamma, epsilon);
	if (debug) {
		agent.enableDebugging();
	}
}

void Environment::setGoalLocationXY(int x, int y)
{
	goalLocationXY = sf::Vector2i(x, y);
}

void Environment::setAgentStartXY(int x, int y)
{
	agentStartXY = sf::Vector2i(x, y);
}

void Environment::doExperiment(string name, Grid& grid)
{
	setupAgent(name);
	int count = 0;

	for (HealthPill* hp : grid.m_gridHealthPills)
	{
		count++;
		std::cout << "Learning for goal " << count << std::endl;
		goalNo = count;

		setGoalLocationXY(hp->x, hp->y);

		for (int e = 0; e < numEpisodes; e++)
		{
			if (debug) {
				std::cout << "Environment: *************** Episode " << e << " starting ***************" << std::endl;
			}
			doEpisode(name, grid);
			//			std::cout << "Last step count: " << movesToGoal.back() << std::endl;
		}
	}

//	for (int e = 0; e < numEpisodes; e++) 
//	{
//		if (debug) {
//			std::cout << "Environment: *************** Episode " << e << " starting ***************" << std::endl;
//		}
//		doEpisode(name, grid);
//	}

	copy = agent.copyQTable();
	std::cout << qTableAsString(agent.qTable, grid) << std::endl;

}

void Environment::doEpisode(string name, Grid& grid)
{
	int stepsTaken = 0;	// performance metric
	currentAgentCoords.x = agentStartXY.x; // reset agent position
	currentAgentCoords.y = agentStartXY.y;
	goalReached = false;

	for (int t = 0; t < maxTimesteps; t++) 
	{
		if (!goalReached) {
			if (debug) {
				std::cout << "Environment: *************** Timestep " << t << " starting ***************" << std::endl;
			}
			doTimestep(grid);
			stepsTaken++;
		}
		else {
			break;
		}
	}

	// wrap up episode
	decayAlpha();
	decayEpsilon();
	movesToGoal.push_back(stepsTaken);
}

void Environment::doTimestep(Grid& grid)
{
	int currentStateNo = Environment::getStateNoFromXY(currentAgentCoords, grid);
	int selectedAction = agent.selectAction(currentStateNo);
	previousAgentCoords = currentAgentCoords;
	currentAgentCoords = getNextStateXY(previousAgentCoords, selectedAction, grid);

	float reward = calculateReward(previousAgentCoords, selectedAction, currentAgentCoords);
	int nextStateNo = getStateNoFromXY(currentAgentCoords, grid);
	agent.updateQValue(currentStateNo, selectedAction, nextStateNo, reward);
	if (debug) {
		std::cout << "Environment: previousState [" << previousAgentCoords.x << "," << previousAgentCoords.y << "," << goalNo << "]; selected move " << actionLabels[selectedAction] << "; currentState [" << currentAgentCoords.x << "," << currentAgentCoords.y << "," << goalNo << "];" << std::endl;
	}
}

// Implementation of the Environment Reward Function
float Environment::calculateReward(sf::Vector2i previousAgentCoords, int selectedAction, sf::Vector2i currentAgentCoords) 
{
	float reward = 0.0f;
	// check if the goal state has been reached
	if (currentAgentCoords.x == goalLocationXY.x && currentAgentCoords.y == goalLocationXY.y) 
	{
		reward = goalReward;
		goalReached = true;
	}
	else {
		reward = stepPenalty;
	}
	return reward;
}

// Models environment transitions (i.e. returns the next state s', given the current state and selected action
sf::Vector2i Environment::getNextStateXY(sf::Vector2i currentStateXY, int action, Grid& grid)
{
	// work out the agent's next position, x=1 y=1 is at the bottom left corner of the grid
	// actions which would move the agent off the grid will leave its position unchanged		
	sf::Vector2i nextStateXY = sf::Vector2i(-1, -1);

	if (action == 0) { // move north
		if (currentStateXY.y > 1) { // ensure agent is not at northmost row
			nextStateXY = sf::Vector2i(currentStateXY.x, currentStateXY.y - 1);
		}
		else { // keep agent at current position if this action would move it off the grid
			nextStateXY = sf::Vector2i(currentStateXY.x, currentStateXY.y);
		}
	}
	else if (action == 1) { // move east
		if (currentStateXY.x < xDimension) { // ensure agent is not at eastmost column
			nextStateXY = sf::Vector2i(currentStateXY.x + 1, currentStateXY.y);
		}
		else { // keep agent at current position if this action would move it off the grid
			nextStateXY = sf::Vector2i(currentStateXY.x, currentStateXY.y);
		}
	}
	else if (action == 2) { // move south
		if (currentStateXY.y < yDimension) {  // ensure agent is not at southmost row
			nextStateXY = sf::Vector2i(currentStateXY.x, currentStateXY.y + 1);
		}
		else { // keep agent at current position if this action would move it off the grid
			nextStateXY = sf::Vector2i(currentStateXY.x, currentStateXY.y);
		}
	}
	else if (action == 3) { // move west
		if (currentStateXY.x > 1) { // ensure agent is not at westmost column
			nextStateXY = sf::Vector2i(currentStateXY.x - 1, currentStateXY.y);
		}
		else { // keep agent at current position if this action would move it off the grid
			nextStateXY = sf::Vector2i(currentStateXY.x, currentStateXY.y);
		}
	}

	if (grid.nodes[nextStateXY.x - 1][nextStateXY.y - 1].containsObstacle)
	{
		nextStateXY = sf::Vector2i(currentStateXY.x, currentStateXY.y);
	}

	return nextStateXY;
}

// Mixed radix conversion (converts a state vector [x,y] to a single integer)
int Environment::getStateNoFromXY(sf::Vector2i state, Grid& grid)
{
//	int numStates = grid.gridSize * grid.gridSize;
	int stateNo = 0;

	stateNo = (state.y - 1) * yDimension;    // Number of rows
	stateNo = stateNo + state.x;                // Number of columns
	stateNo = stateNo + (xDimension * yDimension * (goalNo - 1));	// Number of goals

	//check state is allowed
	if (stateNo > numStates || stateNo < 0) {
		std::cout << "Vector Conversion Error - X: " << state.x << " Y: " << state.y << std::endl;
		std::cout << "Resultant State Number " << stateNo << std::endl;
		std::cout << "max allowed states: " << numStates << std::endl;
	}
	return stateNo;
}

// mixed radix conversion (converts a state number to a state vector [x,y]
sf::Vector2i Environment::getXYFromStateNo(int stateNo, Grid& grid)
{
	//check state is allowed
	if (stateNo > numStates || stateNo < 0) {
		std::cout << "Vector Conversion Error! " << std::endl;
		std::cout << "State Number " << stateNo << std::endl;
		std::cout << "max allowed states: " << numStates << std::endl;
		return sf::Vector2i(0, 0);
	}
	int x;
	int y;
	x = stateNo - goalNo;
	x = x % grid.gridSize;
	if (x == 0)                         // Check boundary conditions
		x = grid.gridSize;
	y = stateNo / grid.gridSize;
	if (stateNo % grid.gridSize != 0)   // Check boundary conditions
		y++;

	return sf::Vector2i(x, y);
}

void Environment::decayAlpha() 
{
	if (alphaDecays) 
	{
		alpha = alpha * alphaDecayRate;
		agent.alpha = alpha;
	}
}

void Environment::decayEpsilon() {
	if (epsilonDecays) 
	{
		epsilon = epsilon * epsilonDecayRate;
		agent.epsilon = epsilon;
	}
}

int Environment::getNumStates(Grid& grid)
{
	return xDimension * yDimension * grid.m_gridHealthPills.size();
}

void Environment::enableDebugging() 
{
	this->debug = true;
}

void Environment::disableDebugging() 
{
	this->debug = false;
	agent.disableDebugging();
}

// returns a formatted Q values table
string Environment::qTableAsString(float** qTable, Grid& grid)
{
	//DecimalFormat df = new DecimalFormat("0.000");

	string output = "State#	Xcoord	Ycoord	|	North	East	South	West";
	for (int s = 0; s < getNumStates(grid); s++)
	{
		sf::Vector2i coord = getXYFromStateNo(s, grid);
		output = output + "\n" + to_string(s) + "		" + to_string(coord.x) + "		" + to_string(coord.y) + "		|	" + to_string(qTable[s][0]) + "	" + to_string(qTable[s][1]) + "	" + to_string(qTable[s][2]) + "	" + to_string(qTable[s][3]);
	}
	return output;
}
