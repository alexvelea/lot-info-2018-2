#include "turneu.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

int n;
int *p;
int lg = 0;

    vector<int> tree;
    vector<int> poz;
    vector<vector<int>> precalc;
    vector<int> best_subtree;

void init(int _n, int *_p) {
	n = _n;

    int nn = n;

    while(nn > 0) {
        lg += 1;
        nn /= 2;
    }
    
    tree = vector<int>(2 * n + 1, 0);
    poz = vector<int>(n + 1, 0);

    for(int i = 0; i < n; ++i) {
        p[i] += 1;
        poz[p[i]] = i;
    }
    

    precalc.resize(n);
    best_subtree = vector<int>(lg + 1, 0);
    
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
}

int query(int node, int k) {
	node += 1;
	int ans = 0;
    int idx = node - 1;
    for(int depth = lg; depth >= 1; --depth) {
        if((1 << (lg - depth)) - precalc[idx][depth] <= k and node >= (1 << (lg - depth))) {
            ans = lg - depth;
        }
    }
        return ans;	
}
