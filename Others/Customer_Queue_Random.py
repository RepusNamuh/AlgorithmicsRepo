import math
import random
import networkx as nx
import matplotlib.pyplot as plt

customers = ["A", "B", "C", "D", "E", "F", "G", "H", "I", "J"]
no_cus = len(customers)
no_que = random.choice(range(2, 6))

queues = ['q1', 'q2', 'q3', 'q4', 'q5']
main_queues = []

def add_to_queues(queue, customers, time):
    if len(main_queues) == 0:
        return main_queues.append((queue, customers, time))   
    for i, (queues, customer, times) in enumerate(main_queues):
          if time < times:
               return main_queues.insert(i, (queue, customers, time))
    return main_queues.append((queue, customers, time))

for i in range(no_que):
    ran_time = random.choice(range(1, 5))
    queue = {}
    j = 0
    max_cus = math.ceil(no_cus/no_que) + 1
    while len(queue) < max_cus and len(customers)!= 0:
          j+=1
          customer = random.choice(customers)
          customers.remove(customer)
          time = j * ran_time
          queue[customer] = time
          add_to_queues(queues[i], customer, time)
    print(queue)
print(main_queues)


