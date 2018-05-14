#include "anagrame.h"
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

vector<int> costs;
int identityCost;

void Solve(int , int n, int* a) {
    costs = vector<int>(n);
    vector<int> v(n);
    for (int i = 0; i < n; ++i) {
        v[i] = a[i];
    }
    identityCost = Ask(v.data());
    for (int i = 0; i < n; ++i) {
        int pos = 1;
        for (int j = 0; j < n; ++j) {
            if (j != i) {
                v[pos++] = a[j];
            }
        }
        v[0] = a[i];
        costs[i] = Ask(v.data()) - identityCost;
    }
    for (int i = 0; i < n; ++i) {
        v[i] = i;
    }
    stable_sort(v.begin(), v.end(), [n, a](int x, int y) -> bool {
//        if (a[x] == a[y]) {
//            return false;
//        }
       bool flag = false;
       if (x > y) {
           flag = true;
           swap(x, y);
       }
       vector<int> v(n);
       for (int i = 0, pos = 2; i < n; ++i) {
           if (i != x && i != y) {
               v[pos++] = a[i];
           }
       }
       v[0] = a[x]; v[1] = a[y];
       int p = Ask(v.data());
       swap(v[0], v[1]);
//       cerr << x << ' ' << y << ' '<<  identityCost << ' ' << costs[x] << ' ' << costs[y] << ' ' << p << endl;
       return (identityCost + costs[x] + costs[y] - 1 == p) ^ flag;
    });
    for (int i = 0; i < n; ++i) {
        v[i] = a[v[i]];
    }
    GiveSolution(v.data());
}
