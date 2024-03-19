import networkx as nx
g = nx.Graph()

def DFS(G, start, end):
  stack = list()
  nodes_visited = []
  stack.append(start)
  while ((start != end) and len(stack) != 0):
    start = stack.pop()
    nodes_visited.append(start)
    temp_list = []
    for node in list(G.adj[start]):
      if not(node in nodes_visited):
        stack.append(node)