import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;


class Edge {
    Node u;
    Node v;
    int flow;
    int capacity;

    public Edge(Node u, Node v, int c) {
        this.u = u;
        this.v = v;
        this.capacity = c;
    }
}

class Node {
    int height;
    int excess;
    int id;
    List<Edge> edges;
    Node(int id) {
        edges = new ArrayList<>();
        this.id = id;
    }

    Node other (Edge e) {
        if (this == e.u) return e.v;
        else if (this == e.v) return e.u;
        else throw new RuntimeException("node not in edge");
    }

    @Override
    public String toString() {
        return "node "+id;
    }
}

class Graph {
    ArrayList<Node> nodes;
    ArrayList<Edge> edges;

    public Graph(int n, int m) {
        nodes = new ArrayList<>(n);
        edges = new ArrayList<>(m);
        for (int i = 0; i< n; i++) {
            nodes.add(null);
        }
    }


    void readEdge( Scanner scanner) {
        int u = scanner.nextInt();
        int v = scanner.nextInt();
        int c = scanner.nextInt();

        Node U = nodes.get(u);
        Node V = nodes.get(v);

        if ( U == null) {
            U = new Node(u);
            nodes.set(u, U);
        }

        if ( V == null) {
            V = new Node(v);
            nodes.set(v, V);
        }

        Edge newEdge = new Edge(U,V,c);
        edges.add(newEdge);
        U.edges.add(newEdge);
        V.edges.add(newEdge);
    }

}

class prepu{
    public static void main(String[] args) {

        Scanner scan = new Scanner(System.in);
        int N = scan.nextInt(); //number of nodes
        int M = scan.nextInt(); //number of edges
        System.out.println("Graph of " + N + " nodes and " + M + " edges!");
        int C = scan.nextInt(); //minimal capacity requirement
        int P = scan.nextInt(); //number of edge candidates to remove
        Graph G = new Graph(N,M);
        for (int i = 0; i < M; ++i) {
            G.readEdge(scan);
        }

        System.out.println("Graph of " + N + " nodes and " + M + " edges!");

    }
}