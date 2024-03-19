import networkx as nx
class Queue:
    def __init__(self):
        self.queue = []
    
    def enqueue(self, item):
        self.queue.append(item)
    
    def dequeue(self):
        if len(self.queue) == 0:
            return None
        return self.queue.pop(0)
    
    def is_empty(self):
        return len(self.queue) == 0

g = nx.Graph()
queue = Queue()

def BFS(graph, source):
    queue.enqueue(source)
    visited = []
    while not queue.is_empty():
        current_node = queue.dequeue()
        if current_node not in visited:                
            visited.append(current_node)
            for neighbour in list(g.adj[current_node]):
                if neighbour not in visited:
                    queue.enqueue(neighbour)
