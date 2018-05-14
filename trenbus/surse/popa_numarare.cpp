#include <algorithm>
#include <numeric>
#include <set>
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

//typedef unordered_map<pair<int, int>, int, Hasher> Map;
typedef map<pair<int, int>, int> Map;

struct Edge {
    int a, b, cost;
    bool operator<(const Edge& o) const {
        return cost < o.cost;
    }
};

int main() {
    ifstream fin("trenbus.in");
    ofstream fout("trenbus.out");

    int C, n;
    fin >> C >> n;
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

    int64_t pairs = 0;
    auto replace = [&](int start, int value, vector<int>& index, vector<vector<int>>& graph) {
        q.push_back(start);
        while (!q.empty()) {
            int node = q.front();
            q.pop_front();
            auto it = freq.find(make_pair(index1[node], index2[node]));
            pairs -= it->second - 1;
            if (--it->second == 0) {
                freq.erase(it);
            }
            index[node] = value;
            pairs += freq[make_pair(index1[node], index2[node])]++;
            for (int to: graph[node]) {
                q.push_back(to);
            }
        }
    };
    int ans = (int) 2e9 + 2;
    vector<int> v;
    int64_t count = 0;
    for (int i = SZ(edges1) - 1, j = -1; i >= 0; --i) {
        if (i + 1 == SZ(edges1) || edges1[i + 1].cost != edges1[i].cost) {
            while (j + 1 < SZ(edges2) && (pairs == 0 || (j >= 0 && edges2[j].cost == edges2[j + 1].cost))) {
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
            if (pairs > 0) {
                int curr = edges1[i].cost + edges2[j].cost;
                if (curr < ans) {
                    ans = curr;
                    count = pairs;
                } else if (curr == ans) {
                    count += pairs;
                }
            }
        }
        replace(edgeCut[i], edgeCut[i], index1, graph1);
    }
    /*
    vector<int> vv;
    adjacent_difference(begin(v), end(v), back_inserter(vv));
    vector<int> vvv;
    adjacent_difference(begin(vv), end(vv), back_inserter(vvv));
    for(auto& x : vvv){
        cout << x << ' '; }
    */
//    int cnt = 0;
//    for(int i = 0, j = 1, k = 2; k < v.size(); ++i, ++j, ++k)
//        if(v[i] > v[j] && v[j] < v[k])
//            cnt += 1;
//    cout << "\tNRTHINGS " << cnt << endl;
//    usleep(2000);

    if (C == 2) {
        fout << ans << ' ' << count << '\n';
    } else {
        fout << ans << '\n';
    }


    fin.close();
    fout.close();
}
