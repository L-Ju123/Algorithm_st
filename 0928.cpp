#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
using namespace std;

struct WeightedGraph {
    int n;  // 정점 개수
    vector<vector<pair<int, int>>> adj; // 인접 리스트 (정점, 가중치)

    explicit WeightedGraph(int n = 0) : n(n), adj(n) {}

    void addEdge(int u, int v, int w, bool undirected = true) {
        if (u < 0 || v < 0) return;
        // ensure adjacency size
        if (u >= n || v >= n) return;
        adj[u].push_back({v, w});
        if (undirected) adj[v].push_back({u, w});
    }

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

// 지하철 노선 추가
void addSubwayLines(WeightedGraph &gr, map<string, int> &station, vector<string> &names) {
    vector<vector<string>> lines = {
        // 1호선
        {"다대포", "하단", "자갈치", "초량", "부산역", "서면", "연산", "동래", "부산대", "노포"},
        // 2호선
        {"양산", "율리", "덕천", "덕포", "사상", "개금", "동의대", "가야", "서면", "뭇골", "수영", "해운대"},
        // 3호선
        {"대저", "구포", "덕천", "만덕", "미남", "거제", "연산", "망마", "수영"},
        // 4호선
        {"미남", "동래", "충렬사", "연산"},
        // 경전철
        {"사상", "공항", "대저", "봉황", "장산대입구"}
    };

    // 1) 모든 역을 map에 등록 (중복 방지)
    int idx = 0;
    for (auto &line : lines) {
        for (auto &st : line) {
            if (station.find(st) == station.end()) {
                station[st] = idx++;
                names.push_back(st);
            }
        }
    }

    // 2) 그래프 크기 설정
    gr = WeightedGraph(static_cast<int>(station.size()));

    // 3) 인접 역 간선 추가 (가중치 = 1)
    for (auto &line : lines) {
        for (int i = 0; i + 1 < (int)line.size(); ++i) {
            int u = station[line[i]];
            int v = station[line[i + 1]];
            gr.addEdge(u, v, 1);
        }
    }
}

// Dijkstra: start -> end 최단 경로(가중치 정수, 여기서는 1)
// gr은 인접 리스트 기반 그래프, names는 인덱스->이름 매핑
void Dijkstra(const WeightedGraph &gr, int start, int end, const vector<string> &names) {
    int num = gr.adj.size();
    if (start < 0 || end < 0 || start >= num || end >= num) {
        cout << "잘못된 정점 인덱스\n";
        return;
    }

    const int INF = 1000000;
    vector<bool> vis(num, false);
    vector<int> dist(num, INF);
    vector<int> prev(num, -1);

    dist[start] = 0;

    // (원래 코드와 같은 O(N^2) 방식)
    for (int iter = 0; iter < num; ++iter) {
        int u = -1;
        int best = INF;
        for (int i = 0; i < num; ++i) {
            if (!vis[i] && dist[i] < best) {
                best = dist[i];
                u = i;
            }
        }
        if (u == -1) break;
        vis[u] = true;

        for (auto [v, w] : gr.adj[u]) {
            if (!vis[v] && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
            }
        }
    }

    cout << "출발역: " << names[start] << "\n";
    cout << "도착역: " << names[end] << "\n";
    if (dist[end] == INF) {
        cout << "경로 없음\n";
        return;
    }

    cout << "최소 이동 거리(정거장 수): " << dist[end] << "\n";
    cout << "이동 경로: ";
    vector<int> path;
    for (int cur = end; cur != -1; cur = prev[cur]) path.push_back(cur);
    for (int i = (int)path.size() - 1; i >= 0; --i) {
        cout << names[path[i]];
        if (i) cout << " -> ";
    }
    cout << "\n";
}

// ---------- UnionFind (수정된 생성자) ----------
struct UnionFind {
    vector<int> parent;
    vector<int> rankv;
    UnionFind(int n = 0) : parent(n), rankv(n, 0) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (rankv[a] < rankv[b]) swap(a, b);
        parent[b] = a;
        if (rankv[a] == rankv[b]) rankv[a]++;
        return true;
    }
};

// ---------- Item & backpack (출력 문자 수정) ----------
struct Item {
    string name;
    int weight;
    int count;
    Item(string name, int w, int c) : name(name), weight(w), count(c) {}
};

