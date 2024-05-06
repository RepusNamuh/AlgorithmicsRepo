import networkx as nx
import matplotlib.pyplot as plt

# Creating a Queue
class Queue:
    def __init__(self):
        self.queue = []
    
    def enqueue(self, item, rank):
        for i in range(len(self.queue)):
                if rank < self.queue[i][1]:
                    return self.queue.insert(i, (item, rank))
        return self.queue.append((item, rank))
    def dequeue(self):
        if len(self.queue) == 0:
            return None
        return self.queue.pop(0)
    
    def is_empty(self):
        return len(self.queue) == 0

queue = Queue()

def Prim_MST_Split(Graph, edge):
    mst = nx.Graph()
    v1 = edge[0]
    v2 = edge[1]
    special_node = "Special"
    Graph.add_node(special_node)
    Graph.add_edge(special_node, v1, time = 0)
    Graph.add_edge(special_node, v2, time = 0)
    mst.add_node(special_node)
    queue.enqueue([special_node, special_node], 0)
    while len(mst.nodes) < len(Graph.nodes):
        edge = queue.dequeue()
        previous = edge[0][0]
        current = edge[0][1]
        if current not in mst.nodes:
            mst.add_edge(previous, current, weight = Graph[previous][current]['time'])
            Graph[previous][current]['colour'] = "r"

        for next in Graph.adj[current]:
            if next not in mst.nodes:
                queue.enqueue([current, next], Graph[current][next]['time'])
    mst.remove_node(special_node)
    return mst

def return_sub_graph(mst):
    """Getting the split mst into two sub-graphs"""
    # Use connected_components to find the two subgraphs
    components = list(nx.connected_components(mst))
    
    # Create subgraphs from the components
    sub_mst1 = mst.subgraph(components[0])
    sub_mst2 = mst.subgraph(components[1])
    return sub_mst1, sub_mst2