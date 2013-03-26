#!/usr/bin/env python

import numpy as np
import numpy.testing as np_test
import pp_classify
import skip_classify
import rank_skip_list

def test_classify():
    np.random.seed(1234)

    N = 1000
    data = np.random.rand(N)
    data.sort()
    s_list = rank_skip_list.RankSkipList()
    for datum in data:
        s_list.insert(datum)

    c = 0.5
    b = 0.1
    s_class = skip_classify.classify_list(s_list, c, b)
    p_class = [pp_classify.classify(x, c, b) for x in data]
    np_test.assert_array_almost_equal(s_class, p_class)

    c = 0.2
    b = 0.2
    s_class = skip_classify.classify_list(s_list, c, b)
    p_class = [pp_classify.classify(x, c, b) for x in data]
    np_test.assert_array_almost_equal(s_class, p_class)

    c = 0.7
    b = 0.3
    s_class = skip_classify.classify_list(s_list, c, b)
    p_class = [pp_classify.classify(x, c, b) for x in data]
    np_test.assert_array_almost_equal(s_class, p_class)


if __name__ == "__main__":
    np_test.run_module_suite()
