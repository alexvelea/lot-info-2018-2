#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

using namespace std;

const int kMaxN = 1 << 17;

typedef __gnu_pbds::gp_hash_table<int, __gnu_pbds::null_type> UnorderedSet;
 
class Input {
  enum { SIZE = 1 << 14 };
public:
  Input() : ii(SIZE) {}
  char rchar() {
    if (ii == SIZE) { 
      ii = 0;
      int s = fread(in, 1, SIZE, stdin);
      if (s < SIZE) fill(in + s, in + SIZE, EOF);
    }
    return in[ii++];
  }
  int read_int() {
    char c;
    int ret = 0, sign = 0;
    while ((c = rchar()) < '-') if (c == EOF) return -1;
    if (c == '-') sign = 1;
    else ret = c - '0';
    while ((c = rchar()) >= '0') ret = ret * 10 + c - '0';
    return sign ? -ret : ret;
  }
  int ii;
  char in[SIZE];
} input;

struct Edge {
    int x, y, cost;
    Edge() = default;
    Edge(int x_, int y_, int cost_) : x(x_), y(y_), cost(cost_) {}

    bool operator <(const Edge& rhs) const {
        return cost < rhs.cost;
    }
};

Edge edges[2][kMaxN - 1];
int link[kMaxN], weight[kMaxN], root[kMaxN];
pair<int, int> change[kMaxN - 1];

int poz[kMaxN];
UnorderedSet freq[kMaxN];

int Root(int node) { 
    while (poz[node] != node) {
        node = poz[node];
    }
    return node;
}

void GetInitialState(int n) {
    for (int i = 0; i < n; ++i) {
        freq[i].clear();
    }
    
    for (int i = 0; i < n; ++i) {
        root[i] = i;
        link[i] = -1;
        weight[i] = 1;
    }
    for (int i = n - 2; i >= 0; --i) {
        int x = root[edges[1][i].x], y = root[edges[1][i].y];
        if (weight[x] < weight[y]) {
            swap(x, y);
        }

        weight[x] += weight[y];
        change[i] = {x, y};
        for (int z; ~y; y = z) {
            z = link[y];
            link[y] = link[x];
            link[x] = y;
            root[y] = x;
        }
    }

    for (int i = 0; i < n; ++i) {
        poz[i] = i;
        freq[i].insert(root[i]);
    }
}

bool InsertFreq(int id, int el) {
    if (freq[id].find(el) != freq[id].end()) {
        return true;
    }
    freq[id].insert(el);
    return false;
}

bool SolveDecision(int n, int mx_cost) {
    GetInitialState(n);

    for (int i = 0, j = 0; i < n - 1; ++i) {
        while (j < n - 1 and edges[1][j].cost + edges[0][i].cost > mx_cost) {
            int curr, oth; tie(curr, oth) = change[j];
            int z, prv = -1; 
            do {
                z = link[curr];
                link[curr] = link[z];
                link[z] = prv;
                prv = z;

                const int forest = Root(z);
                freq[forest].erase(curr);
                if (InsertFreq(forest, oth)) {
                    return true;
                }
            } while (z != oth);
            ++j;
        }

        int x = Root(edges[0][i].x), y = Root(edges[0][i].y);
        if (freq[x].size() < freq[y].size()) {
            swap(x, y);
        }
        poz[y] = x;
        for (auto&& it : freq[y]) {
            if (InsertFreq(x, it)) {
                return true;
            }
        }
        freq[y].clear();
    }
    return false;
}

int main() {
#ifdef LOCAL
    freopen("task.in", "r", stdin);
#else
    freopen("trenbus.in", "r", stdin);
    freopen("trenbus.out", "w", stdout);
#endif
    int n = input.read_int(); 
    for (int t = 0; t < 2; ++t) {
        for (int i = 0; i < n - 1; ++i) {
            edges[t][i].x = input.read_int() - 1;
            edges[t][i].y = input.read_int() - 1;
            edges[t][i].cost = input.read_int();
        }
        sort(edges[t], edges[t] + n - 1);
    }

    reverse(edges[1], edges[1] + n - 1);
    
    int ans = 0;
    for (int bit = 1 << 30; bit > 0; bit >>= 1) {
        if (not SolveDecision(n, ans | bit)) {
            ans |= bit;
        }
    }
    printf("%d\n", ans + 1);
}

