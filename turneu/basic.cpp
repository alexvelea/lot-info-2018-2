/*
 *  Autor 
 *  Timp: O(
 *  Mem:  O(
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// _n e dimensiunea permutarii
// permutarea e indexata de la [0, n)
void SetPermutation(int _n, int* _perm) {

}

// se apeleaza de comisie
int Solve(int x, int k) {

}

// Citeste din fisier sa nu complicam lucrurile de geaba. 
void Mock() {
    int n; cin >> n;
    vector<int> read_order(n);
    for (auto& itr : read_order) {
        cin >> itr;
    }

    SetPermutation(n, &read_order[0]);

    int q; cin >> q;
    while (q--) {
        int x, k; cin >> x >> k;
        cout << Solve(x, k) << '\n';
    }
}

int main() {
    Mock();
    return 0;
}
