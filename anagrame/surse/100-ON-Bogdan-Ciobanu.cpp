#include "anagrame.h"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> SolveSystem(const vector<int>& a) {
    int n = (int)a.size();
    vector<int> ans(n - 1);
    for (int i = 0; i < n - 1; ++i) {
        ans[i] = (a[i] - a[i + 1]) / 2;
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
        for (auto&& it : pos) {
            res[it - 1] = mset[i];
        }
        i = j;
    }
    GiveSolution(res);
}
