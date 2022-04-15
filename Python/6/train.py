import gym
import gym_foo
from ppo import PPO

EPISODES = 100000

def make_env():
    env = gym.make('foo-v0')
    return env

def train():
    ppo = PPO(make_env)

    for i in range(EPISODES):
        ppo.update()

if __name__ == "__main__":
    train()
