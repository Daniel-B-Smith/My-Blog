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
        -inf and inf as the two sentinel nodes.

        UNSAFE OUTSIDE OF SKIP LIST!
        """
        self.right.left = self.__class__(value)
        self.right.left.right = self.right
        self.right = self.right.left
        self.right.left = self
    
    def __str__(self):
        return "A skip list node with value {}".format(self.value)

class SkipList(object):
    def __init__(self):
        self.head = SLNode(float("-inf"))
        self._tail = SLNode(float("inf"))
        self.head.right = self._tail
        self._tail.left = self.head

    def _init_level(self):
        self.head.up = SLNode(-np.inf)
        self.head.up.down = self.head
        self.head = self.head.up
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
        while node!=stop and node.value<float("inf"):
            yield node.value
            node = node.right

    def insert(self, value):
        node = self.head
        while node.down:
            if value < node.right.value:
                node = node.down
            else:
                node = node.right
        while value > node.right.value:
            node = node.right
        node(value)
        self._raise_level(node.right)

    def find(self, value):
        node = self.head
        while node.down:
            if value < node.right.value:
                node = node.down
            else:
                node = node.right
        while value >= node.value:
            if value==node.value:
                return node
            else:
                node = node.right
        return None

    def _delete(self, node):
        node.right.left = node.left
        node.left.right = node.right

    def delete(self, value):
        node = self.find(value)
        if node is None:
            raise ValueError('{} not in skip list'.format(value))
        else:
            while node.up:
                self._delete(node)
                upnode = node.up
                del node.up
                node = upnode
                del node.down
                del upnode
            self._delete(node)

    def _raise_level(self, node):
        if self._raise_check():
            up_check = node.left
            while up_check.up is None and up_check.left:
                up_check = up_check.left
            if up_check.up is not None:
                place_node = up_check.up
            else:
                self._init_level()
                place_node = self.head
            place_node(node.value)
            node.up = place_node.right
            place_node.right.down = node
            self._raise_level(self.head.right)

    def _raise_check(self):
        return (np.random.rand()>0.5)

    def __str__(self):
        return "Skip list object"

    def _que_append(self, node, queue):
        if node and node not in queue:
            queue.append(node)

    def reset(self):
        del_queue = [self.head]
        while del_queue:
            node = del_queue.pop(0)
            if hasattr(node, 'up'):
                del node.up
            if hasattr(node, 'down'):
                self._que_append(node.down, del_queue)
                del node.down
            if hasattr(node, 'right'):
                self._que_append(node.right, del_queue)
                del node.right
            if hasattr(node, 'left'):
                del node.left
        self.__init__()

    """
    def __del__(self):
        self.reset()
        del self.head.right
        del self.head
        del self._tail.left
        del self._tail
    """

def main():
    print "Nothing to see here!"
    return 0

if __name__ == "__main__":
    sys.exit(main())
