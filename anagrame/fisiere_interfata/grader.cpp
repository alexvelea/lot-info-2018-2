#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>

#include "anagrame.h"
using namespace std;

namespace COMISIE {

const int kMaxN = 1 << 10;

int n;
int test_number;
vector<int> a;

int max_ask_calls;
int num_ask_calls;

vector<int> first_itr, el_order;

enum Status { FINISH_OK, QUERY_LIMIT_KILL, WRONG_INTERACTIO, NO_ANSWER };

void Judge(const Status& s, vector<int> els) {
#ifdef TUMBLETEST
    std::ostream& fout = std::cout;
#else
    ofstream fout("anagrame.out");
#endif

    const int SAFE_ONE = 118153912;
    const int SAFE_TWO = 598692164;

    fout << (SAFE_ONE ^ SAFE_TWO) << '\n';
    fout << s << ' ' << num_ask_calls << '\n';
    for (auto& itr : els) {
        fout << itr << ' ';
    }
    fout << endl;

#ifndef TUMBLETEST
    fout.close();
#endif

    exit(0);
}

void SetSolution(int els[]) {
    Judge(FINISH_OK, vector<int>(els, els + n));
};

void QueryLimitExceded() {
    Judge(QUERY_LIMIT_KILL, {});
};

void WrongInteraction() {
    Judge(WRONG_INTERACTIO, {});
};

void NoAnswer() {
    Judge(NO_ANSWER, {});
}

int countInversions(vector<int>& a, int left, int right) {
    if (left + 1 >= right) {
        return 0;
    } else {
        int mid = (left + right) / 2;
        int ans = countInversions(a, left, mid) + countInversions(a, mid, right);
        vector<int> v(right - left);
        for (int i = left, j = mid, k = 0; i < mid || j < right;) {
            if (i < mid && (j == right || a[i] < a[j])) {
                ans += j - mid;
                v[k++] = a[i++];
            } else {
                v[k++] = a[j++];
            }
        }
        copy(v.begin(), v.end(), a.begin() + left);
        return ans;
    }
}

struct FenwickTree {
    vector<int> els;
    FenwickTree(int n) : els(n + 1, 0) { }
    
    int Query(int pos) {
        int ans = 0;
        while (pos) {
            ans += els[pos];
            pos -= pos & (-pos);
        }

        return ans;
    }

    void Update(int pos, int val) {
        while (pos < (int)els.size()) {
            els[pos] += val;
            pos += pos & (-pos);
        }
    }
};

int Ask(int* b) {
    // Count how many queries the contestant made
    if (num_ask_calls == 5 * max_ask_calls) {
        QueryLimitExceded();
    }
    
    num_ask_calls += 1;

    vector<int> freq_a(n + 1, 0), freq_b(n + 1, 0);
    for (int i = 0; i < n; i += 1) {
        freq_a[b[i]] += 1;
    }

    for (auto& itr : a) {
        freq_b[itr] += 1;
    }

    if (freq_a != freq_b) {
        WrongInteraction();
    }

    
    vector<int> actual_perm;
    
    vector<int> current_itr = first_itr;
    for (int i = 0; i < n; i += 1) {
        actual_perm.push_back(el_order[current_itr[b[i]]]);
        current_itr[b[i]] += 1;
    }
    
    FenwickTree f_tree(n);
    int ans = 0;

    for (int i = 0; i < n; i += 1) {
        ans += i - f_tree.Query(actual_perm[i]);
        f_tree.Update(actual_perm[i], +1);
    }

    return ans;
}

void Init() {
    vector<int> num_ap(n + 1);
    for (auto itr : a) {
        num_ap[itr] += 1;
    }

    first_itr = vector<int>(n + 1);
    for (int i = 1, sum = 0; i <= n; i += 1) {
        first_itr[i] = sum;
        sum += num_ap[i];
    }

    auto current_itr = first_itr;
    el_order = vector<int>(n);

    for (int i = 0; i < n; i += 1) {
        el_order[current_itr[a[i]]] = i + 1;
        current_itr[a[i]] += 1;
    }
}

void Run() {
#ifdef TUMBLETEST
    istream& fin = std::cin;
#else
    ifstream fin("anagrame.in");
#endif

    fin >> n >> max_ask_calls >> test_number;
    a = vector<int>(n);
    for (int& x : a) {
        fin >> x;
    }

    Init();

    // make contestant copy of array
    int* b = new int[n];
    for (int i = 0; i < n; ++i) {
        b[i] = COMISIE::a[i];
    }

    sort(b, b + n);
    Solve(test_number, n, b);

    delete[] b;

    COMISIE::NoAnswer();

#ifndef TUMBLETEST
    fin.close();
#endif
}

};

void GiveSolution(int* a) {
    COMISIE::SetSolution(a);
}

int Ask(int* a) {
    return COMISIE::Ask(a);
}

int main() {
    COMISIE::Run();
}
