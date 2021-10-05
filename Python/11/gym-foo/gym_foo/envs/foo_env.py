# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
    
    OBSERVATION VALUES
    0 - No action is taken (only after reset)
    1 - Agent has passed the level
    2 - Agent couldn't pass the level
    
    MAP VALUES
    0 - Empty (Can Pass)
    1 - Obstacle (Can't Pass)
"""

import gym
import numpy
from gym import error, spaces, utils
from gym.utils import seeding


gameMap = numpy.matrix([[1, 0, 1], [0, 0, 1], [1, 0, 0], 
                    [0, 1, 1], [0, 1, 0], [1, 1, 0], 
                    [0, 0, 1], [1, 0, 1], [1, 0, 0]])

class FooEnv(gym.Env):
  metadata = {'render.modes': ['human']}
  
  def __init__(self):
      self.map = gameMap
      self.action_space = numpy.array([0, 1, 2])
      self.observation_space = numpy.array([1, 2])
      self.reset()
      
      
  def step(self, action):
      
      if self.rowPosition == 8 and self.map[self.rowPosition, action] == 0:
          self.done = True
        
      if self.done == False:
          if self.map[self.rowPosition, action] == 0:
              self.observation = [1, 0]
              self.reward = 1
          elif self.map[self.rowPosition, action] == 1:
              self.observation = [0, 1]
              self.reward = 0
      
      if self.observation == 1:
          self.rowPosition += 1
      else:
           self.done = True   
      
      return self.observation, self.reward, self.done, {"Row Position": self.rowPosition}
  
  def reset(self):
      self.observation = [0, 0]
      self.rowPosition = 0
      self.reward = 0
      self.done = False
      return self.observation
      

