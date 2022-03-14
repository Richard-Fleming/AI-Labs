
// The Genetic Algorithm population to be evolved.

#pragma once

#include "Bird.h"

class Population
{
public:
	Population();
	~Population();

	void createPopulation(int populationSize);
	int update(Pillar pillar);
	void evolve();
	int getFittest();
	int tournamentSelection();
	Brain mutate(Brain birdBrain);
	Brain crossOver(Brain a, Brain b);

	Bird *birdSet;
	Bird *newBirdSet;

	int generation = 0;
	int tournamentSize = 5;
	float mutationRate = 0.30;
	bool elitism = false;

};

