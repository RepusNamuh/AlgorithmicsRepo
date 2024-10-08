import random, math
from typing import List

def subset(X, A):
    Y = X.copy()
    i = random.randint(0, len(A) - 1)
    j = random.randint(0, len(A) - 1)
    if A[i] in X:
        Y.remove(A[i])
    else:
        Y.append(A[i])
    if A[j] in X and A[j] in Y:
        Y.remove(A[j])
    else:
        Y.append(A[j])
    
    return Y

def Simulated_Annealing(A: List, d: int, error:float, step_size: float):
    X = []
    rand_int = random.randint(0, len(A) - 1)
    for i in range(rand_int):
        rand_index = random.randint(0, len(A) - 1)
        X.append(A[rand_index])

    lower_threshold = d - error
    upper_threshold = d + error
    T = 1.0
    T_min = 0.00001
    smallest_diff = math.inf
    
    while T > T_min and (lower_threshold > sum(X) or sum(X) > upper_threshold):
        for i in range(step_size):
            X_new = subset(X, A)
            diff1 = abs(sum(X_new) - d)
            diff2 = abs(sum(X) - d)
            abs_diff = abs(diff1 - diff2)
            rand_prob = random.random()
            prob = math.exp(-abs_diff / T)

            if diff1 < diff2:
                X = X_new
            elif diff1 > diff2 and prob > rand_prob:
                X = X_new
        smallest_diff = min(smallest_diff, abs_diff)
        T = T * 0.99

    return X, smallest_diff
