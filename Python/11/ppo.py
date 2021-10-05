import gym
import numpy as np
from tensorboardX import SummaryWriter

from actor import Actor
from critic import Critic

# Reward discount factor.
GAMMA = 0.99
# Memory size.
MEMORY_SIZE = 256
# Environment name.
ENV = 'LunarLander-v2'


class PPO:

    def __init__(self, env_class):
        """
		:param env_class: env class.
		"""

        self.env_class = env_class

        # Keep logger.
        self.writer = SummaryWriter(self.get_name())
        # Initialize gradient steps to log.
        self.gradient_steps = 0
        # Initialize episode number.
        self.episode = 0

        # we are using a dummy env instance to get state dimension and action dimension.
        dummy_env = env_class()
        self.state_dimension = 2
        self.action_dimension = 3

        # construct actor and critic networks.
        self.actor = Actor(self.state_dimension, self.action_dimension)
        self.critic = Critic(self.state_dimension)

        # Create environment.
        self.env = env_class()

    def get_name(self):
        name = 'AllRuns/'
        name += 'discrete/'
        name += ENV
        return name

    def update(self):
        batch_states, batch_actions, batch_predictions, batch_rewards = self.sampler()
        obs, action, pred, reward = batch_states[:MEMORY_SIZE], batch_actions[:MEMORY_SIZE], batch_predictions[:MEMORY_SIZE], batch_rewards[:MEMORY_SIZE]
        old_prediction = pred
        pred_values = self.critic.predict(obs)

        advantage = reward - pred_values
        actor_loss = self.actor.fit(action, obs, advantage, old_prediction)
        critic_loss = self.critic.fit(obs, reward)
        self.writer.add_scalar('Actor loss', actor_loss.history['loss'][-1], self.gradient_steps)
        self.writer.add_scalar('Critic loss', critic_loss.history['loss'][-1], self.gradient_steps)

        self.gradient_steps += 1

    def sampler(self):
        batch = [[], [], [], []]
        tmp_batch = [[], [], [], []]
        current_state = self.env.reset()

        while len(batch[0]) < MEMORY_SIZE:
            action, action_matrix, predicted_action = self.actor.get_action(np.array(current_state))

            state, reward, done, info = self.env.step(action)

            tmp_batch[0].append(np.array(current_state))
            tmp_batch[1].append(action_matrix)
            tmp_batch[2].append(predicted_action)
            tmp_batch[3].append(reward)
            current_state = np.array(state)

            if done:
                tmp_batch[3] = self.transform_reward(tmp_batch[3])
                for i in range(len(tmp_batch[0])):
                    obs, action, pred, r = tmp_batch[0][i], tmp_batch[1][i], tmp_batch[2][i], tmp_batch[3][i]
                    batch[0].append(obs)
                    batch[1].append(action)
                    batch[2].append(pred)
                    batch[3].append(r)
                tmp_batch = [[], [], [], []]
                current_state = self.reset_env()

        obs, action, pred, reward = np.array(batch[0]), np.array(batch[1]), np.array(batch[2]), np.reshape(np.array(batch[3]), (len(batch[3]), 1))
        pred = np.reshape(pred, (pred.shape[0], pred.shape[2]))
        return obs, action, pred, reward

    def transform_reward(self, reward):
        print('Episode reward', np.array(reward).sum(), self.episode)
        self.writer.add_scalar('Episode reward', np.array(reward).sum(), self.episode)
        for j in range(len(reward) - 2, -1, -1):
            reward[j] += reward[j + 1] * GAMMA
        return reward

    def reset_env(self):
        self.episode += 1
        current_state = self.env.reset()
        return current_state
