from collections import defaultdict

def DPminOP(n, operations, values, A):
  A[1] = 1
  for i in range(2, n + 1):
    A[i] = i
    j = 0
    for opt in  operations:
        value = values[j]
        if opt == "/" and ((i % value) == 0):
            if i / value == 1:
                A[i] = 1
            if A[i / value] + 1 < A[i]:
                A[i] = A[i / value] + 1
        elif opt == "-" and (i - value) >= 1:
            if i - value == 1:
                A[i] = 1
            elif A[i - value] + 1 < A[i]:
                A[i] = A[i - value] + 1
        j += 1
      
  return A[n]
n = 15
operations = ["-", "/", "/", "-"]
values = [1, 2, 3, 5]
A = defaultdict(int)

print(DPminOP(n, operations, values, A))
        