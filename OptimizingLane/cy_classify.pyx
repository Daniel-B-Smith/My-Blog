import numpy as np

cimport numpy as np
cimport cython

def classify(x, c, b):
    """
    Test classification function. Takes test result x, cut-off c and bandwidth
    b.
    """
    if x<c-b:
        return 0.
    elif x>c+b:
        return 1.
    else:
        if b>10**-7:
            return (x-c+b)/2/b
        else:
            return 0.5

def type_classify(float x, float c, float b):
    """
    Test classification function. Takes test result x, cut-off c and bandwidth
    b.
    """
    if x<c-b:
        return 0
    elif x>c+b:
        return 1
    else:
        if b>10**-7:
            return (x-c+b)/2/b
        else:
            return 0.5

@cython.boundscheck(False)
def cy_classify(np.ndarray[np.float_t, ndim=1] data, float c, float b):
    cdef unsigned int i
    cdef np.ndarray[np.float_t, ndim=1] out = np.zeros(data.shape[0], 
                                                       dtype=float)
    for i in xrange(data.shape[0]):
        out[i] = c_classify(data[i], c, b)
    return out

cdef float c_classify(float x, float c, float b):
    """
    Test classification function. Takes test result x, cut-off c and bandwidth
    b.
    """
    if x<c-b:
        return 0
    elif x>c+b:
        return 1
    else:
        if b>10**-7:
            return (x-c+b)/2/b
        else:
            return 0.5
