#include "Droid.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "MoveAway.h"
#include "MoveTo.h"

using namespace std;

Droid::Droid(string name, int x, int y, int health, int damage, int range, Grid& grid)
{
    this->name = name;
    this->x = x;
    this->y = y;
    this->range = range;
    this->damage = damage;
    this->health = health;
    position = grid.getGridLocation(x, y);
    target = position;

    if (!droidTex.loadFromFile("ASSETS\\IMAGES\\BB-8.png"))
    {
        // simple error message if previous call fails
        std::cout << "problem loading sprite" << std::endl;
    }
    sf::Vector2u z = droidTex.getSize();
    droidSprite.setTexture(droidTex);
    droidSprite.setScale(sf::Vector2f(0.002f * grid.nodes[0][0].s_radius, 0.002f * grid.nodes[0][0].s_radius));
    droidSprite.setOrigin(z.x / 2, z.y / 2);
    droidSprite.setPosition(position);

    numStates = grid.gridSize * grid.gridSize * grid.m_gridHealthPills.size();  // Multiple goals means a lot more states

}

bool Droid::doLearning(Grid& grid)
{
    std::cout << "Starting training for " << name << std::endl;
    // initialise the environment and run the experiment
    env = Environment(grid);
    //env.enableDebugging();
    env.setAgentStartXY(x, y);
    env.doExperiment(name, grid);
    qTable = env.copy;
    writeQtableToFile();
    std::cout << "Finished training for " << name << std::endl;
    return true;

}

/// <summary>
/// This method demonstrates following the q-values to the goal.
/// It uses a moveTo behaviour but does not cater for changes in bahaviour or alarm conditions.
/// The behaviour is reset each time to moveTo the neighbour grid cell with the highest q-value.
/// </summary>
/// <param name="grid"></param>
void Droid::update(Grid& grid)
{
    if (!isAlive())
    {
        std::cout << name + " is dead! " << std::endl;
        return;
    }
    if (isAtGoal)
        return;

    // We are assuming there is only one goal i.e. the first Health Pill in the array
    setDroidGoalLocation(grid.m_gridHealthPills[goalNo - 1]->x, grid.m_gridHealthPills[goalNo - 1]->y);

    if (learning)
    {
        if (doLearning(grid))   // Generate the qTable
        {
            std::cout << name + " learning successfull. " << std::endl;
            learning = false;
        }
        else
            std::cout << name + " problem encountered with the learning process. " << std::endl;
        return;
    }

    if (loadFromFile)
    {
        initialiseQvalues();
        readQtableFromFile();
        loadFromFile = false;
        learning = false;
    }

    // q-Learning finished/q-table loaded, lets follow the path of highest qValues
    droidSprite.setPosition(position);
    x = grid.getGridCellX(sf::Vector2i(position));
    y = grid.getGridCellY(sf::Vector2i(position));
        
    if (health > 0)
        health = health - 0.1;

    int currState = getStateNoFromXY(sf::Vector2i(x, y), grid);
    int nextAction = getMaxValuedAction(currState);
    sf::Vector2i nextCell = getNextStateXY(sf::Vector2i(x, y), nextAction, grid);

    if (behaviour->isSuccess())
    {
        if (x == goalLocationXY.x && y == goalLocationXY.y) // We have reached the goal.
        {
            std::cout << "Droid " << name << " has reached the goal!" << std::endl;
            isAtGoal = true;
            return;
        }
        else
        {
            Routine* nextLocation = new MoveTo(nextCell.x, nextCell.y, grid);
            setBehaviour(nextLocation);
        }
    }
    else if (behaviour->getState() == behaviour->RoutineState::None)
    {
        Routine* nextLocation = new MoveTo(nextCell.x, nextCell.y, grid);
        setBehaviour(nextLocation);
    }

    // We use the brain behaviour as a sort of master controller.
    // The brain behaviour always runs first and checks to see have any alarm conditions arisen while the BT has been executing.
    // If so we will have specific code here to handle them. We will also set a flag so that the current behaviour executing in the BT will make itself Fail,
    // thereby forcing a re-evaluation of the BT from the root.
    // This is only necessary for those behaviours that take time to run e.g. MoveTo, GetHealth
    if (brain->getState() == behaviour->RoutineState::None)
    {
        // hasn't started yet so we start it
        brain->start(". This is the Brain node for " + name);
    }
    brain->act(this, grid); // We have set an empty Brain so nothing happens here.
    if (brain->isSuccess()) // Success means some alarm condition has been found.
    {
        // Handle the specific alarm
        // For now it is simply to execute the first step in a MoveAway behaviour.
        // The normal Behaviour tree will Fail() due to the danger variable being set within the brain behaviour.
        if (isLowHealth())
        {
            // Specific actions for lowHealth alarm
        }
        else
        {
            // Specific actions for enemy nearby
            MoveAway moveAway = MoveAway(grid);
            moveAway.start(" as Alarm for " + name);
            moveAway.act(this, grid);
        }
    }
    // Now run the main BT
    if (behaviour->getState() == behaviour->RoutineState::None)
    {
        // hasn't started yet so we start it
        behaviour->start(" for " + name);
    }
    behaviour->act(this, grid);

    handleCollisions(grid);
    brain->reset(" from the Droid update.");    //We want this to run every time.
}

