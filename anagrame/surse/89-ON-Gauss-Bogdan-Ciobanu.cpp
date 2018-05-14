#include "anagrame.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

const double kEps = 1e-6;

vector<int> SolveSystem(const vector<int>& a) {
    int n = (int)a.size();
    vector<vector<double>> mat(n, vector<double>(n + 1));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j < n - i) {
                mat[i][j] = 1.0;
            } else {
                mat[i][j] = -1.0;
            }
        }
        mat[i][n] = a[i];
    }

    for (int i = 0, j = 0; i < n and j <= n; ++j) {
        int k = i;
        while (k < n and fabs(mat[k][j]) < kEps) {
            ++k;
        }
        if (k == n) {
            continue;
        }

        swap(mat[k], mat[i]);
        for (k = j + 1; k <= n; ++k) {
            mat[i][k] /= mat[i][j];
        }
        mat[i][j] = 1.0;

        for (k = i + 1; k < n; ++k) {
            const double c = mat[k][j];
            for (int it = j; it <= n; ++it) {
                mat[k][it] -= c * mat[i][it];
            }
        }
        ++i;
    }

    vector<int> ans(n);
    for (int i = n - 1; i >= 0; --i) {
        int j = 0;
        while (j <= n and fabs(mat[i][j]) < kEps) {
            ++j;
        }

        double res = mat[i][n];
        for (int k = j + 1; k < n; ++k) {
            res -= mat[i][k] * ans[k];
        }
        ans[i] = (int)(res + 0.5);
    }
    return ans;
}

void Solve(int , int n, int* mset) {
    vector<int> a(n);
    for (int i = 0;  i < n; ++i) {
        a[i] = Ask(mset);
        rotate(mset, mset + 1, mset + n);
    }

    int* res = new int[n];
    for (int i = 0; i < n;) {
        int cnt = 0;
        while (i + cnt < n and mset[i + cnt] == mset[i]) {
            ++cnt;
        }

        int j = i;
        vector<int> s;
        while (true) {
            int st = cnt - (j - i);
            int dr = j - i;
            s.push_back(a[j % n] 
                + st * (st + 1) / 2 
                - n * (n + 1) / 2 + (n - dr) * (n - dr + 1) / 2);

            if (j == n or mset[j] != mset[i]) {
                break;
            }

            ++j;
        }

        auto pos = SolveSystem(s);
        for (int it = 1; it < (int)pos.size(); ++it) {
            res[pos[it] - 1] = mset[i];
        }
        i = j;
    }
    GiveSolution(res);
}
