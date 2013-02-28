

def fib(int n):
    cdef int a, b
    a, b = 0, 1
    while b < n:
        a, b = b, a + b
    return b

def fib2(n):
    a, b = 0, 1
    while b < n:
        a, b = b, a + b
    return b

def dumb(int n):
    return dumb_fib(n)

cdef int dumb_fib(int n):
    if n==1:
        return 1
    elif n==2:
        return 1
    else:
        return dumb_fib(n-1) + dumb_fib(n-2)