import json
import networkx as nx
import matplotlib
matplotlib.use('Agg') 
import matplotlib.pyplot as plt

# Mapeamento das strings do C++ para cores válidas no Matplotlib
COLOR_MAP = {
    "BRANCO": "#FFFFFF",
    "CINZA": "#A0A0A0",
    "PRETO": "#2B2B2B",
    "WHITE": "#FFFFFF",
    "GREY": "#A0A0A0",
    "BLACK": "#2B2B2B",
}


def format_number(value):
    """Shows integer weights/distances without the .0 suffix."""
    if value is None:
        return "inf"
    if float(value).is_integer():
        return int(value)
    return value


def plot_graph_from_json(json_path: str):

    ### ------- Prepares the gragph
    with open(json_path, "r", encoding="utf-8") as f:
        data = json.load(f)

    # Creates the graph for py
    is_directed = data.get("is_directed", False)
    G = nx.DiGraph() if is_directed else nx.Graph()

    # Adds the nodes
    node_data_map = {}
    for node_info in data["nodes_print_data"]:
        u = node_info["id"]
        node_data_map[u] = node_info
        G.add_node(u)

    # Adds adjacencies (with weights, when the JSON comes from a weighted graph)
    weights = data.get("weights")
    for u, neighbors in enumerate(data["edges"]):
        for i, v in enumerate(neighbors):
            G.add_edge(u, v, weight=weights[u][i] if weights else None)

    # Edges that belong to some shortest path
    path_edges = set()
    for sp in data.get("shortest_paths", []):
        path = sp["path"]
        for a, b in zip(path, path[1:]):
            path_edges.add((a, b))

    ### ------ Configures plot
    pos = nx.spring_layout(G, k=1.3, iterations=80, seed=42)

    # Extracts labels and colors
    node_colors = []
    labels = {}

    for node in G.nodes():
        info = node_data_map.get(
            node, {"colour": "CINZA", "d": 0, "f": 0, "pi": -1}
        )
        color_name = info.get("colour", "CINZA").upper()
        node_colors.append(COLOR_MAP.get(color_name, "#D3D3D3"))

        # label showing d, f, pi or only the node number
        labels[node] = (
            #f"v{node}\n({info['d']}/{info['f']})\nπ:{info['pi']}"
            f"{node}"
        )

    plt.figure(figsize=(9, 7))

    # Draws the nodes
    nx.draw_networkx_nodes(
        G,
        pos,
        node_color=node_colors,
        node_size=1500,
        edgecolors="black",  # Borda visível para nós brancos
        linewidths=2,
    )

    # Draws the edges, highlighting the shortest paths in red
    def is_path_edge(edge):
        return edge in path_edges or (edge[1], edge[0]) in path_edges

    regular_edges = [e for e in G.edges() if not is_path_edge(e)]
    highlight_edges = [e for e in G.edges() if is_path_edge(e)]

    nx.draw_networkx_edges(
        G,
        pos,
        edgelist=regular_edges,
        arrowstyle="->",
        arrowsize=20 if is_directed else 0,
        edge_color="black",
        width=1.5,
    )
    if highlight_edges:
        nx.draw_networkx_edges(
            G,
            pos,
            edgelist=highlight_edges,
            arrowstyle="->",
            arrowsize=20 if is_directed else 0,
            edge_color="#D62728",
            width=3,
        )

    # Draws the weights on the edges
    if weights:
        edge_labels = {
            (u, v): format_number(G[u][v]["weight"])
            for u, v in G.edges()
            if G[u][v]["weight"] is not None
        }
        nx.draw_networkx_edge_labels(
            G, pos, edge_labels=edge_labels, font_size=8, font_color="#1F4E79"
        )

    # Draws the labels
    for node, (x, y) in pos.items():
        info = node_data_map.get(node, {})
        text_color = (
            "white" if info.get("colour", "").upper() == "PRETO" else "black"
        )
        plt.text(
            x,
            y,
            labels[node],
            fontsize=9,
            ha="center",
            va="center",
            color=text_color,
            fontweight="bold",
        )

    plt.title(
        f"Visualização do Grafo ({'Direcionado' if is_directed else 'Não-Direcionado'})",
        fontsize=14,
    )

    # Shortest paths summary (dijkstra)
    shortest_paths = data.get("shortest_paths", [])
    if shortest_paths:
        lines = [
            f"{sp['src']} -> {sp['dest']}: custo {format_number(sp['dist'])} | "
            + " -> ".join(str(v) for v in sp["path"])
            for sp in shortest_paths
        ]
        plt.figtext(0.02, 0.02, "\n".join(lines), fontsize=9, family="monospace")

    # saves the plot in an image
    plt.axis("off")
    plt.tight_layout()
    plt.savefig("docs/grafo.png", dpi=300, bbox_inches='tight')
    plt.close()


if __name__ == "__main__":
    plot_graph_from_json("bin/graph_data.json")