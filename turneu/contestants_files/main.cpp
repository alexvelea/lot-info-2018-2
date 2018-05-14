#include <bits/stdc++.h>
#include "turneu.h"
using namespace std;

int main() {    
    ifstream cin("turneu.in");
    ofstream cout("turneu.out");

    int n;
    cin >> n;
    vector<int> els(n);
    for (auto& itr : els) {
        cin >> itr;
    }

    init(n, &(els[0]));

    int q;
    cin >> q;
    while (q--) {
        int x, y;
        cin >> x >> y;
        cout << query(x, y) << '\n';
    }
    
    return 0;
}
