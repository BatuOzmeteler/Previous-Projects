#!/usr/bin/env python3
# -*- coding: utf-8 -*-

class Person:
    def __init__(self, name):
        self.name = name

    def say_hi(self):
        print('Hello, my name is', self.name)
p = Person('Ali Veli')
p.say_hi()
