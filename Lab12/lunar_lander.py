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
from tensorflow.keras import layers
from collections import deque
import matplotlib.pyplot as plt

import numpy as np
env = gym.make('LunarLander-v2')
env.seed(0)
np.random.seed(0)


class DQN:

    """ Implementation of deep q learning algorithm """

    def __init__(self, action_space, state_space):

        self.action_space = action_space
        self.state_space = state_space
        self.epsilon = 1.0
        self.gamma = .99
        self.batch_size = 64
        self.epsilon_min = .01
        #self.lr = 0.001 # Defaults to .001 in the library
        self.epsilon_decay = .996
        self.memory = deque(maxlen=1000000)
        self.model = self.build_model()

    def build_model(self):

        model = tf.keras.Sequential()
        model.add(layers.Dense(128, input_dim=self.state_space, activation='relu'))
        model.add(layers.Dense(256, activation='relu'))
        model.add(layers.Dense(self.action_space, activation='linear'))
        model.compile(loss='mse', optimizer='Adam')
        model.summary()
        return model

    def remember(self, state, action, reward, next_state, done):
        self.memory.append((state, action, reward, next_state, done))

    def act(self, state):
        if np.random.rand() <= self.epsilon:
            return random.randrange(self.action_space)
        act_values = self.model.predict(state)
#        print(act_values)
        return np.argmax(act_values[0])

    def replay(self):

        if len(self.memory) < self.batch_size:
            return

        minibatch = random.sample(self.memory, self.batch_size)
        states = np.array([i[0] for i in minibatch])
        actions = np.array([i[1] for i in minibatch])
        rewards = np.array([i[2] for i in minibatch])
        next_states = np.array([i[3] for i in minibatch])
        dones = np.array([i[4] for i in minibatch])

        states = np.squeeze(states)
        next_states = np.squeeze(next_states)

        targets_full = self.model.predict_on_batch(states)
#        targets_full = targets_full.numpy()		# Oisin - Without this line (to convert to a numpy array) I was getting 
												# TypeError: 'tensorflow.python.framework.ops.EagerTensor' object does not support item assignment
												# on line: targets_full[[ind], [actions]] = targets
												# when running in tensorflow 2.1.0
												# Did not get this if running with tensorflow 2.3.0
        #targets_full = targets_full.flatten()
        targets = rewards + self.gamma*(np.amax(self.model.predict_on_batch(next_states), axis=1))*(1-dones) 	# Update the q-value
																												# 1-dones for previous runs which reached the end and so no point in predicting the next action (1-1=0)
																												
#        targets = rewards + self.gamma*(np.amax(self.model.predict_on_batch(next_states), axis=1))-np.amax(targets_full, axis=1) *(1-dones) 	# Update the q-value the same as in the q-learning equation 
																																				# which includes subtracting the original q-value
																																				# However, most examples do not do this.
        ind = np.array([i for i in range(self.batch_size)])
        targets_full[[ind], [actions]] = targets

        self.model.fit(states, targets_full, epochs=1, verbose=0)	# Train the network on the new q-values.
        if self.epsilon > self.epsilon_min:
            self.epsilon *= self.epsilon_decay


def train_dqn(episode):

    loss = []
    agent = DQN(env.action_space.n, env.observation_space.shape[0])
    for e in range(episode):
        state = env.reset()
        state = np.reshape(state, (1, 8))
        score = 0
        max_steps = 3000
        if e % 100 == 0:
            render = False
        else:
            render = False
        for i in range(max_steps):
            action = agent.act(state)
            #if render:
            env.render()
            next_state, reward, done, _ = env.step(action)
            score += reward
            next_state = np.reshape(next_state, (1, 8))
            agent.remember(state, action, reward, next_state, done)
            state = next_state
            agent.replay()
            if done:
                print("episode: {}/{}, score: {}".format(e, episode, score))
                break
        loss.append(score)

        # Average score of last 100 episode
        is_solved = np.mean(loss[-100:])
        if is_solved > 200:
            print('\n Task Completed! \n')
            # save model and architecture to single file
            agent.model.save("lunar_model.h5")
            break
        print("Average over last 100 episode: {0:.2f} \n".format(is_solved))
    return loss


if __name__ == '__main__':

    print(env.observation_space)
    print(env.action_space)
    episodes = 400
    loss = train_dqn(episodes)
    plt.plot([i+1 for i in range(0, len(loss), 2)], loss[::2])
    plt.show()