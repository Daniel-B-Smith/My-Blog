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
        """
        self.right.left = self.__class__(value)
        self.right.left.right = self.right
        self.right = self.right.left
        self.right.left = self
    
    def __str__(self):
        return "A skip list node with value {}".format(self.value)

class SkipList(object):
    def __init__(self):
        self.head = []
        self.tail = []
        self._init_level()

    def _init_level(self):
        self.head.append(SLNode(-np.inf))
        self.tail.append(SLNode(np.inf))
        self.head[-1].right = self.tail[-1]
        self.tail[-1].left = self.head[-1]

    def iterate(self, start=None, stop=None):
        """
        Creates a generator that loops through the data in the bottom
        list.
        """
        if start is None:
            start = self.head[0].right
        if stop is None:
            stop = self.tail[0]

        node = start
        while node.right!=stop:
            yield node.value
            node = node.right
        
    def insert(self, value):
        node = self.head[-1].right
        while value < node.value or node.down is not None:
            if value > node.value:
                node = node.right
            elif value == node.value:
                node = node.down if node.down else node
            else:
                node = node.left.down if node.left.down else node.left
        node(value)
        self._raise_level(node.right)

    def _raise_level(self, node):
        if self._raise_check():
            up_check = node.left
            while up_check.up is None and up_check.value>-np.inf:
                up_check = up_check.left
            if up_check.value>-np.inf:
                node.up = up_check.up(node.value)
                self._raise_level(node.up)
            else:
                self._init_level()
                self.head[-1](node.value)


    def _raise_check(self):
        return (np.random.rand>0.5)
        

    def __str__(self):
        return "Skip list object"


def main():
    print "Nothing to see here!"
    return 0

if __name__ == "__main__":
    sys.exit(main())
