In order to use our environment;

1- "pip install -e" command has to be run in 
'foo_gym' folder.
2- "train.py" can be run to train the agent in the selected environment.

In order to change the environment;

----------------------------
def make_env():
    env = gym.make('foo-v0')
    return env
----------------------------

This particular line ----> env = gym.make('foo-v0')
has to be modified. 'foo-v0' should be replaced with 
the desired environment ID. 
