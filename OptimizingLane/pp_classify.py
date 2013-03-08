def classify(x, c, b):
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
