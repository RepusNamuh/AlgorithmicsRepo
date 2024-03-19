import math
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt

def FloydWarshall(G,directed):
  AllNodes = list(G.nodes)
  nV = len(AllNodes)
  dist=np.zeros((nV, nV)) # create a distance matrix for graph
  # initialise to run on graph G={V,E}
  for i in range(0,nV):
    for j in range(0,nV):
      dist[i][j] = math.inf
      
  for i in range(0,nV):
    dist[i][i] = 0 # distance of node to itself is 0
    
  for e in G.edges:
    u = e[0]
    v = e[1]
    w = G[u][v]['weight']
    dist[u-1][v-1] = w
    if (directed == False):
      # for undirected graph ensure symmetry
      dist[v-1][u-1] = w
    
  # build up the array of shortest distance in dist
  for k in range(0,nV):
    for i in range(0,nV):
      for j in range(0,nV):
        # is it cheaper to get to node j from i via k?
        if (dist[i][k] + dist[k][j] < dist[i][j] ):
          dist[i][j] = dist[i][k] + dist[k][j]
    
  return dist