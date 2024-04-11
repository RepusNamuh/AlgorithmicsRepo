import networkx as nx
import matplotlib.pyplot as plt
from queue import PriorityQueue
def best_first_search(G, S, target):
    SeenNodes = []
    pq = PriorityQueue()
    pq.put((0, S))
    targetFound = (S == target) # assigns False or True depending on equality
    while (pq.empty() == False) and (not targetFound):
        u = pq.get()[1] # Getting the node with the lowest cost
        SeenNodes.append(u)
        if (u == target):
            targetFound = True
        else:
            print ('BFS: ',u)
        for v in G.neighbors(u):
            if not(v in SeenNodes):
                cost=V_cost[v]
                pq.put((cost, v))
    print(SeenNodes)
G = nx.Graph()
G.add_edges_from([('A','B'),('A','C'),('B','D'),('B','E'),('B','F'),('C','G'),('E','H'),('G','I'),('H','I'),('I','J')])
V_cost = {'A':4,'B':2,'C':3,'D':3,'E':2,'F':1,'G':2,'H':2,'I':1,'J':0} # Dictionary of costs
print(V_cost)
pos = nx.shell_layout(G)
nx.draw_networkx_nodes(G, pos, node_size = 500, node_color = 'pink')
nx.draw_networkx_labels(G, pos)
nx.draw_networkx_edges(G, pos, width=3, edge_color='darkblue')
# plot graph
plt.axis('off')
plt.show()
best_first_search(G,'B','I')