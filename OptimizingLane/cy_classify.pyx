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
@cython.wraparound(False)
def cy_classify(double[::1] data, float c, float b):
    cdef unsigned int i
    cdef np.ndarray[np.float_t, ndim=1] out = np.zeros(data.shape[0], 
                                                       dtype=float)
    for i in xrange(data.shape[0]):
        out[i] = c_classify(data[i], c, b)
    return out

@cython.boundscheck(False)
@cython.wraparound(False)
def cyo_classify(np.ndarray[np.float_t, ndim=1] data, float c, float b):
    cdef unsigned int i
    cdef np.ndarray[np.float_t, ndim=1] out = np.zeros(data.shape[0], 
                                                       dtype=float)
    for i in xrange(data.shape[0]):
        out[i] = o_classify(data[i], c, b)
    return out

@cython.boundscheck(False)
@cython.wraparound(False)
def unr_classify(np.ndarray[np.float_t, ndim=1] data, float c, float b):
    cdef unsigned int i
    cdef np.ndarray[np.float_t, ndim=1] out = np.zeros(data.shape[0], 
                                                       dtype=float)
    for i in xrange(0, data.shape[0], 8):
        out[i] = o_classify(data[i], c, b)
        out[i+1] = o_classify(data[i], c, b)
        out[i+2] = o_classify(data[i], c, b)
        out[i+3] = o_classify(data[i], c, b)
        out[i+4] = o_classify(data[i], c, b)
        out[i+5] = o_classify(data[i], c, b)
        out[i+6] = o_classify(data[i], c, b)
        out[i+7] = o_classify(data[i], c, b)
    for j in xrange(i+1, data.shape[0]):
        out[i] = o_classify(data[i], c, b)
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

cdef float o_classify(float x, float m, float w):
     cdef float out = (x-m)*w
     out = 0 if (out < 0) else out
     out = 1 if (out > 1) else out
     return out
