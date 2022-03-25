# Based on:
# https://pythonprogramming.net/openai-cartpole-neural-network-example-machine-learning-tutorial/
# Upadated by Dr. Cawley IT Carlow 
#	- works for Tensorflow > 2.1.0
#	- simplified ANN architecture

import gym
import random
import numpy as np
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import layers
from statistics import mean, median
from collections import Counter

# If you leave out the .env below it sets a limit of 200 for the score.
# With .env there is no limit except whatever you set in the goal_steps variable
env = gym.make('CartPole-v0').env 
env.reset()
goal_steps = 500
score_requirement = 50
initial_games = 10000

def some_random_games_first():
    # Each of these is its own game.
    for episode in range(10):
        env.reset()
        score = 0		
        # this is each frame, up to 500...but we wont make it that far.
        for t in range(goal_steps):
            print("Step: ", t)   # Show how many steps it actually achieves in each game
            # This will display the environment
            # Only display if you really want to see it.
            # Takes much longer train if you display it, but still quick enough in this situation.
            env.render()
            # This will just create a sample action in any environment.
            # In this environment, the action can be 0 or 1, which is left or right
            action = env.action_space.sample()
            # This executes an action in the environment, 
            # and returns: the observation of the environment, the reward, if the env is over, and other info if any.
            observation, reward, done, info = env.step(action)
            score = score + reward
            if done:
                print("Score: ", score)
                break

# We play lots of random games and use games that score high to train the network.
def initial_population():
    # [OBS, MOVES]
    training_data = []
    # all scores:
    scores = []
    # just the scores that met our threshold:
    accepted_scores = [] #Only store scores > 50 (score_requirement)

    print("Generating random games...")
    # Iterate through however many games we want.
	# Note we are rendering the game.
    for _ in range(initial_games):
        score = 0
        # moves specifically from this environment:
        game_memory = []
        # previous observation that we saw
        prev_observation = []
        # for each frame in 500
        for _ in range(goal_steps):
            # choose random action (0 or 1)
            action = random.randrange(0,2) # Returns either 0 or 1
            # make that move in the game and observe what happens
            observation, reward, done, info = env.step(action)
            
            # notice that the observation is returned FROM the action
            # so we'll store the previous observation here, pairing
            # the prev observation to the action we'll take.            
            if len(prev_observation) > 0:
                game_memory.append([prev_observation, action])
                
            prev_observation = observation
            score += reward
            if done:	# The environment will tell us if we are done.
                break

        # If our score is higher than our threshold, we'd like to save every move we made in this game and use it to train the model.
        # NOTE the reinforcement methodology here. 
        # All we're doing is reinforcing the score, we're not trying 
        # to influence the machine in any way as to HOW that score is reached.            
        if score > score_requirement:
            accepted_scores.append(score)
            for data in game_memory:
                # convert to one-hot (this is the output layer for our neural network)
                if data[1] == 1:
                    output = [0,1]
                elif data[1] == 0:
                    output = [1,0]

                # saving our training data
                training_data.append([data[0], output]) #Save the observations and the input to the game
                
        # reset env to play again
        env.reset()
        # save overall scores
        scores.append(score)

    # just in case you wanted to reference later
    training_data_save = np.array(training_data)
    np.save('saved.npy', training_data_save)

    print('Average accepted score:', mean(accepted_scores))
    print('Median accepted score:', median(accepted_scores))
#    print(Counter(accepted_scores))

    return training_data            

def neural_network_model(input_size):

    model = tf.keras.Sequential()
    model.add(layers.Dense(1, input_dim=input_size, activation='relu'))
    model.add(layers.Dense(env.action_space.n, activation='softmax'))
    model.compile(loss='mse', optimizer='Adam')
	
    model.summary
    
    return model

def train_model(training_data, model=False):
    print("Training the model...")
    X = np.array([i[0] for i in training_data]).reshape(-1, len(training_data[0][0]), 1)
    y = [i[1] for i in training_data]
    X = np.squeeze(X)
    y = np.squeeze(y)
#    print(X)
#    print(y)
    
    if not model:
        model = neural_network_model(input_size = len(X[0]))
    
    model.fit(X, y, epochs=3)	# Perform the network training
        
    return model


#some_random_games_first()	# This calls the function to play some games with random moves if you want to see it in action with no logic.
#exit()						# We'll stop it continuing if we are just doing the random games.

#If we want to train, then uncomment below and comment out the loading from file commands
#training_data = initial_population()
#model = train_model(training_data)
#model.summary()

#Use the trained network saved to file
model = neural_network_model(4)
model = keras.models.load_model('over150.model')

scores = []
choices = []

print("Using the trained model ...")
for each_game in range(10):	# We will play 10 games and get the average score
    score = 0
    game_memory = []
    prev_obs = []
    env.reset()
    for s in range(goal_steps):
#        print("Step ", s)
        env.render()
        if len(prev_obs) == 0:
            action = random.randrange(0,2)
        else:
            temp_obs = prev_obs.reshape(-1, len(prev_obs))  # Lots of issues trying to get the shape right for the prediction
            tempaction = model(temp_obs)        # Makes a prediction
            action = np.argmax(tempaction)		# Returns the index of the max value (so either 0 or 1 which will translate to either left or right)
        choices.append(action)
        
        new_observation, reward, done, info = env.step(action)
        prev_obs = new_observation
        game_memory.append([new_observation, action])
        score += reward
        if done:
            break
    print('Done with a score of ', score)
    scores.append(score)
#print(scores)
average = sum(scores)/len(scores)
print('Average Score', average)
if average > 150:
    print("Saving model...")
    model.save('over150.model')
print("Finished")


            
            
            
            
            
            
            
            
            
    
