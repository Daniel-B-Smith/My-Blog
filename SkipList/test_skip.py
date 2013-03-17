#!/usr/bin/env python

import sys
import numpy as np
import numpy.testing as np_test

import skip_list

def test_node():
    left_node = skip_list.SLNode(12)
    assert left_node.value == 12

    right_node = skip_list.SLNode(13)
    left_node.right = right_node
    right_node.left = left_node

    assert right_node.left.right == right_node
    assert left_node.right.left == left_node

    up_node = right_node
    down_node = right_node
    up_node.down = down_node
    down_node.up = up_node

    assert up_node.down.up == up_node
    assert down_node.up.down == down_node

def test_skip_list():
    s_list = skip_list.SkipList()
    assert s_list.head.up is None
    assert s_list.head.left is None
    assert s_list.head.down is None
    assert s_list.head.right == s_list._tail
    assert s_list._tail.left == s_list.head
    assert s_list._tail.up is None
    assert s_list._tail.down is None
    assert s_list._tail.right is None

    s_list._init_level()
    assert s_list.head.down.value == -np.inf
    assert s_list._tail.down.value == np.inf
    assert s_list.head.right == s_list._tail
    assert s_list._tail.left == s_list.head

def test_skip_list_insert():
    np.random.seed(1234)

    data = [3, 1, 17, 5, 32]
    s_list = skip_list.SkipList()
    for datum in data:
        s_list.insert(datum)
    
    sorted_data = sorted(data)
    for i, val in enumerate(s_list.iterate()):
        assert sorted_data[i] == val

    out = [[             32],
           [         17, 32],
           [1,       17, 32], 
           [1, 3, 5, 17, 32]]

    node = s_list.head
    i = 0
    while node.down:
        for j, val in enumerate(s_list.iterate(start=node.right)):
            assert out[i][j] == val
        i += 1
        node = node.down
    for j, val in enumerate(s_list.iterate(start=node.right)):
        assert out[i][j] == val

    
if __name__ == "__main__":
    np_test.run_module_suite()