void backpack(const vector<Item>& items, int capacity) {
    vector<tuple<string, int, int>> ave;
    for (auto &it : items) {
        int gram = it.weight / max(1, it.count);
        ave.push_back({it.name, gram, it.count});
    }
    sort(ave.begin(), ave.end(), [](auto &a, auto &b) { return get<1>(a) > get<1>(b); });

    vector<int> useCount(ave.size(), 0);
    int remain = capacity;
    for (int i = 0; i < (int)ave.size(); ++i) {
        int val = get<1>(ave[i]);
        int maxCnt = get<2>(ave[i]);
        for (int j = 0; j < maxCnt; ++j) {
            if (remain - val >= 0) {
                remain -= val;
                useCount[i]++;
            } else break;
        }
    }
    for (int i = 0; i < (int)ave.size(); ++i) {
        cout << get<0>(ave[i]) << ": 사용무게 = " << useCount[i] * get<1>(ave[i])
             << " (개수 " << useCount[i] << ")\n";
    }
}

// ---------- Prim (Vector only) ----------
void PrimAl(const WeightedGraph &gr, int start = 0) {
    int n = gr.n;
    const int INF = 1000000;
    vector<int> key(n, INF);
    vector<int> parent(n, -1);
    vector<bool> inMST(n, false);

    key[start] = 0;
    for (int i = 0; i < n; ++i) {
        int u = -1, best = INF;
        for (int v = 0; v < n; ++v) {
            if (!inMST[v] && key[v] < best) { best = key[v]; u = v; }
        }
        if (u == -1) break;
        inMST[u] = true;
        for (auto [v, w] : gr.adj[u]) {
            if (!inMST[v] && w < key[v]) {
                key[v] = w;
                parent[v] = u;
            }
        }
    }

    int total = 0;
    cout << "Prim MST edges:\n";
    for (int i = 0; i < n; ++i) {
        if (parent[i] != -1) {
            cout << parent[i] << " - " << i << " (w=" << key[i] << ")\n";
            total += key[i];
        }
    }
    cout << "Total Weight = " << total << "\n";
}

// ---------- Kruskal ----------
void KruskalAl(const WeightedGraph &gr) {
    struct Edge { int u, v, w; };
    vector<Edge> edges;
    for (int u = 0; u < gr.n; ++u) {
        for (auto [v,w] : gr.adj[u]) {
            if (u < v) edges.push_back({u,v,w});
        }
    }
    sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b){ return a.w < b.w; });
    UnionFind uf(gr.n);
    vector<Edge> mst;
    int total = 0;
    for (auto &e : edges) {
        if (uf.unite(e.u, e.v)) {
            mst.push_back(e);
            total += e.w;
        }
    }
    cout << "Kruskal MST edges:\n";
    for (auto &e : mst) cout << e.u << " - " << e.v << " (w=" << e.w << ")\n";
    cout << "Total Weight = " << total << "\n";
}

// ---------- main ----------
int main() {
    // (1) 간단한 그래프 예시
    {
        WeightedGraph g(5);
        g.addEdge(0,1,10);
        g.addEdge(0,2,5);
        g.addEdge(1,3,2);
        g.addEdge(2,3,7);
        g.addEdge(3,4,1);
        cout << "Simple graph:\n";
        g.print();
        cout << "\n";
    }

    // (2) 도시 예시와 Dijkstra
    {
        map<string,int> city;
        city["서울"] = 0; city["대전"] = 1; city["대구"] = 2; city["부산"] = 3;
        vector<string> cityNames = {"서울","대전","대구","부산"};
        WeightedGraph gr(4);
        gr.addEdge(city["서울"], city["대전"], 3);
        gr.addEdge(city["대전"], city["대구"], 4);
        gr.addEdge(city["대구"], city["부산"], 6);
        gr.addEdge(city["서울"], city["부산"], 15);
        cout << "City graph distances from 서울:\n";
        Dijkstra(gr, city["서울"], city["부산"], cityNames);
        cout << "\n";
    }

    // (3) backpack 예시
    {
        vector<Item> items;
        items.emplace_back("금", 100, 2);
        items.emplace_back("은", 300, 10);
        items.emplace_back("철", 500, 50);
        cout << "Backpack example (capacity 600):\n";
        backpack(items, 600);
        cout << "\n";
    }

    // (4) 부산 지하철 전체 구성 및 사용자 입력으로 최단 경로 계산
    map<string,int> station;
    vector<string> names;
    WeightedGraph subway(0);
    addSubwayLines(subway, station, names);

    cout << "등록된 역 개수: " << station.size() << "\n";
    // optional: names and indices print (comment out if too verbose)
    // for (int i = 0; i < (int)names.size(); ++i) cout << i << ": " << names[i] << "\n";

    string startStation, endStation;
    cout << "출발역 입력: ";
    cin >> startStation;
    cout << "도착역 입력: ";
    cin >> endStation;

    if (station.find(startStation) == station.end() || station.find(endStation) == station.end()) {
        cout << "입력한 역 이름이 존재하지 않습니다.\n";
        return 0;
    }

    int s = station[startStation];
    int e = station[endStation];
    Dijkstra(subway, s, e, names);

    return 0;
}
