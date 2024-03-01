import networkx as nx 
import matplotlib.pyplot as plt

G = nx.Graph() # Create a graph


#nodes and edges
G.add_nodes_from(['a', 'b', 'c', 'd', 'e', 'f'])
G.add_edge('a', 'b', weight = 2)
G.add_edge('a', 'e', weight = 3)
G.add_edge('e', 'd', weight = 8)
G.add_edge('b', 'c', weight = 7)
G.add_edge('d', 'c', weight = 9)
G.add_edge('c', 'f', weight = 4)
G.add_edge('d', 'f', weight = 4)
G.add_edge('b', 'f', weight = 1)

# Define node colors
# I need to research online for this generator
node_colors = ['b' if n in ['e', 'd'] else 'r' if n in ['b', 'c'] else 'gray' for n in G.nodes()]

# Define edge colors
edge_colors = ['blue' if e in [('e', 'd'), ('d', 'e')] else 'red' if e in [('b', 'c'), ('c', 'b')] else 'black' for e in G.edges()]

pos = nx.spring_layout(G)

nx.draw_networkx_edges(G, pos, edge_color=edge_colors)
nx.draw_networkx_labels(G, pos)
nx.draw_networkx_nodes(G, pos, node_color='white', edgecolors=node_colors, linewidths=2)

# Draw edges with weight
labels = nx.get_edge_attributes(G, 'weight')
nx.draw_networkx_edge_labels(G, pos, edge_labels = labels)

plt.show() # Display the graph

def DFS(G, A, B):
  stack = list()
  nodes_visited = []
  stack.append(A)
  while ((A != B) and len(stack) != 0):
    A = stack.pop()
    nodes_visited.append(A)
    temp_list = []
    for node in list(G.adj[A]):
      if not(node in nodes_visited):
        temp_list.append(node)
    for item in temp_list:
      stack.append(temp_list.pop())
    
  print(nodes_visited)
  
print(DFS(G, 'a', 'f'))