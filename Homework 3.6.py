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

people_Q = Queue()
# Create the graph of relationships.
g = nx.Graph()
g.add_nodes_from(['A', 'B', 'C', 'D', 'E', 'F'])
g.add_edge("A", "B", time=1, colour = "black")
g.add_edge("A", "C", time=2, colour = "black")
g.add_edge("A", "E", time = 1, colour = "black")
g.add_edge("B", "F", time = 4, colour = "black")
g.add_edge("A", "F", time = 10, colour = "black")
g.add_edge("C", "D", time = 3, colour = "black")
g.add_edge("E", "F", time = 2, colour = "black")
g.add_edge("D", "E", time = 9, colour = "black")
g.add_edge("C", "E", time = 3, colour = "black")
# Create a MST of the graph
mst = nx.Graph()
start_node = "A"
mst.add_node(start_node)
people_Q.enqueue(["A" , start_node], 0)
while len(mst.nodes) < len(g.nodes):
    edge = people_Q.dequeue()
    previous = edge[0][0]
    current = edge[0][1]
    if current not in mst.nodes:
        mst.add_edge(previous, current, time = g[previous][current]['time'])
        g[previous][current]['colour'] = "r"

    for next in g.adj[current]:
        if next not in mst.nodes:
            people_Q.enqueue([current, next], g[current][next]['time'])
    

pos = nx.spring_layout(g)
nx.draw(g, pos, with_labels=True)
edge_labels = nx.get_edge_attributes(g, 'time')
edge_colour = nx.get_edge_attributes(g, 'colour')
edge_colour = {k: str(v) for k, v in edge_colour.items()}
nx.draw_networkx_edges(g, pos, edge_color=edge_colour.values())
nx.draw_networkx_edge_labels(g, pos, edge_labels = edge_labels)
plt.show() # Display the graph
