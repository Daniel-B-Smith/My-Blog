#!/usr/bin/env python

import sys
import numpy as np

class RSLNode(object):
    def __init__(self, value):
        self._value = value
        self.left = None
        self.right = None
        self.up = None
        self.down = None
        self.accum = 0
    
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
        self.right.accum += 1
    
    def __str__(self):
        return "A skip list node with value {}".format(self.value)

class RankSkipList(object):
    def __init__(self):
        self.head = RSLNode(float("-inf"))
        self._tail = RSLNode(float("inf"))
        self.head.right = self._tail
        self._tail.left = self.head
        self.n_items = 0

    def _init_level(self):
        self.head.up = RSLNode(-np.inf)
        self.head.up.down = self.head
        self.head = self.head.up
        self.head.right = self._tail
        self._tail.left = self.head

    def iterate(self, start=None, stop=None):
        """
        Creates a generator that loops through the data in the bottom
        list. If start and stop are defined, includes start and stops at
        stop.left, e.g. generates [start, stop)
        """
        if start is None:
            node = self.head
            while node.down:
                node = node.down
            start = node.right

        node = start
        while node!=stop and node.value < float("inf"):
            yield node.value
            node = node.right

    def insert(self, value):
        self.n_items += 1
        node = self.head
        while node.down:
            if value < node.right.value:
                node.right.accum += 1
                node = node.down
            else:
                node = node.right
        while value > node.right.value:
            node = node.right
        node(value)
        node.right.accum = 1
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
    
    def find_rank(self, rank):
        if rank <= 0 or rank > self.n_items:
            raise ValueError
        node = self.head
        while rank > 0 or node.down:
            if rank < node.right.accum:
                if node.down:
                    node = node.down
            else:
                node = node.right
                rank -= node.accum
        return node

    def _delete(self, node):
        node.right.left = node.left
        node.left.right = node.right

    def delete(self, value):
        node = self.head
        while node.value != value or node.down:
            if value < node.right.value:
                if node.down:
                    node.right.accum -= 1
                    node = node.down
                else:
                    raise ValueError('value not in list')
            else:
                node = node.right
        while node.up:
            self._delete(node)
            upnode = node.up
            del node.up
            node = upnode
            node.right.accum += node.accum
            del node.down
            del upnode
        self._delete(node)

    def _raise_level(self, node, accum=None):
        if accum is None:
            accum = 1
        if self._raise_check():
            up_check = node.left
            while up_check.up is None and up_check.value>float("-inf"):
                accum += up_check.accum
                up_check = up_check.left
            if up_check.up:
                place_node = up_check.up
                place_node(node.value)
                place_node.right.accum = accum
                place_node.right.right.accum -= accum
                node.up = place_node.right
                place_node.right.down = node
                self._raise_level(place_node.right, accum=accum)
            else:
                self._init_level()
                self.head(node.value)
                self.head.right.accum = accum
                node.up = self.head.right
                self.head.right.down = node
                self._raise_level(self.head.right, accum=accum)

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

    def rank(self, value):
        node = self.head
        rank = 0
        while value >= node.value:
            if value < node.right.value:
                if node.down:
                    node = node.down
                else:
                    break
            else:
                node = node.right
                rank += node.accum
        return rank

    def __del__(self):
        self.reset()
        del self.head.right
        del self.head
        del self._tail.left
        del self._tail

    def count(self):
        return self.n_items

def main():
    print "Nothing to see here!"
    return 0

if __name__ == "__main__":
    sys.exit(main())
