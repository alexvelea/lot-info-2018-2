/*
 *  Autor: George Chichirim
 *  Timp: O(p * 2 ^ p + log(p) * Q)
 *  Mem:  4 * 2 ^ p ints
 */
#include <bits/stdc++.h>

using namespace std;

const int MaxPut = 22;

vector<int> elem[MaxPut];
int *v;
int sol0[1 << MaxPut], n, put;

// _n e dimensiunea permutarii
// permutarea e indexata de la [0, n)
void SetPermutation(int _n, int* _perm)
{
    n = _n;
    v = _perm;
    for(put = 0; (1 << put) < n; put++);

    // init
    for(int step = 0; step < put; step++)
    {
        int lim1 = 1 << step;
        int lim2 = 1 << (step + 1);
        elem[step].resize(lim2);

        // merge
        for(int x = 0; x < n; x += lim2)
        {
            merge(v + x, v + x + lim1, v + x + lim1, v + x + lim2, elem[step].begin());
            for(int i = 0; i < lim2; i++)
                v[x + i] = elem[step][i];
        }

        // update sol0
        for(int x = 0; x < n; x += lim2)
        {
            int maxx = 0;
            for(int i = 0; i < lim2; i++)
                maxx = max(maxx, v[x + i]);
            sol0[maxx]++;
        }

        // compute elem[step]
        for(int i = 0; i < lim2; i++)
            elem[step][i] = n;
        for(int x = 0; x < n; x += lim2)
            for(int i = 0; i < lim2; i++)
                elem[step][lim2 - i - 1] = min(elem[step][lim2 - i - 1], v[x + i]);
    }
}

// se apeleaza de comisie
int Solve(int x, int k)
{
    if(k == 0) return sol0[x];
    int st = 0, dr = put - 1;
    while(st <= dr)
    {
        int mid = (st + dr) / 2;
        if((1 << (mid + 1)) <= x + 1 && elem[mid][min(k, (1 << (mid + 1)) - 1)] <= x) st = mid + 1;
        else dr = mid - 1;
    }
    return st;
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
