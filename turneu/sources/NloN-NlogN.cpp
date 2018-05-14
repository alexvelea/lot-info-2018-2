/*
 *  Alex Velea
 *  Timp: O(NlogN + QlogN)
 *  Mem:  O(NlogN)
 */

#include "turneu.h"
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

const int kMaxPow = 19, kMaxN = 1 << kMaxPow;

int n, p;
int* order;

short num_required_to_win_short[kMaxN][16];
int num_required_to_win_int[kMaxN][3];

void Set(int i, int step, int val) {
    if (step < 16) {
        num_required_to_win_short[i][step] = val;
    } else {
        num_required_to_win_int[i][step - 16] = val;
    }
}

int Get(int i, int step) {
    if (step < 16) {
        return num_required_to_win_short[i][step];
    } else {
        return num_required_to_win_int[i][step - 16];
    }   
}

void Init() {
    vector<int> where(n);
    for (int i = 0; i < n; i += 1) {
        where[order[i]] = i;
    }

    vector<int> subtree_size(n);
    for (int step = 0; step < p; step += 1) {
        for (int i = 0; i < (n >> step); i += 1) {
            subtree_size[i] = 0;
        }

        int mx = 0;
        for (int i = 0; i < n; i += 1) {
            int w = where[i];
            Set(i, step, (1 << step) - max(min(mx, (1 << step) - 1), subtree_size[w >> step] + 1));
            subtree_size[w >> step] += 1;
            mx = max(mx, subtree_size[w >> step]);
        }
    }
}

void init(int _n, int* _perm) {
    order = _perm;
    n = _n;
    while ((1 << p) < n) {
        p += 1;
    }

    Init();
}

int query(int x, int k) {
    k = min(k, x);
    
    if (x == n - 1) { return p; }

    int ans = 0;
    for (int step = 0; step < p and x + 1 >= (1 << step); step += 1) {
        if (Get(x, step) <= k) {
            ans = step;
        }
    }

    return ans;
}
