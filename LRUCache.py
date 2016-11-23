#!/usr/bin/env python3.4
from collections import OrderedDict

class LRUCache:

    def __init__(self, capacity):
        self.capacity = capacity
        self.cache = OrderedDict()

    def get(self, key):
        val = self.cache.get(key, -1)
        if val != -1:
            self.cache.move_to_end(key, last=True)
        return val

    def set(self, key, value):
        if self.cache.get(key, -1) != -1:
            self.cache.pop(key)
        if len(self.cache) == self.capacity:
            self.cache.popitem(last=False)

        self.cache[key] = value
