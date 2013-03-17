#!/usr/bin/env python

import sys
import numpy as np

class SLNode(object):
    def __init__(self, value):
        self._value = value
        self.left = None
        self.right = None
        self.up = None
        self.down = None
    
    value = property(lambda self: self._value) 
    def __call__(self, value):
        """
        Use __call__ to insert a node into a skip list.
        self.right is always defined since a skip list is initialized with
        -np.inf and np.inf as the two sentinel nodes.

        UNSAFE OUTSIDE OF SKIP LIST!
        """
        self.right.left = self.__class__(value)
        self.right.left.right = self.right
        self.right = self.right.left
        self.right.left = self
    
    def __del__(self):
        if self.right and self.left:
            self.right.left = self.left
            self.left.right = self.right
        if self.up:
            del self.up
        if self.down:
            del self.down
        del self.value

    def __str__(self):
        return "A skip list node with value {}".format(self.value)

class SkipList(object):
    def __init__(self):
        self.head = SLNode(-np.inf)
        self._tail = SLNode(np.inf)
        self.head.right = self._tail
        self._tail.left = self.head

    def _init_level(self):
        self.head.up = SLNode(-np.inf)
        self.head.up.down = self.head
        self.head = self.head.up
        self._tail.up = SLNode(np.inf)
        self._tail.up.down = self._tail
        self._tail = self._tail.up
        self.head.right = self._tail
        self._tail.left = self.head

    def iterate(self, start=None, stop=None):
        """
        Creates a generator that loops through the data in the bottom
        list.
        """
        if start is None:
            node = self.head
            while node.down:
                node = node.down
            start = node.right

        node = start
        while node!=stop and node.value<np.inf:
            yield node.value
            node = node.right

    def insert(self, value):
        node = self.head.right
        while node.down:
            if value < node.right.value:
                node = node.down
            else:
                node = node.right
        while value > node.value:
            node = node.right
        node.left(value)
        self._raise_level(node.left)

    def _raise_level(self, node):
        if self._raise_check():
            up_check = node.left
            while up_check.up is None and up_check.value>-np.inf:
                up_check = up_check.left
            if up_check.up is not None:
                place_node = up_check.up
                place_node(node.value)
                node.up = place_node.right
                place_node.right.down = node
                self._raise_level(place_node.right)
            elif up_check.up is None:
                self._init_level()
                self.head(node.value)
                node.up = self.head.right
                self.head.right.down = node
                self._raise_level(self.head.right)

    def _raise_check(self):
        return (np.random.rand()>0.5)

    def __str__(self):
        return "Skip list object"

def main():
    print "Nothing to see here!"
    return 0

if __name__ == "__main__":
    sys.exit(main())
