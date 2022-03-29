# Landing pad is always at coordinates (0,0). Coordinates are the first
# two numbers in state vector. Reward for moving from the top of the screen
# to landing pad and zero speed is about 100..140 points. If lander moves
# away from landing pad it loses reward back. Episode finishes if the lander
# crashes or comes to rest, receiving additional -100 or +100 points.
# Each leg ground contact is +10. Firing main engine is -0.3 points each frame.
# Solved is 200 points. Landing outside landing pad is possible. Fuel is
# infinite, so an agent can learn to fly and then land on its first attempt.
# Four discrete actions available: do nothing, fire left orientation engine,
# fire main engine, fire right orientation engine.


# Issues with Tensorflow 2.1.0


import gym		# Will also require Box2D to be installed for the Lunar Lander env.
import random
import tensorflow as tf
from tensorflow import keras

import numpy as np
env = gym.make('LunarLander-v2')
env.seed(0)
np.random.seed(0)


if __name__ == '__main__':
    model = keras.models.load_model('lunar_model.h5')
    score = 0
    total_score = 0
    mean_score = 0
    episode = 20
    for e in range(episode):
        score = 0
        state = env.reset()
        state = np.reshape(state, (1, 8)) 
        for i in range(500):
            act_values = model.predict(state)
            env.render()
            next_state, reward, done, _ = env.step(np.argmax(act_values[0]))
            score += reward
            next_state = np.reshape(next_state, (1, 8))
            state = next_state
        
        print("episode: {}/{}, score: {}\n".format(e + 1, episode, score))
        total_score += score
        print("Average Score: {}\n".format(total_score / (e + 1)))