import math
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt

g = nx.Graph()

node_visited = []
success_visit = []

# Closer coordinate mean closer to destination
# Though in real life, this will not always work.
def coordinate_compare(current, next, goal):
    x1, y1 = pos[current]
    x2, y2 = pos[next]
    x3, y3 = pos[goal]
    distance1 = math.sqrt((x1 - x3)**2 + (y1 - y3)**2)
    distance2 = math.sqrt((x2 - x3)**2 + (y2 - y3)**2)
    if distance1 > distance2:
        return current
    else:
        return next

# Find the next node to visit closer to the goal
def closest_node(current, goal, next = None):
    next = current
    if len(list(g.adj[current])) == 0: # Because of the graph construction
        return next, current
    for node in list(g.adj[current]):
        if node in node_visited:
            continue
        if node == goal:
            next = node
            break
        if next == current: #Should not compare coordinate of current and node
            next = node
            continue
        next = coordinate_compare(next, node, goal)
           
    return next, current

# Best First Search
def BestFirstSearch(start, goal):
    current = start
    node_visited.append(current)
    while current != goal:
        current, previous = closest_node(current, goal) 
        if current != previous:
            node_visited.append(current)
            success_visit.append(previous)
        else:
            if success_visit == []:
                print("No path found")
                break
            else:
                current = success_visit.pop()
                continue