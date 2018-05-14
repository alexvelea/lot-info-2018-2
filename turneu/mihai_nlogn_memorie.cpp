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
    
    vector<vector<int>> precalc(n);
    vector<int> best_subtree(lg + 1, 0);
    
    auto toPrecalc = [&] (int node) -> bool {
        return true;
    };

    auto getIndex = [&] (int node) -> int {
        return node - 1;
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

    int q; cin >> q;

    for(int i = 0; i < q; ++i) {
        int x, k; cin >> x >> k;
        x += 1;
        cout << solveQuery(x, k) << "\n";
    }
}
