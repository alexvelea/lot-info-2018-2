#include "anagrame.h"
#include <iostream>
#include <vector>
using namespace std;

void Solve(int, int n, int* a) {
    vector<int> v(n);
    vector<int> res(n);
    for (int i = 0; i < n; ++i) {
        int v1, v2;
        {
            int pos = 1;
            for (int j = 0; j < n; ++j) {
                if (j != i) {
                    v[pos++] = a[j];
                }
            }
            v[0] = a[i];
            v1 = Ask(v.data());
        }
        {
            int pos = 0;
            for (int j = 0; j < n; ++j) {
                if (j != i) {
                    v[pos++] = a[j];
                }
            }
            v.back() = a[i];
            v2 = Ask(v.data());
        }
//        cerr << v1 << ' ' << v2 << endl;
        int l = (v1 - v2 + n - 1) / 2;
//        cerr << l << endl;
        res[l] = i + 1;
    }
    GiveSolution(res.data());
}