#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


int Fbo(int n) {
	vector<int>f = {0, 1, 1};

	for (int i = 2;i < n;i++) {
		f.push_back(f[i - 1] + f[i]);
	}

	return f[n];
}

void Ccoin(int money) {
    cout << "돈: " << money << endl;
    int c500 = money /500;
    money %= 500;
    int c100 = money / 100;
    money %= 100;
    int c50 = money / 50;
    money %= 50;
    int c10 = money / 10;
    money %= 10;
    int c5 = money / 5;
    money %= 5;
    int c1 = 0;

    cout << "500원: " << c500 << "　100원: " << c100 << "　50원: " << c50 << "　10원: " << c10 << "　5원: " << c5 << "　1원: " << c1 << endl;

}


struct WeightedGraph {
    int n;  // 정점 개수
    vector<vector<pair<int, int>>> adj; // 인접 리스트 정의

    // 생성자: 정점 n개짜리 그래프 초기화
    explicit WeightedGraph(int n) : n(n), adj(n) {}

    // 간선 추가 함수
    void addEdge(int u, int v, int w, bool undirected = true) {
        adj[u].push_back({ v, w });  // u -> v (가중치 w)
        if (undirected) {
            adj[v].push_back({ u, w }); // v -> u (가중치 w), 무방향일 때만, 반대쪽 추가
        }
    }
    
    // 그래프 출력
    void print() const {
        for (int u = 0; u < n; ++u) {
            cout << u << " : ";
            for (auto [v, w] : adj[u]) {
                cout << "(" << v << "," << w << ") ";
            }
            cout << "\n";
        }
    }
};

struct UnionFind {
    vector<int> parent, rank;
    UnionFind(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (rank[a] < rank[b]) swap(a, b);
        parent[b] = a;
        if (rank[a] == rank[b]) rank[a]++;
        return true;
    }
    
};

void KruskalAl(WeightedGraph gr) {
    // WeigtedGrapgh의 adj의 가중치를 기준으로 정렬(뒤가 더 커지게)
    struct Edge { int u, v, w; };
    vector<Edge> edges;

    for (int u = 0; u < gr.n; u++) {
        for (auto [v, w] : gr.adj[u]) {
            if (u < v) edges.push_back({ u,v,w });
        }
    }

    sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.w < b.w;
        });
    
    UnionFind uf(gr.n);

    int mst_weight = 0;
    vector<Edge> mst;

    for (auto& e : edges) {
        if (uf.unite(e.u, e.v)) {
            mst.push_back(e);
            mst_weight += e.w;
        }
    }

    cout << "Kruskal MST edges:\n";
    for (auto& e : mst) {
        cout << e.u << " - " << e.v << " (w=" << e.w << ")\n";
    }
    cout << "Total Weight = " << mst_weight << "\n";
}





int main() {
	cout << "F(2) : " << Fbo(2) << endl;
	cout << "F(3) : " << Fbo(3) << endl;
	cout << "F(5) : " << Fbo(5) << endl;
	cout << "F(7) : " << Fbo(7) << endl;

    //////////////////
    Ccoin(780);
    Ccoin(530);


    WeightedGraph g(5); // 정점 5개 (0~4)

    // 간선 추가 (u, v, weight)
    g.addEdge(0, 1, 10);
    g.addEdge(0, 2, 5);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 3, 7);
    g.addEdge(3, 4, 1);

    g.print();

    KruskalAl(g);

    vector<int> v = { 5, 3, 8, 3, 9, 1, 5, 7, 8, 2 };

}