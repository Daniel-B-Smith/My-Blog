import _classify
import numpy as np

def classify(data, c, b):
    data = np.asarray(data)
    return _classify.classify(data, c, b)
