#!/usr/bin/env python -tt

from collections import namedtuple

class ImmutQuaternion(namedtuple('ImmutQuaternion', ['s', 'i', 'j', 'k'])):
    """
    This is the quaternion class that generates immutable quaternions that 
    you can multiply in the usual way (e.g. x*y returns a new quaternion using
    the Hamiltonian product).
    """
    def __mul__(self, q):
        if not isinstance(q, ImmutQuaternion):
            raise TypeError ("Product must be of two quaternions!")
        s = self.s*q.s - self.i*q.i - self.j*q.j - self.k*q.k
        i = self.s*q.i + self.i*q.s + self.j*q.k - self.k*q.j
        j = self.s*q.j - self.i*q.k + self.j*q.s + self.k*q.i
        k = self.s*q.k + self.i*q.j - self.j*q.i + self.k*q.s
        return ImmutQuaternion(s, i, j, k)
    def __str__(self):
        return '%f 1  %f i  %f j  %f k' % (self.s, self.i, self.j, self.k)