float Droid::getNewOrientation(Grid& grid) {
    sf::Vector2f direction = target - position;
    if (grid.length(direction) > 0) 
    {
        float rads = atan2(direction.y, direction.x);
        float newAngle = rads * 180 / pi;   // Convert to degrees
        return newAngle + 90;	            // Becasue SFML orientation has 0 degrees pointing up, while normal trigonometry has 0 along the x axis.
    }
    else
        return 0;
}

bool Droid::isAlive()
{
    return health > 0;
}

void Droid::draw(sf::RenderWindow& t_window, Grid& grid)
{
    if (!isAlive())
    {
        droidSprite.setColor(sf::Color::Black);
    }
    else if (danger)     // For the case where we are in proximity of danger.
    {
        droidSprite.setColor(sf::Color::Red);
        //danger = false;       // This should be set specifically by a behaviour
    }
    else if (alarmHasBeenRaised && !isLowHealth())
        droidSprite.setColor(sf::Color::Green);
    else if (isLowHealth())
            droidSprite.setColor(sf::Color::Cyan);
    else
        droidSprite.setColor(sf::Color::White);

    t_window.draw(droidSprite); //Position of sprite set by the MoveTo behaviour
}

Routine* Droid::getBehaviour() {
    return behaviour;
}

void Droid::setBehaviour(Routine* routine) {
    this->behaviour = routine;
}

Routine* Droid::getBrain() {
    return brain;
}

void Droid::setBrain(Routine* routine) {
    this->brain = routine;
}

//Normalise a vector
sf::Vector2f Droid::normalize(const sf::Vector2f source)
{
    float length = sqrt((source.x * source.x) + (source.y * source.y));
    if (length != 0)
        return sf::Vector2f(source.x / length, source.y / length);
    else
        return source;
}

bool Droid::isLowHealth()
{
    return (health < lowHealth);
}

void Droid::addHealth(int newHealth)
{
    health += newHealth;
    std::cout << name + " has picked up some extra health. Value " << newHealth << std::endl;

}

void Droid::handleCollisions(Grid& grid)
{
    for (HealthPill* hp : grid.m_gridHealthPills)
    {
        if (hp->visible)
        {
            sf::Vector2f direction = grid.getGridLocation(hp->x, hp->y) - position;
            //        int l = (int) grid.length(direction);
            if ((int)grid.length(direction) == 0)
                //            if (x == hp->x && y == hp->y)
            {
                hp->HasBeenPickedUp();
                addHealth(hp->strength);
            }
        }
    }
}

int Droid::getMaxValuedAction(int state)
{
    // greedy action selection implementation
    // return the index of the most valuable action for a particular state
    int maxIndex = -1;
    float maxValue = -FLT_MAX;
    for (int action = 0; action < numActions; action++) 
    {
        if (qTable[state-1][action] > maxValue) {
            maxIndex = action;
            maxValue = qTable[state-1][action];
        }
    }
    return maxIndex;
}

