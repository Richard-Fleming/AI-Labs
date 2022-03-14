#include "Population.h"
#include "Game.h"
#include <iostream>



Population::Population()
{
}

Population::~Population()
{
}

void Population::createPopulation(int populationSize)
{
	birdSet = new Bird[populationSize];
	for (int i = 0; i < populationSize; i++)
	{
		birdSet[i].init(sf::Color(rand() % (255), rand() % (255), rand() % (255)));
	}

}

// The game loop method
// We pass in the pillar it is approaching
int Population::update(Pillar pillar)
{
	int birdsAlive = 0;	// We want to know when all the Birds have died so we can evelove the next generation.
	for (int i = 0; i < Game::birdCount; i++)
	{
		if (birdSet[i].alive)
		{
			birdSet[i].update(pillar);
			birdsAlive++;
		}
	}

	return birdsAlive;
}

// Move the evolution process forward.
void Population::evolve()
{
	generation++;

	int elitismOffset = 0;
	int index = 0;
	int fittest;
	Bird a;
	Bird b;

	fittest = getFittest();
	
	std::cout << "Generation: " << generation << std::endl;
	std::cout << "Highest Fitness: " << birdSet[fittest].fitness << std::endl;

	newBirdSet = new Bird[Game::birdCount];

	// Is Elitism turned on for the population?
	// ...

	for (int i = 0; i < Game::birdCount; i++)
	{
		if (elitism && i == fittest)
		{
			newBirdSet[i] = birdSet[i];
		}
		else
		{
			int indexa = tournamentSelection();	// We select candidates to evolve using Tournament selection.
			int indexb = tournamentSelection();
			a = Bird();
			a.init(sf::Color(rand() % (255), rand() % (255), rand() % (255)));
			a.brain = crossOver(birdSet[indexa].brain, birdSet[indexb].brain);
			a.brain = mutate(a.brain);
			newBirdSet[i] = a;
		}
	}
	// Copy the new birdSet back into the population.
	for (int i = 0; i < Game::birdCount; i++)
	{
		birdSet[i] = newBirdSet[i];
	}
}

int Population::getFittest()
{
	int index = 0;
	int fittest = birdSet[0].fitness;

	for (int i = 0; i < Game::birdCount; i++)
	{
		if (birdSet[i].fitness > fittest)
		{
			fittest = birdSet[i].fitness;
			index = i;
		}
	}
	return index;
}

// The Genes in our population are the weights in the Neural Network.
Brain Population::crossOver(Brain a, Brain b)
{
	Brain newBrain;
	newBrain.init(5, 4, 1);

	// Crossover of weights in layer 1 
	for (int i = 0; i < a.numInputs; i++)
	{
		for (int j = 0; j < a.numHidden; j++)
		{
			int r = rand() % 2; // Randomly select one of the two Genes to keep.
			if (r == 0)
			{
				newBrain.weightsLayer1[i][j] = b.weightsLayer1[i][j];
				if (b.useBiases)
					newBrain.biases[i] = b.biases[i];
			}
			else
			{
				newBrain.weightsLayer1[i][j] = a.weightsLayer1[i][j];
				if (a.useBiases)
					newBrain.biases[i] = a.biases[i];
			}
		}
	}
	// Crossover of weights in output layer 
	for (int i = 0; i < a.numHidden; i++)
	{
			int r = rand() % 2;
			if (r == 0)
			{
				newBrain.weightsLayer2[i] = b.weightsLayer2[i];
			}
			else
			{
				newBrain.weightsLayer2[i] = a.weightsLayer2[i];
			}

	}
	return newBrain;
}
// Select individuals for crossover
int Population::tournamentSelection() 
{
	// Create a tournament population
	Population tournament;
	tournament.createPopulation(Game::birdCount);

	// For each place in the tournament get a random individual
	for (int i = 0; i < tournamentSize; i++) 
	{
		int randomId = rand() % Game::birdCount;
		tournament.birdSet[i] = birdSet[randomId];
	}
	// Get the fittest of the selected Birds.
	int fittest = tournament.getFittest();
	return fittest;
}

// Mutate an individual
Brain Population::mutate(Brain birdBrain) 
{
	Brain tempBrain = birdBrain;
	// Loop through the neuron weights
	for (int i = 0; i < tempBrain.numHidden; i++)
	{
		for (int j = 0; j < tempBrain.numInputs; j++)
		{
			float r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
			if (r <= mutationRate)	// Only mutate according to our mutation rate propability.
			{
				float w = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2))) - 1;	// Pick a new random weight for this gene.
				tempBrain.weightsLayer1[i][j] = w;
			}
			//			std::cout << "weights layer 1 " << i << ", " << j << " = " << this->weightsLayer1[i][j]<< std::endl;
		}
		// Mutate the Biases
		for (int j = 0; j < tempBrain.numHidden+1; j++)
		{
			float r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
			if (r <= mutationRate)	// Only mutate according to our mutation rate propability.
			{
				float w = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2))) - 1;	// Pick a new random weight for this gene.
				//std::cout << "Bias " << i << " mutated from " << birdBrain.biases[i] << " to " << w << " = " << std::endl;
				tempBrain.biases[j] = w;
			}
		}
		
	}
	return tempBrain;
}