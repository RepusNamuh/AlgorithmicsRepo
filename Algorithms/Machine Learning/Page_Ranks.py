import networkx as nx
from networkx import NetworkXError
import numpy as np
import matplotlib.pyplot as plt

def pageranks(G, alpha=0.85,max_iter=100, tol=1.0e-6, weight='weight'): # adapted & /simplified from what is implemented in networkX
    if len(G) == 0: # no nodes
        return {}
    if not G.is_directed(): # convert to directed graph
        D = G.to_directed()
    else:
        D = G
    # Create a copy in (right) stochastic form which for each node, 
    # the sum of the weights of all the out-edges of that node is 1. 
    W = nx.stochastic_graph(D, weight=weight)
    N = W.number_of_nodes()
    # Set fixed starting vector 
    x = dict.fromkeys(W, 1.0 / N)
    # Assign uniform vector
    p = dict.fromkeys(W, 1.0 / N)
    # Use uniform vector 
    dangling_weights = p
    dangling_nodes = [n for n in W if W.out_degree(n, weight=weight) == 0.0]
    
    # power iteration: make up to max_iter iterations
    for _ in range(max_iter):
        xlast = x
        x = dict.fromkeys(xlast.keys(), 0)
        danglesum = alpha * sum(xlast[n] for n in dangling_nodes)
        for n in x:
            # this matrix multiply looks odd because it is
            # doing a left multiply x^T=xlast^T*W
            for nbr in W[n]:
                x[nbr] += alpha * xlast[n] * W[n][nbr][weight]
            x[n] += danglesum * dangling_weights[n] + (1.0 - alpha) * p[n]

        # check convergence, l1 norm
        err = sum([abs(x[n] - xlast[n]) for n in x])
        if err < N*tol:
            return x
    raise NetworkXError('pagerank: power iteration failed to converge in %d iterations.' % max_iter)

G = nx.DiGraph()
G.add_edges_from([('A','B'),('B','C'),('C','D'),('A','D'),('A','C')])
# draw layout
pos = nx.spring_layout(G)
# draw nodes
nx.draw_networkx_nodes(G, pos, node_size=550, node_color = 'yellow')
nx.draw_networkx_labels(G,pos)
# draw edges
nx.draw_networkx_edges(G, pos, width=2, edge_color ='darkblue')
# plot
plt.axis('off')
plt.show()
pr = pageranks(G)
print(G,pr)