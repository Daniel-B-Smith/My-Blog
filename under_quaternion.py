#!/usr/bin/env python -tt

class Quaternion(object):
    def __init__(self, s, i, j, k):
        self.s = s
        self.i = i
        self.j = j
        self.k = k
        self._index = ('s', 'i', 'j', 'k')
    def __getitem__(self, n):
        try:
            if not isinstance(n, slice):
                return self.__dict__[self._index[n]]
            else:
                new_quat = Quaternion(None, None, None, None)
                step = 1 if n.step is None else n.step
                for m in xrange(n.start, n.stop, step):
                    new_quat.__dict__[self._index[m]] = \
                        self.__dict__[self._index[m]]
                return new_quat
        except IndexError:
            raise IndexError ('Quaternion object only has four items')

    def __setitem__(self, n, x):
        if not isinstance(n, slice):
            try:
                self.__dict__[self._index[n]] = x
            except IndexError:
                raise IndexError ('Quaternion object only has four items')
        else:
            step = 1 if n.step is None else n.step
            for m in xrange(n.start, n.stop, step):
                try:
                    self.__dict__[self._index[m]] = x[m]
                except IndexError:
                    pass
    def __str__(self):
        return '%f 1  %f i  %f j  %f k' % (self.s, self.i, self.j, self.k)
