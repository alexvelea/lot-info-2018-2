#include <bits/stdc++.h>

using namespace std;

int seed;
mt19937 gen;

vector<int> RandomPermutation(int n) {
    vector<int> p(n);
    iota(p.begin(), p.end(), 0);
    shuffle(p.begin(), p.end(), gen);
    return p;
}

void Print(const vector<int>& p) {
    int n = (int)p.size();
    cout << n << '\n';
    for (int i = 0; i < n; ++i) {
        cout << 1 + p[i] << " \n"[i + 1 == n];
    }
}

vector<int> ApplyPermutation(const vector<int>& a, const vector<int> p) {
    vector<int> b(a.size());
    for (int i = 0; i < (int)a.size(); ++i) {
        b[p[i]] = a[i];
    }
    return b;
}

vector<int> GenTest(int x0, int num0, int x1, int num1) { 
    if (x0 == num0 and num1 == 0) {
        return RandomPermutation(num0);
    }
    vector<int> random_perm = RandomPermutation(num0 + num1);
    uniform_int_distribution<int> distr(1, 1 << 30);
    vector<int> p;
    for (int i = 0; i < num0; ++i) {
        const int el = distr(gen) % x0;
        p.push_back(el);
    }
    for (int i = 0; i < num1; ++i) {
        const int el = distr(gen) % x1;
        p.push_back(random_perm[x0 + el]);
    }
    return ApplyPermutation(p, RandomPermutation(num0 + num1));
}

int main(int argc, char** argv) {
    if (argc < 5) {
        exit(-1);
    }

    int n = atoi(argv[1]);
    int x0 = atoi(argv[2]), n0 = atoi(argv[3]);
    int x1 = atoi(argv[4]), n1 = n - n0;

    assert(1 <= n and n <= 30000);
    assert(x0 <= n0);
    assert(x1 <= n1);
    assert(x0 > 0 or (x0 == 0 and n0 == 0));
    assert(x1 > 0 or (x1 == 0 and n1 == 0));

    seed = (argc == 5) ? 0x5EED : atoi(argv[5]);
    gen = mt19937(seed);

    Print(GenTest(x0, n0, x1, n1));
}
