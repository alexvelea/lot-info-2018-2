/*
 *  Autor Alex Velea
 *  Timp: O(N * N * Q) fuck knows
 */
#include <bits/stdc++.h>
#include "turneu.h"
#define Debug(...)
using namespace std;

const int kMaxPow = 20, kMaxN = 1 << kMaxPow;

int n, p;
int order[kMaxN];

// _n e dimensiunea permutarii
// permutarea e indexata de la [0, n)
void init(int _n, int* _perm) {
    while ((1 << p) < _n) {
        p += 1;
    }
    n = _n;

    for (int i = 0; i < n; i += 1) {
        order[i] = _perm[i];
    }
}

// se apeleaza de comisie
int query(int x, int k) {
    vector<int> o(n);
    for (int i = 0; i < n; i += 1) {
        o[i] = order[i];
    }

    for (auto& itr : o) {
        if (itr == x) {
            itr = 1;
        } else {
            itr = (itr > x) * 2;
        }
    }

    int ans = 0;
    for (int step = 0; step <= p; step += 1) {
        int l = (1 << step);
        for (int bucket = 0; bucket < (1 << (p - step)); bucket += 1) {
            int first_el = l * bucket;

            int remaining_0 = 0;
            for (int i = 0; i < n; i += 1) {
                if (first_el <= i and i < first_el + l) {
                    continue;
                }

                if (o[i] < 2) {
                    remaining_0 += 1;
                }
            }

            int required = 0;
            bool inside = false;
            for (int i = 0; i < l; i += 1) {
                 if (o[i + first_el] == 2) {
                    required += 1;
                 }

                 if (o[i + first_el] == 1) {
                    inside = true;
                 }
            }

            Debug(step, bucket, inside, required, remaining_0);
            if (remaining_0 >= required and required <= k) {
                if (k) {
                    ans = step;
                } else if (inside) {
                    ans = step;
                }
            }
        }
    }

    return ans;
}
