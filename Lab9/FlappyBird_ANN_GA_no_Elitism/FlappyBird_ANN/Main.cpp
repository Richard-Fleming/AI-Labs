
// Based on the Python version: https://jatinmandav.wordpress.com/2018/03/05/a-i-learns-to-play-flappy-bird/
// Ported to C++ by Dr. Oisin Cawley, IT Carlow.
// Added Tournament selection and Elitism to the GA.
// Added crossover and mutation to Biases also.

// This version will train the Brid's neural network via a Genetic Algorithm
// Key press option 'M' to save the current network to file.
// Key press option 'C' to start capturing the environment and neural network decision if we wanted to generate test data for example.

// See Game.h for controlling if it executes the GA to train the NN, or Player mode, or to load a NN from file.

#include "Game.h"

/// <summary>
/// Main entry to the program 
/// 
/// 
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
/// <returns></returns>
int main(int, char*[])
{

	srand(time(NULL));

	Game game;
	game.init();
	return 0;
}