// Mixed radix conversion (converts a state vector [x,y] to a single integer)
int Droid::getStateNoFromXY(sf::Vector2i state, Grid& grid)
{
//    int numStates = grid.gridSize * grid.gridSize;
    int stateNo = 0;

    stateNo = (state.y - 1) * grid.gridSize;    // Number of rows
    stateNo = stateNo + state.x;                // Number of columns
    stateNo = stateNo + (grid.gridSize * grid.gridSize * (goalNo - 1));	// Number of goals

    //check state is allowed
    if (stateNo > numStates || stateNo < 0) {
        std::cout << "Vector Conversion Error - X: " << state.x << " Y: " << state.y << std::endl;
        std::cout << "Resultant State Number " << stateNo << std::endl;
        std::cout << "max allowed states: " << numStates << std::endl;
    }
    return stateNo;
}

// Models environment transitions (i.e. returns the next state s', given the current state and selected action
sf::Vector2i Droid::getNextStateXY(sf::Vector2i currentStateXY, int action, Grid& grid)
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
        if (currentStateXY.x < grid.gridSize) { // ensure agent is not at eastmost column
            nextStateXY = sf::Vector2i(currentStateXY.x + 1, currentStateXY.y);
        }
        else { // keep agent at current position if this action would move it off the grid
            nextStateXY = sf::Vector2i(currentStateXY.x, currentStateXY.y);
        }
    }
    else if (action == 2) { // move south
        if (currentStateXY.y < grid.gridSize) {  // ensure agent is not at southmost row
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

    return nextStateXY;
}

// mixed radix conversion (converts a state number to a state vector [x,y]
sf::Vector2i Droid::getXYFromStateNo(int stateNo, Grid& grid)
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
    x = stateNo % grid.gridSize;
    if (x == 0)                         // Check boundary conditions
        x = grid.gridSize;
    y = stateNo / grid.gridSize;
    if (stateNo % grid.gridSize != 0)   // Check boundary conditions
        y++;

    return sf::Vector2i(x, y);
}

void Droid::writeQtableToFile()
{
    std::ofstream qTablefile;
    string fName = "qTable_" + name + ".csv";
    qTablefile.open(fName);

    for (int s = 0; s < numStates; s++)
    {
        qTablefile << s <<",";
        for (int a = 0; a < numActions; a++)
        {
            float f = qTable[s][a];
            qTablefile << f;
            if (a < numActions-1)
                qTablefile << ",";
        }
        qTablefile << std::endl;
    }
    qTablefile.close();
}

void Droid::readQtableFromFile()
{
    string line, word;
    int index = 0;
    float qValue = 0.0f;
    std::ifstream qTablefile;
    string fName = "qTable_" + name + ".csv";

    qTablefile.open(fName, ios::in);
    if (!qTablefile)
    {
        cout << "Could not open file: " << fName << std::endl;
        return;
    }
    while (!qTablefile.eof())
    {
        for (int st = 0; st < numStates; st++)
        {
            getline(qTablefile, line);
//            std::cout << "Reading qTable: " << line << std::endl;
            // used for breaking words 
            stringstream s(line);
            // read every column data of a row and store it in a string variable, 'word'
            getline(s, word, ',');  // Ignore the first value which is the state;
            int tempState = stof(word);
            for (int a = 0; a < numActions; a++)
            {
                getline(s, word, ',');
                {
                    qValue = stof(word);
                    qTable[st][a] = qValue;
                }
            }
        }
        getline(qTablefile, line);  // EOF?
    }
    qTablefile.close();
}

void Droid::initialiseQvalues()
{
    std::cout << "Initialising Q values for Droid: " << name << std::endl;

    qTable = vector<vector<float>>(numStates);

    for (int s = 0; s < numStates; s++)
    {
        qTable[s] = vector<float>(numActions);
        for (int a = 0; a < numActions; a++)
        {
            qTable[s][a] = 0.0f;
        }
    }

}

void Droid::setDroidGoalLocation(int x, int y)
{
    goalLocationXY = sf::Vector2i(x, y);
}
