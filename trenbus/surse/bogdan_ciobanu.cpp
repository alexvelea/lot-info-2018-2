#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_set>

using namespace std;

const int kMaxN = 1 << 17;

struct Edge {
    int x, y, cost;
    Edge() = default;
    Edge(int x_, int y_, int cost_) : x(x_), y(y_), cost(cost_) {}

    bool operator <(const Edge& rhs) const {
        return cost < rhs.cost;
    }

    friend istream& operator >>(istream& is, Edge& rhs) {
        is >> rhs.x >> rhs.y >> rhs.cost; --rhs.x; --rhs.y;
        return is;
    }
};

Edge edges[2][kMaxN - 1];
vector<int> sets[kMaxN];
pair<int, int> change[kMaxN - 1];
int where[kMaxN], poz[kMaxN];
unordered_set<int> freq[kMaxN];

int Root(int node) {
    while (poz[node] != node) {
        node = poz[node];
    }
    return node;
}

bool SolveDecision(int n, int mx_cost) {
    for (int i = 0; i < n; ++i) {
        sets[i].clear();
        freq[i].clear();
    }
    for (int i = 0; i < n; ++i) {
        where[i] = i;
        sets[i].push_back(i);
    }
    for (int i = n - 2; i >= 0; --i) {
        int x = where[edges[1][i].x], y = where[edges[1][i].y];
        if (sets[x].size() < sets[y].size()) {
            swap(x, y);
        }

        change[i] = {x, y};
        for (auto&& el : sets[y]) {
            sets[x].push_back(el);
            where[el] = x;
        }
    }

    for (int i = 0; i < n; ++i) {
        poz[i] = i;
        freq[i].insert(where[i]);
    }

    for (int i = 0, j = 0; i < n - 1; ++i) {
        while (j < n - 1 and edges[1][j].cost + edges[0][i].cost > mx_cost) {
            int curr, oth; tie(curr, oth) = change[j];
            for (auto&& el : sets[oth]) {
                int forest = Root(el);
                freq[forest].erase(curr);
                if (freq[forest].find(oth) != freq[forest].end()) {
                    return true;
                }
                freq[forest].insert(oth);
                sets[curr].pop_back();
            }
            ++j;
        }

        int x = Root(edges[0][i].x), y = Root(edges[0][i].y);
        if (freq[x].size() < freq[y].size()) {
            swap(x, y);
        }
        poz[y] = x;
        for (auto&& it : freq[y]) {
            if (freq[x].find(it) != freq[x].end()) {
                return true;
            }
            freq[x].insert(it);
        }
        freq[y].clear();
    }
    return false;
}

int main() {
    freopen("trenbus.in", "r", stdin);
    freopen("trenbus.out", "w", stdout);
    int n; cin >> n;
    for (int t = 0; t < 2; ++t) {
        for (int i = 0; i < n - 1; ++i) {
           cin >> edges[t][i];
        }
        sort(edges[t], edges[t] + n - 1);
    }

    reverse(edges[1], edges[1] + n - 1);

    int low = 1, high = 2000000000 + 1; 
    while (high - low > 1) {
        int mid = low + (high - low) / 2; 
        if (SolveDecision(n, mid)) {
            high = mid;
        } else {
            low = mid;
        }
    }
    cout << high << endl;
}
