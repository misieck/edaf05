import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

class Edge {
	Node u;
	Node v;
	int flow = 0;
	int capacity;

	public Edge(Node u, Node v, int c) {
		this.u = u;
		this.v = v;
		this.capacity = c;
	}
}

class Node {
	int height = 0;
	int excess = 0;
	int id;
	List<Edge> edges;

	Node(int id) {
		edges = new ArrayList<>();
		this.id = id;
	}

	Node other(Edge e) {
		if (this == e.u)
			return e.v;
		else if (this == e.v)
			return e.u;
		else
			throw new RuntimeException("node not in edge");
	}

	@Override
	public String toString() {
		return "node " + id;
	}
}

class Graph {
	ArrayList<Node> nodes;
	ArrayList<Edge> edges;
	ArrayList<Node> candidateSet;// candidates for push i.e. nodes with excess

	public Graph(int n, int m) {
		nodes = new ArrayList<>(n);
		edges = new ArrayList<>(m);
		candidateSet = new ArrayList<>(10);
		for (int i = 0; i < n; i++) {
			nodes.add(null);
		}
	}

	void readEdge(Scanner scanner) {
		int u = scanner.nextInt();
		int v = scanner.nextInt();
		int c = scanner.nextInt();

		Node U = nodes.get(u);
		Node V = nodes.get(v);

		if (U == null) {
			U = new Node(u);
			nodes.set(u, U);
		}

		if (V == null) {
			V = new Node(v);
			nodes.set(v, V);
		}

		Edge newEdge = new Edge(U, V, c);
		edges.add(newEdge);
		U.edges.add(newEdge);
		V.edges.add(newEdge);
	}

	boolean isFinished() {
		return nodes.get(0).excess == -nodes.get(nodes.size() - 1).excess;
	}

	boolean isLastNode(Node u) {
		return u == nodes.get(nodes.size() - 1);
	}

	boolean isStartNode(Node u) {
		return u == nodes.get(0);
	}

	void reset() {
		for (Node n : nodes) {
			n.height = 0;
			n.excess = 0;

		}

		for (Edge e : edges) {
			e.flow = 0;
		}

	}

	void removeEdges(int from, int to, ArrayList<Integer> array) {
		for (int i = from; i <= to; i++) {
			removeEdge(array.get(i));
		}
	}

	void addEdges(int from, int to, ArrayList<Integer> array) {
		for (int i = from; i <= to; i++) {
			addEdge(array.get(i));
		}
	}

	void removeEdge(int idx) {
		Edge e = edges.get(idx);
		e.u.edges.remove(e);
		e.v.edges.remove(e);
		reset();
	}

	void addEdge(int idx) {
		Edge e = edges.get(idx);
		e.u.edges.add(e);
		e.v.edges.add(e);
		reset();
	}

}

class prepu {
	public static void main(String[] args) {

		Scanner scan = new Scanner(System.in);
		int N = scan.nextInt(); // number of nodes
		int M = scan.nextInt(); // number of edges
		// System.out.println("Graph of " + N + " nodes and " + M + " edges!");
		int C = scan.nextInt(); // minimal capacity requirement
		int P = scan.nextInt(); // number of edge candidates to remove
		Graph G = new Graph(N, M);
		for (int i = 0; i < M; ++i) {
			G.readEdge(scan);
		}
		assert G.edges.size() == M; // Verify number of edges
		assert G.nodes.size() == N; // Verify number of nodes

		ArrayList<Integer> removeOrderList = new ArrayList<>(P);
		for (int i = 0; i < P; ++i) {
			removeOrderList.add(scan.nextInt());
		}
		int flow;

		int res = binarySearch(G, C, removeOrderList);
		G.reset();
		flow = preflow(G);
		System.out.println(res + " " + flow);
		/*
		 * 
		 * 
		 * for (Integer toRemove: removeOrderList){ G.removeEdge(toRemove); flow =
		 * preflow(G); if (flow >= C) { removedCount ++;
		 * //System.out.println(removedCount + " " + flow); } else {
		 * //System.out.println( "ADDING BACK " + toRemove); G.addEdge(toRemove); break;
		 * }
		 * 
		 * }
		 */
	}

	static int binarySearch(Graph g, int C, ArrayList<Integer> toRemove) {
		int flow = 0;
		int from = 0;
		int size = toRemove.size();
		int to = size - 1;
		int mid = size / 2;
		int oldmid = mid;

		g.removeEdges(from, mid, toRemove);
		while (from <= to) {
			flow = preflow(g);
			if (flow >= C) {
				from = mid + 1;
				oldmid = mid;
				mid = (from + to) / 2;
				g.removeEdges(from, mid, toRemove);
			} else {
				to = mid - 1;
				// g.addEdges(from, mid);
				mid = (from + to) / 2;
				g.addEdges(mid + 1, to + 1, toRemove);
			}

		}

		return oldmid + 1;
	}

	private static int preflow(Graph G) {
		Node S = G.nodes.get(0);
		S.height = G.nodes.size();
		S.edges.forEach((e) -> {
			S.excess += e.capacity;
			push(G, S, S.other(e), e);
		});
		S.edges.forEach((e) -> {
			S.excess -= e.capacity;
		});

		var list = G.candidateSet;
		while (!list.isEmpty()) {
			Node u = pop(list);
			for (Edge e : u.edges) {
				Node v = u.other(e);
				int cmp = 0;
				if (u == e.u) {// positive directions
					cmp = e.flow;
				} else {
					cmp = -e.flow;
				}

				if (u.height > v.height && cmp < e.capacity) {
					push(G, u, v, e);
					if (G.isFinished())
						return -G.nodes.get(0).excess;
				}
			}
			if (u.excess > 0) {
				relabel(G, u);
			}
		}
		return -G.nodes.get(0).excess;
	}

	private static Node pop(ArrayList<Node> list) {
		Node u = list.get(list.size() - 1);
		list.remove(list.size() - 1);
		return u;
	}

	private static void relabel(Graph G, Node u) {
		u.height++;
		G.candidateSet.add(u);
	}

	private static void push(Graph G, Node u, Node v, Edge e) {
		assert u.height > v.height;
		assert u.excess > 0;
		int delta;
		if (e.u == u) {
			delta = Math.min(u.excess, e.capacity - e.flow);
			e.flow += delta;
		} else {
			delta = Math.min(u.excess, e.capacity + e.flow);
			e.flow -= delta;
		}

		u.excess -= delta;
		v.excess += delta;

		assert delta >= 0;
		assert u.excess >= 0;
		assert Math.abs(e.flow) <= e.capacity;
		if (u.excess > 0 && !G.isLastNode(u)) {
			G.candidateSet.add(u); // If u is in set it is not added
		}
		if (v.excess > 0 && v.excess == delta && !G.isLastNode(v)) {
			G.candidateSet.add(v); // If v is in set it is not added
		}

	}
}