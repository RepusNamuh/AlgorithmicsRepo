import networkx as nx 
import matplotlib.pyplot as plt

G = nx.Graph() # Create a graph
nodes = [1, 2, 3, 4, 5, 6]
G.add_nodes_from(nodes)
G.add_edges_from([(1, 2), (1, 5), (5, 4), (5, 2), (2, 3), (3, 4), (4, 6)])

nx.draw(G, with_labels = True)

plt.show() # Display the graph