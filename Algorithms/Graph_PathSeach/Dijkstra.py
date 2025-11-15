import math
import networkx as nx
import matplotlib.pyplot as plt

def dijkstra(G,startNode):
  unvisitedNodes = list(G.nodes)
  shortestPath = {} # distance on each node while processing so far
  previousNodes = {} # shortest known path previous node to current node so far
 
  for node in unvisitedNodes:
    shortestPath[node] = math.inf # initialise all nodes distance to infinity
    shortestPath[startNode] = 0 # starting node's distance initialise with 0
    
  while unvisitedNodes:
    # find the node with the lowest distance so far
    currentMinNode = None
    for node in unvisitedNodes: # Iterate over the nodes
      if currentMinNode == None:
        currentMinNode = node
      elif shortestPath[node] < shortestPath[currentMinNode]:
        currentMinNode = node
                
    for V in G.neighbors(currentMinNode):
      tentativeValue = shortestPath[currentMinNode] + G[currentMinNode][V]['weight']
      if tentativeValue < shortestPath[V]:
        shortestPath[V] = tentativeValue # relax the edges
        previousNodes[V] = currentMinNode # save path
 
    unvisitedNodes.remove(currentMinNode) # remove the processed node from unvisited
    
  return previousNodes, shortestPath