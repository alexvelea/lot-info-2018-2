#include "mere.h"
#include <iostream>
#include <algorithm>
using namespace std;

const int kMaxN = 10000, kMod = 1e9 + 7;

int dp[kMaxN + 1][2];
pair<int, int> constraint[kMaxN + 1];
int mx_satisfy[2][kMaxN];
int n; long long v[2];

bool CanSatisfy(int c0, int c1, int id) {
    return mx_satisfy[id][c0] >= c1;
}

int& Add(int& x, int y) {
    x += y;
    if (x >= kMod) {
        x -= kMod;
    }
    return x;
}

void ComputeState(int r, int id) {
    for (int i = r; i > 0; --i) {
        if (not CanSatisfy(i, r, id)) {
            break;
        }
        
        if (r != n) {
            const int dist_oth = abs(constraint[r + 1].second 
                - constraint[i - 1].second);
            const long long delta_oth = v[1 - id] 
                * (constraint[r + 1].first - constraint[i - 1].first);

            if (dist_oth > delta_oth) {
                continue;
            }
        }
        Add(dp[r][id], dp[i - 1][1 - id]);
    }
}

int count(int N, int VA, int VB, int *X, int *T) {
    n = N;
    v[0] = VA; v[1] = VB;

    // (time, coordinate)
    constraint[0] = make_pair(0, 0);
    for (int i = 1; i <= n; ++i) {
        constraint[i] = make_pair(T[i - 1], X[i - 1]);
    }

    for (int p : {0, 1}) {
        mx_satisfy[p][n] = n;
        for (int i = n - 1; i >= 0; --i) {
            if (v[p] * (constraint[i + 1].first - constraint[i].first) 
                    >= abs(constraint[i + 1].second - constraint[i].second)) {
                mx_satisfy[p][i] = mx_satisfy[p][i + 1];
            } else {
                mx_satisfy[p][i] = i;
            }
        }
    }

    for (int p : {0, 1}) {
        if (CanSatisfy(0, 1, 1 - p)) {
            dp[0][p] = 1;
        }
    }

    for (int i = 1; i < n; ++i) {
        for (int p : {0, 1}) {
            ComputeState(i, p);
        }
    }

    int result = 0;
    for (int i = 1; i <= n; ++i) {
        for (int p : {0, 1}) {
            if (CanSatisfy(i, n, p)) {
                Add(result, dp[i - 1][1 - p]);
            }
        }
    }
    return result;
}
