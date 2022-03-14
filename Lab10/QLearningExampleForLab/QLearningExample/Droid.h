#ifndef DROID_H
#define DROID_H

#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "Routine.h"
#include "HealthPill.h"
#include "Environment.h"
#include <string>

using namespace std;

static const float pi = 3.141592654f;

class Droid
{
public:

    string name;
    int x;
    int y;
    sf::Vector2i goalLocationXY;
    int range;
    int damage;
    float health;
    int lowHealth = 40;
    bool danger = false;                //Set inside the CheckForAlarms Behaviour
    bool alarmHasBeenRaised = false;    //Set inside the CheckForAlarms Behaviour
    sf::Vector2f position;
    sf::Vector2f target;
    sf::Vector2f velocity;
    bool loadFromFile = false;
    bool learning = false;
    int goalNo = 0;
    bool isAtGoal = false;
    bool debug = false;

    Routine* brain;
    Routine* behaviour;
    Environment env;

    // agent parameters, default values
    float alpha = 0.1f;     // learning rate
    float gamma = 0.9f;     // discount factor
    float epsilon = 0.1f;   // exploration rate
    int numStates = 0;
    int numActions = 4;
    char actionLabels[4][10] = { "North","East","South","West" };
    vector<vector<float>> qTable;

    Droid(string name, int x, int y, int health, int damage, int range, Grid& grid);
    void update(Grid& grid);
    void draw(sf::RenderWindow& t_window, Grid& grid);
    bool isAlive();
    bool isLowHealth();
    void addHealth(int newHealth);
    float getNewOrientation(Grid& grid);
    void handleCollisions(Grid& grid);
    bool doLearning(Grid& grid);
//    void initialiseQvalues(int numStates, int numActions);
//    void updateQValue(int previousState, int selectedAction, int currentState, float reward);
//    float getMaxQValue(int state);
    int getMaxValuedAction(int state);
//    int selectAction(int state);
//    int selectRandomAction();
    int getStateNoFromXY(sf::Vector2i state, Grid& grid);
    sf::Vector2i getNextStateXY(sf::Vector2i currentStateXY, int action, Grid& grid);
    sf::Vector2i getXYFromStateNo(int stateNo, Grid& grid);
    void writeQtableToFile();
    void readQtableFromFile();
    void initialiseQvalues();
    void setDroidGoalLocation(int x, int y);

    Routine* getBehaviour();
    void setBehaviour(Routine* routine);
    Routine* getBrain();
    void setBrain(Routine* routine);
    sf::Vector2f normalize(const sf::Vector2f source);

//    sf::Sprite objSprite;
    sf::Texture droidTex;
    sf::Sprite droidSprite;

};
#endif