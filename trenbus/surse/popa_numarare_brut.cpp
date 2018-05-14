#include <algorithm>
#include <numeric>
#include <set>
#include <fstream>
#include <unordered_map>
#include <map>
#include <vector>
#include <deque>
#include <iostream>
#include <queue>

#define SZ(x) ((int) (x).size())
using namespace std;

int main() {
    ifstream fin("trenbus.in");
    ofstream fout("trenbus.out");

    int n;
    fin >> n;
//    cerr << n << endl;

    vector<vector<pair<int, int>>> tree1(n);
    vector<vector<pair<int, int>>> tree2(n);
    for (int i = 1; i < n; ++i) {
        int a, b, cost;
        fin >> a >> b >> cost;
        a--; b--;
        tree1[a].push_back(make_pair(b, cost));
        tree1[b].push_back(make_pair(a, cost));
    }
    for (int i = 1; i < n; ++i) {
        int a, b, cost;
        fin >> a >> b >> cost;
        a--; b--;
        tree2[a].push_back(make_pair(b, cost));
        tree2[b].push_back(make_pair(a, cost));
    }

    vector<vector<int>> cost1(n, vector<int>(n, -1));
    vector<vector<int>> cost2(n, vector<int>(n, -1));
    auto bfs = [](vector<vector<int>>& cost, const vector<vector<pair<int, int>>>& tree) {
        int n = SZ(tree);
        for (int start = 0; start < n; ++start) {
            cost[start][start] = 0;
            queue<int> q;
            q.push(start);
            while (!q.empty()) {
                int node = q.front();
                q.pop();
                for (const pair<int, int>& e: tree[node]) {
                    if (cost[start][e.first] == -1) {
                        cost[start][e.first] = max(cost[start][node], e.second);
                        q.push(e.first);
                    }
                }
            }
        }
    };
    bfs(cost1, tree1);
    bfs(cost2, tree2);
    int ans = (int) 2e9 + 5;
    int cnt = 0;
    for (int x = 0; x < n; ++x) {
        for (int y = 0; y < x; ++y) {
            if (ans > cost1[x][y] + cost2[x][y]) {
                ans = cost1[x][y] + cost2[x][y];
                cnt = 1;
            } else if (ans == cost1[x][y] + cost2[x][y]) {
                cnt++;
            }
        }
    }
    fout << ans << ' ' << cnt << '\n';

    fin.close();
    fout.close();
}
