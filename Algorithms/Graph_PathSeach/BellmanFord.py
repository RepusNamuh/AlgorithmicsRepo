import math
import networkx as nx
import matplotlib.pyplot as plt

def BellmanFord(G,startNode):
    unvisitedNodes = list(G.nodes)
    shortestPath = {} # distance on each node while processing so far
    previousNodes = {} # shortest known path previous node to current node so far

    for node in unvisitedNodes:
        shortestPath[node] = math.inf # initialise all nodes distance to infinity
        shortestPath[startNode] = 0 # starting node's distance initialise with 0
    
    for i in range(len(list(G.nodes))-1):
        for u,v in list(G.edges):
            if shortestPath[u] + G[u][v]['weight'] < shortestPath[v]:
                shortestPath[v] = shortestPath[u] + G[u][v]['weight']
                previousNodes[v] = u
    
    for u,v in list(G.edges):
        if shortestPath[u] + G[u][v]['weight'] < shortestPath[v]:
            return "Negative cycle detected"
    return (shortestPath, previousNodes)

