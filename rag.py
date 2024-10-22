import networkx as nx
import matplotlib.pyplot as plt

# Membuat graf kosong
G = nx.DiGraph()

# Menambahkan node (proses dan resource)
processes = ["P1", "P2"]
resources = ["R1", "R2"]

# Menambahkan node proses ke graf
G.add_nodes_from(processes, type="process")

# Menambahkan node resource ke graf
G.add_nodes_from(resources, type="resource")

# Menambahkan edge (panah) untuk hubungan antara proses dan resource
# P1 meminta R1
G.add_edge("P1", "R1", relation="requests")
# R1 dialokasikan ke P2
G.add_edge("R1", "P2", relation="allocated to")
# P2 meminta R2
G.add_edge("P2", "R2", relation="requests")
# R2 dialokasikan ke P1
G.add_edge("R2", "P1", relation="allocated to")

# Visualisasi graf dengan Matplotlib
pos = nx.spring_layout(G)  # Layout untuk tata letak node

# Menggambar node
nx.draw_networkx_nodes(G, pos, node_color="lightblue", node_size=2000)

# Menggambar edge (panah) dengan label
nx.draw_networkx_edges(G, pos, edgelist=G.edges(), arrowstyle="->",