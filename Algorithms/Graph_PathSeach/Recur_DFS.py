import networkx as nx
import matplotlib.pyplot as plt

visited = []

def recur_dfs(G, start):
    if start not in visited:
        visited.append(start)
        for next in list(G.adj[start]):
            recur_dfs(G, next )

G = nx.Graph()
G.add_edges_from([('A','B'),('A','C'),('B','D'),('B','E'),('B','F'),('C','G'),('E','H'),('G','I'),('H','I'),('I','J')])
pos = nx.shell_layout(G)
nx.draw_networkx_nodes(G, pos, node_size = 500, node_color = 'pink')
nx.draw_networkx_labels(G, pos)
nx.draw_networkx_edges(G, pos, width=3, edge_color='darkblue')
# plot graph
plt.axis('off')
plt.show()
recur_dfs(G, 'A')
print(visited)
