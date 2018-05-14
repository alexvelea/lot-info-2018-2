
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <map>
#include <vector>
#include <deque>
#include <iostream>

#define SZ(x) ((int) (x).size())
using namespace std;

struct Hasher {
    size_t operator()(pair<int, int> a) const {
        return 998244353U * a.first + a.second;
    }
};

typedef unordered_map<pair<int, int>, int, Hasher> Map;
//typedef map<pair<int, int>, int>> Map;

struct Edge {
    int a, b, cost;
    bool operator<(const Edge& o) const {
        return cost < o.cost;
    }
};

int main() {
    ifstream fin("trenbus.in");
    ofstream fout("trenbus.out");

    int n;
    fin >> n;
//    cerr << n << endl;

    vector<Edge> edges1(n - 1);
    for (Edge& e: edges1) {
        fin >> e.a >> e.b >> e.cost;
        e.a--; e.b--;
    }
    vector<Edge> edges2(n - 1);
    for (Edge& e: edges2) {
        fin >> e.a >> e.b >> e.cost;
        e.a--; e.b--;
    }
    sort(edges1.begin(), edges1.end());
    sort(edges2.begin(), edges2.end());

    vector<vector<int>> graph1(n);
    vector<int> f(n);
    for (int i = 0; i < n; ++i) {
        f[i] = i;
    }
    vector<int> compSize(n, 1);
    auto find = [&](int x) -> int {
        while (f[x] != x) {
            x = f[x];
        }
        return x;
    };

    vector<int> edgeCut(n - 1);
    for (int i = 0; i < SZ(edges1); ++i) {
        Edge curr = edges1[i];
        int a = find(curr.a);
        int b = find(curr.b);
        if (compSize[a] < compSize[b]) {
            swap(a, b);
        }
        compSize[a] += compSize[b];
        f[b] = a;
        edgeCut[i] = b;
        graph1[a].push_back(b);
    }
    vector<vector<int>> graph2(n);
    deque<int> q;


    vector<int> index1(n), index2(n);
    Map freq;
    for (int i = 0, root = find(0); i < n; ++i) {
        index1[i] = root;
        index2[i] = i;
        freq[make_pair(root, i)]++;

        f[i] = i;
        compSize[i] = 1;
    }

    int biggerThanOne = 0;
    auto replace = [&](int start, int value, vector<int>& index, vector<vector<int>>& graph) {
        q.push_back(start);
        while (!q.empty()) {
            int node = q.front();
            q.pop_front();
            auto it = freq.find(make_pair(index1[node], index2[node]));
            if (--it->second == 0) {
                freq.erase(it);
            } else if (it->second == 1) {
                biggerThanOne--;
            }
            index[node] = value;
            if (++freq[make_pair(index1[node], index2[node])] == 2) {
                biggerThanOne++;
            }
            for (int to: graph[node]) {
                q.push_back(to);
            }
        }
    };
    int ans = (int) 2e9 + 2;
    for (int i = SZ(edges1) - 1, j = -1; i >= 0; --i) {
        while (j + 1 < SZ(edges2) && biggerThanOne == 0) {
            ++j;
            int a = find(edges2[j].a);
            int b = find(edges2[j].b);
            if (compSize[a] < compSize[b]) {
                swap(a, b);
            }
            compSize[a] += compSize[b];
            f[b] = a;
            graph2[a].push_back(b);
            replace(b, a, index2, graph2);
        }
        if (biggerThanOne > 0) {
            ans = min(ans, edges1[i].cost + edges2[j].cost);
        }
        replace(edgeCut[i], edgeCut[i], index1, graph1);
    }
    fout << 10 << endl;
    fout << ans << '\n';


    fin.close();
    fout.close();
}
