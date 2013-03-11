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
    assert len(s_list.head)==1
    assert len(s_list.tail)==1
    assert s_list.head[0].right == s_list.tail[0]
    assert s_list.tail[0].left == s_list.head[0]

def test_skip_list_insert():
    np.random.seed(1234)

    data = [3, 1, 17, 5, 32, 11, 14, 91, 27]
    s_list = skip_list.SkipList()
    for datum in data:
        s_list.insert(datum)
    
    sort_data = sorted(data)
    for i, val in enumerate(s_list.iterate()):
        assert sorted_data[i] == val

if __name__ == "__main__":
    np_test.run_module_suite()
