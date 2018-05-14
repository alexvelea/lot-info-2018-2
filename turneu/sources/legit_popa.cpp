#include "turneu.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <array>

using namespace std;

#define SZ(x) ((int) (x).size())

static array<vector<int>, 21> firstOfValue;
static vector<int> firstBad;

static vector<int> pos;
static int n;

void init(int m, int* a) {
    int n = 0;
    while ((1 << n) < m) {
        n++;
    }
    ::n = n;
    pos = vector<int>(1 << n);
    for (int i = 0; i < (1 << n); ++i) {
        pos[a[i]] = i;
    }
    {
        vector<int> maximum(2 * m - 1);
        for (int i = SZ(maximum) - m; i < SZ(maximum); ++i) {
            maximum[i] = a[i - (SZ(maximum) - m)];
        }
        firstBad = vector<int>(m, -1);
        for (int i = SZ(maximum) - m - 1; i >= 0; --i) {
            maximum[i] = max(maximum[2 * i + 1], maximum[2 * i + 2]);
            int v = maximum[2 * i + 1] ^ maximum[2 * i + 2] ^ maximum[i];
            firstBad[v] = i;
        }
    }
    for (int lvl = 0; lvl <= n; ++lvl) {
        firstOfValue[lvl] = vector<int>(1 << (n - lvl), -1);
    }
    vector<int> cnt(2 * m - 1, 0);
    vector<int> vmax(n + 1, 0);
    for (int val = 0; val < m; ++val) {
        int node = pos[val] + SZ(cnt) - m;
        for (int lvl = n; lvl >= 0; --lvl, node = (node - 1) / 2) {
            if (++cnt[node] > vmax[lvl]) {
                vmax[lvl] = cnt[node];
                firstOfValue[lvl][cnt[node] - 1] = val;
            }
        }
    }
}

int query(int x, int k) {
    int m = 1 << n;
    int log2 = 0;
    for (int y = x + 1; y > 1; y /= 2) {
        log2++;
    }
    if (firstBad[x] == -1) {
        return n;
    }
    int ans1 = -1;
    for (int node = (2 * m - 1) - m + pos[x]; node != firstBad[x]; node = (node - 1) / 2) {
        ans1++;
    }
    if (k == 0) {
        return ans1;
    }
    int ans2 = -1;
    for (int lvl = n, node = (2 * m - 1) - m + pos[x]; lvl >= 0; --lvl, node = (node - 1) / 2) {
        if (k >= SZ(firstOfValue[lvl])) {
            ans2++;
            continue;
        }
        int p = SZ(firstOfValue[lvl]) - k - 1;
        if (firstOfValue[lvl][p] <= x) {
            ans2++;
        } else {
            break;
        }
    }
    return min(log2, max(ans1, ans2));
}

#undef SZ
