#include "anagrame.h"

#include <algorithm>
#include <iostream>
#include <vector>
#include <random>
#include <chrono>

using namespace std;

void Solve(int, int n, int* a) {
//    srand(chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count());
    vector<int> v(n);
    for (int i = 0; i < n; ++i) {
        v[i] = a[i];
    }
    vector<int> answers(n);
    for (int i = 0; i < n; ++i) {
        answers[i] = Ask(v.data());
        rotate(v.begin(), v.begin() + 1, v.end());
    }
    vector<int> res(n);
    vector<int> inds(n);
    for (int i = n - 1; i >= 0; --i) {
        if (i + 1 >= n || a[i] != a[i + 1]) {
            inds[i] = 0;
        } else {
            inds[i] = inds[i + 1] + 1;
        }
    }
    int cnt = 0;
    for (int i = 0; i < n; ++i) {
        int x = a[i];
        if (i > 0 && x == a[i - 1]) {
            cnt++;
        } else {
            cnt = 0;
        }
        int ind = inds[i];
        int v1 = answers[i], v2 = answers[(i + 1) % n];
        int size = n - cnt - ind;
//        cerr << v1 << ' ' << v2 << endl;
        int l = (v1 - v2 + size - 1) / 2;
//        cerr << l << endl;
        res[l + ind] = a[i];
    }
    GiveSolution(res.data());
}
