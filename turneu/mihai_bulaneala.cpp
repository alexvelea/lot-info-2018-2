#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

int main() {

    int n; cin >> n;
    
    int nn = n, lg = 0;

    while(nn > 0) {
        lg += 1;
        nn /= 2;
    }
    
    vector<int> p(n, 0);
    vector<int> tree(2 * n + 1, 0);
    vector<int> poz(n + 1, 0);

    for(int i = 0; i < n; ++i) {
        cin >> p[i];
        p[i] += 1;
        poz[p[i]] = i;
    }
    
    vector<vector<int>> precalc(n / lg + 1);
    vector<int> best_subtree(lg + 1, 0);
    
    auto toPrecalc = [&] (int node) -> bool {
        return node % lg == 1;
    };

    auto getIndex = [&] (int node) -> int {
        return node / lg;
    };

    for(int i = 1; i <= n; ++i) {
        int node = poz[i] + n;

        vector<int> here(lg + 1, 0);

        for(int depth = lg; depth >= 1; depth -= 1, node /= 2) {
            tree[node] += 1;
            best_subtree[depth] = max(best_subtree[depth], tree[node]);
            if(best_subtree[depth] != tree[node] and best_subtree[depth] == (1 << (lg - depth))) {
                here[depth] = best_subtree[depth] - 1;
            } else {
                here[depth] = best_subtree[depth];
            }
        }

        if(toPrecalc(i)) {
            precalc[getIndex(i)] = here;
        }
    }

    auto solveQuery = [&] (int node, int k) -> int {
        int ans = 0;
        int idx = getIndex(node);
        for(int depth = lg; depth >= 1; --depth) {
            if((1 << (lg - depth)) - precalc[idx][depth] <= k and node >= (1 << (lg - depth))) {
                ans = lg - depth;
            }
        }
        return ans;
    };
    
    vector<int> otherTree(2 * n + 1, 0);

    auto bruteForce = [&] (int node, int k) -> int {
        int ans = 0;
        for(int i = n; i < node + n; ++i) {
            otherTree[i] = (p[i - n] < node);
        }

        int depth = lg - 1;

        for(int i = n - 1; i >= 1; --i) {
            otherTree[i] = otherTree[2 * i] + otherTree[2 * i + 1];
            if((1 << (lg - depth)) - otherTree[i] <= k and node >= (1 << (lg - depth))) {
                ans = lg - depth;
            }

            if((i & (i - 1)) == 0) {
                depth -= 1;
            }
        }
        return ans;
    };

    int q; cin >> q;
    int bad = 0;

    for(int i = 0; i < q; ++i) {
        int x, k; cin >> x >> k;
        x += 1;
        int xx = x;
        int diff = 0;

        while(not toPrecalc(x)) {
            x -= 1;
            diff += 1;
        }
        
        int low = solveQuery(x, k);
        int high = solveQuery(x, k + diff);

        if(low == high) {
            cout << low << "\n";
        } else {
            bad += 1;
            cout << bruteForce(xx, k) << "\n";
        }
    }

    cerr << n << '\t' << q << '\t' << bad << '\n';
}
