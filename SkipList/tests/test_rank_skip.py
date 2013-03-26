#!/usr/bin/env python

import sys
import numpy as np
import numpy.testing as np_test

import rank_skip_list

def test_node():
    left_node = rank_skip_list.RSLNode(12)
    assert left_node.value == 12

    right_node = rank_skip_list.RSLNode(13)
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

def test_rank_skip_list(s_list=None):
    if s_list is None:
        s_list = rank_skip_list.RankSkipList()
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
    assert s_list.head.right == s_list._tail
    assert s_list._tail.left == s_list.head

def _rank_skip_list_structure(s_list, structure):
    node = s_list.head
    i = 0
    while node.down:
        for j, val in enumerate(s_list.iterate(start=node.right)):
            assert structure[i][j] == val
        i += 1
        node = node.down
    for j, val in enumerate(s_list.iterate(start=node.right)):
        assert structure[i][j] == val

def test_rank_skip_list_insert():
    np.random.seed(1234)

    data = [3, 1, 17, 5, 32]
    s_list = rank_skip_list.RankSkipList()
    for datum in data:
        s_list.insert(datum)
    
    sorted_data = sorted(data)
    for i, val in enumerate(s_list.iterate()):
        assert sorted_data[i] == val

    out = [[             32],
           [         17, 32],
           [1,       17, 32], 
           [1, 3, 5, 17, 32]]

    _rank_skip_list_structure(s_list, out)

    node = s_list.find(13)
    assert node is None
    node = s_list.find(17)
    assert node.down is None
    assert node.value==17

    out = [[         32],
           [         32],
           [1,       32], 
           [1, 3, 5, 32]]

    s_list.delete(17)
    _rank_skip_list_structure(s_list, out)

    s_list.reset()
    test_rank_skip_list(s_list=s_list)

def _rank(data, val):
    return max((i for i, _ in enumerate(data) if _<= val))+1

def test_rank_method():
    np.random.seed(1234)

    data = [3, 1, 17, 5, 32]
    s_list = rank_skip_list.RankSkipList()
    for datum in data:
        s_list.insert(datum)

    data = sorted(data)
    test_ranks = [3, 17, 6, 8, 33, 11]
    for val in test_ranks:
        assert _rank(data, val) == s_list.rank(val)

    s_list.delete(17)
    s_list.delete(5)

    data.remove(17)
    data.remove(5)

    test_ranks = [3, 17, 6, 8, 33, 11]
    for val in test_ranks:
        assert _rank(data, val) == s_list.rank(val)

def test_rank_random():
    np.random.seed(667)

    N = 1000

    s_list = rank_skip_list.RankSkipList()
    data = np.random.rand(N)
    for datum in data:
        s_list.insert(datum)

    data.sort()
    test_ranks = np.hstack((np.random.rand(20), 
                            np.random.choice(data, size=20, replace=False)))
    for val in test_ranks:
        assert _rank(data, val) == s_list.rank(val)
    for index in np.random.randint(1, N+1, size=int(0.1*N)):
        assert data[index-1] == s_list.find_rank(index).value

    deletes = np.random.choice(data, size=int(0.1*N), replace=False)
    for d in deletes:
        s_list.delete(d)
        data = data[data!=d]

    for val in test_ranks:
        assert _rank(data, val) == s_list.rank(val)
    for index in np.random.randint(1, int(0.9*N+1), size=int(0.1*N)):
        assert data[index-1] == s_list.find_rank(index).value

if __name__ == "__main__":
    np_test.run_module_suite()
