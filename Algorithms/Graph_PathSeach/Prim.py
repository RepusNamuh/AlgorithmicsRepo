import networkx as nx

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

def Prim_MST(Graph, start, end):
    mst = nx.Graph()
    mst.add_node(start)
    queue.enqueue([start, start], 0)
    while len(mst.nodes) < len(Graph.nodes):
        edge = queue.dequeue()
        previous = edge[0][0]
        current = edge[0][1]
        if current not in mst.nodes:
            mst.add_edge(previous, current, weight = Graph[previous][current]['weight'])
            Graph[previous][current]['colour'] = "r"

        for next in Graph.adj[current]:
            if next not in mst.nodes:
                queue.enqueue([current, next], Graph[current][next]['weight'])
    return mst