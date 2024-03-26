import networkx as nx
import matplotlib.pyplot as plt
def topologicalSort(G):
# assumption: at least one source node in G
    if (G.number_of_nodes() > 0):
        SourceNodes = []
        for N in list(G.nodes()):
            if G.in_degree(N) == 0:
                print('Source Node ', N)
                SourceNodes.append(N)
                TopoList.append(SourceNodes)
        for S in SourceNodes:
            G.remove_node(S)
        topologicalSort(G) # recur on graph
    else:
        print('Base Case Stop Recursion')
# directed graph nodes and edges (from,to)
DG=nx.DiGraph()
DG.add_edges_from([(5,0),(5,2),(4,0),(4,1),(2,3),(3,1)])
pos = nx.circular_layout(DG)
# nodes
nx.draw_networkx_nodes(DG, pos, node_size=700, node_color='lightgrey')
nx.draw_networkx_labels(DG,pos,font_color = 'blue', font_size = 14)
# edges
nx.draw_networkx_edges(DG, pos, width=6, edge_color ='red')
plt.axis("off")
plt.show()
# Topological Sort
TopoList=[]
topologicalSort(DG)
print(TopoList)