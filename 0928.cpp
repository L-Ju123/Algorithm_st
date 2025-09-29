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




struct Item {
    string name;
    string job; // a,b,c 중 하나
    int attack;
    int level;
    int timestamp;
};


bool compareItem(const Item& a, const Item& b) {
    //레벨 내림차순
    if (a.level != b.level) {
        return a.level > b.level;
    }

    //공격력 내림차순
    if (a.attack != b.attack) {
        return a.attack > b.attack;
    }

    //시간순 오름차순
    return a.timestamp < b.timestamp;
}

class inven {
private:
    vector<Item> invens;
    int next_timestamp;

public:
    inven() : next_timestamp(0) {}

    void add_item(const string& name, const string& job, int attack, int level) {
        if (job != "A" && job != "B" && job != "C") {
            throw invalid_argument("Job must be 'A', 'B', or 'C'.");
        }

        Item newItem = { name, job, attack, level, next_timestamp };
        invens.push_back(newItem);
        next_timestamp++;
        cout << name << " 아이템 추가됨 (Timestamp: " << newItem.timestamp << ")" << endl;
    }

    // 정렬
    void sort_by_priority() {
        std::sort(invens.begin(), invens.end(), compareItem);
    }

    // 테스트용 출력 함수
    void print_items() {
        for (auto& item : invens) {
            cout << "이름: " << item.name
                << ", 직업: " << item.job
                << ", 공격력: " << item.attack
                << ", 레벨: " << item.level
                << ", 시간: " << item.timestamp
                << endl;
        }
    }
};



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

    inven my_inven;

    my_inven.add_item("A의 검", "A", 150, 50);  
    my_inven.add_item("B의 검" , "B", 10, 60);  
    my_inven.add_item("A의 활", "A", 150, 50); 
    my_inven.add_item("C의 지팡이", "C", 200, 40); 
    my_inven.add_item("B의 지팡이", "B", 20, 60);  
    my_inven.add_item("A의 지팡이", "A", 180, 50); 

    my_inven. print_items();
    my_inven.sort_by_priority();
    cout << endl;
    my_inven.print_items();
}