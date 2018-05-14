#include "anagrame.h"

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

void Solve(int, int n, int* a) {
    vector<int> v(n);
    for (int i = 0; i < n; ++i) {
        v[i] = i + 1;
    }
    vector<int> answers(n);
    for (int i = 0; i < n; ++i) {
        answers[i] = Ask(v.data());
        rotate(v.begin(), v.begin() + 1, v.end());
    }
    vector<int> res(n);
    for (int i = 0; i < n; ++i) {
        int v1 = answers[i], v2 = answers[(i + 1) % n];
//        cerr << v1 << ' ' << v2 << endl;
        int l = (v1 - v2 + n - 1) / 2;
//        cerr << l << endl;
        res[l] = i + 1;
    }
    GiveSolution(res.data());
}
