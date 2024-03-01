import networkx as nx
import matplotlib.pyplot as plt

# Creating a Queue
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

people_Q = Queue()
def add_edge(graph, node1, node2):
    weights = 0
    for attr1, attr2 in zip(graph.nodes[node1].values(), graph.nodes[node2].values()):
        if attr1 == attr2:
            weights += 1
            
    return graph.add_edge(node1, node2, weight=weights)
    
# Breath First Search
def BFS(graph, source):
    people_Q.enqueue(source)
    visited = []
    while not people_Q.is_empty():
        current = people_Q.dequeue()
        if current not in visited:
            if len(visited) > 0:
                for previous in visited:
                    add_edge(graph, previous, current)                     
            visited.append(current)
            non_neighbours = list(nx.non_neighbors(g, current))
            for neighbour in list(graph.adj[current]) + non_neighbours:
                if neighbour not in visited:
                    people_Q.enqueue(neighbour)

#Start to create a simple graph
g = nx.Graph()

def add_to_dict(subject, colour):
    return {"Subject": subject, "Colour": colour}

g.add_nodes_from(['A', 'B', 'C'])
# Add attributes to the nodes
nx.set_node_attributes(g, {"A" : add_to_dict("Maths", "Red"), 
                           "B" : add_to_dict("Science", "Blue"), 
                           "C" : add_to_dict("Maths", "green")})

source_node = "B"

# Creating edges and adding weight to them
BFS(g, source_node)
pos = nx.shell_layout(g)

nx.draw(g, pos, with_labels = True)

# Draw edges with weight
labels = nx.get_edge_attributes(g, 'weight')
nx.draw_networkx_edge_labels(g, pos, edge_labels = labels)

plt.show() # Display the graph

