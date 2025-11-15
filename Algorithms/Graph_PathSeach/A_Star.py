import math
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
    

def heuristic(node):
    pass

def A_Star(G, source, target):
    visited = []
    q = Queue()
    g_score = {}
    h_score = {}
    f_score = {}
    previous = {}
    for node in list(G.nodes):
        g_score[node] = math.inf
        h_score[node] = heuristic(node)
        f_score[node] = math.inf
        previous[node] = None
        q.enqueue(node, f_score[node])
    
    g_score[source] = 0
    h_score[source] = heuristic[source]
    f_score[source] = g_score[source] + h_score[source]
    finished = False
    weight = nx.get_edge_attributes(G, "weight")

    while (not finished) and len(q) != 0:
        current = q.dequeue()
        if(current == target):
            finished = True
            visited.append(current)
        else:
            for neighbour in list(G.adj[current]):
                if neighbour not in visited:
                    tentative_g_score = weight[(current, neighbour)] + g_score[current]
                    if tentative_g_score < g_score[neighbour]:
                        g_score[neighbour] = tentative_g_score
                        h_score[neighbour] = heuristic(neighbour)
                        f_score[neighbour] = g_score[neighbour] + h_score[neighbour]
                        previous[neighbour] = current
            visited.append(current)

    return visited