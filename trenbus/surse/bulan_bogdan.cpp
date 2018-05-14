#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
 
using namespace std;
 
const int kMaxN = 100000, kIters = 40;
 
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
 
vector<pair<int, int>> tree[2][kMaxN];
int cost[2][kMaxN], last_computed[2][kMaxN];
 
int main() {
    freopen("trenbuz.in", "r", stdin);
    freopen("trenbuz.out", "w", stdout);
    int n = input.read_int(); 
    for (int t = 0; t < 2; ++t) {
        for (int i = 0; i < n - 1; ++i) {
            int x = input.read_int() - 1, y = input.read_int() - 1, cost = input.read_int();
            tree[t][x].emplace_back(y, cost);
            tree[t][y].emplace_back(x, cost);
        }
    }
 
    int ans = 2000000000 + 1;
    memset(last_computed, -1, sizeof last_computed); 
    priority_queue<tuple<int, int, int>> heap;
    for (int i = 0; i < n; ++i) {
        cost[0][i] = 0;
        cost[1][i] = 0;
        last_computed[0][i] = i;
        last_computed[1][i] = i;
 
        heap.emplace(-cost[0][i], i, 0);
        heap.emplace(-cost[1][i], i, 1);
 
        int num_iters = kIters;
        while (not heap.empty() and num_iters-- > 0) {
            int c, node, t; tie(c, node, t) = heap.top();
            heap.pop();
            c = -c;
            for (auto&& son_edge : tree[t][node]) {
                if (last_computed[t][son_edge.first] == i) {
                    continue;
                }
                last_computed[t][son_edge.first] = i;
                cost[t][son_edge.first] = max(c, son_edge.second);
                heap.emplace(-cost[t][son_edge.first], son_edge.first, t);
                if (last_computed[1 - t][son_edge.first] == i 
                        and cost[t][son_edge.first] + cost[1 - t][son_edge.first] < ans) {
                    ans = cost[t][son_edge.first] + cost[1 - t][son_edge.first];
                    num_iters += kIters;
                }
            }
        }
        while (not heap.empty()) {
            heap.pop();
        }
    }
    cerr << "%" << endl;
    cout << ans << endl;
}
