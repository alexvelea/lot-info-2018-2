#include <bits/stdc++.h>
 
using namespace std;
 
int main() {
    ifstream cin("sortall.in");
    ofstream cout("sortall.out");
    int n; cin >> n;
    vector<int> v(n);
    for (auto& iter : v) {
        cin >> iter;
    }
 
    unsigned cost = 0;
    for (int i = 0; i < n; ++i) {
        set<int> s;
        for (int j = i; j < n; ++j) {
            s.insert(v[j]);
             
            int num = 1;
            for (auto&& iter : s) {
                cost += unsigned(iter) * num++;
            }
        }
    }
 
    cout << cost << endl;
 
    return 0;
}