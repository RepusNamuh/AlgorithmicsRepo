import networkx as nx
import matplotlib.pyplot as plt

colours = ["red", "green", "yellow", "blue"]
def node_colour(Graph, node, colour):
  colours = nx.get_node_attributes(Graph, "colour")
  for neighbour in list(Graph.adj[node]):
    if colours[neighbour] == colour and colours[neighbour] != "white":
      return False
  return True
    
def BTS(Graph, nodes, colours):
  if len(nodes) == 0:
    return list(Graph.nodes(data=True))
  
  for node in nodes:
    nodes.remove(node)
    for c in colours:
      if node_colour(Graph, node, c):
        Graph.nodes[node]["colour"] = c
        solution = BTS(Graph, nodes, colours)
        if solution != None:
          return solution
        Graph.nodes[node]["colour"] = c

def draw_graph(graph):
  pos = nx.spring_layout(graph)
  nx.draw(graph, pos, with_labels=True)
  node_colour = nx.get_node_attributes(g, 'colour')
  node_colour = {k: str(v) for k, v in node_colour.items()}
  nx.draw_networkx_nodes(g, pos, node_color=node_colour.values())
  plt.show()

g = nx.Graph()
g.add_nodes_from(["1", "2", "3", "4", "5"], colour = "white")
g.add_edges_from([("1", "2"), 
                  ("1", "3"), 
                  ("1", "4"), 
                  ("1", "5"), 
                  ("2", "3"), 
                  ("2", "5"), 
                  ("2", "4"),
                  ("3", "4"),
                  ("4", "5")])

print(BTS(g, list(g.nodes), colours))
draw_graph(g)