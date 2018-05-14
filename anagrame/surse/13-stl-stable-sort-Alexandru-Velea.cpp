#include "anagrame.h"
#include <cassert>
#include <algorithm>
#include <vector>
using namespace std;

const int kMaxN = 1 << 10;

enum SortType {
    MergeSort,
    STLSort,
    StableSort,
};

const bool kUseSmartCompare = false;
const SortType kSortType = StableSort;

int first_element[kMaxN + 1];
int n, *perm;
bool SmartCompare(int a, int b) {
    if (a == b) {
        return false;
    }

    bool reverse = false;
    if (a > b) {
        reverse = true;
        swap(a, b);
    }

    bool ans = false;

    vector<int> ask = {a, b};
    for (int i = 1; i <= n; i += 1) {
        if (i == a or i == b) {
            continue;
        }

        ask.push_back(i);
    }

    int cost = first_element[a] + first_element[b] - first_element[1] - Ask(ask.data());
    if (cost == +1) {
        ans = true;
    } else if (cost == -1) {
        ans = false;
    } else {
        assert(0);
    }

    return ans ^ reverse;
}

bool DummySort(int a, int b) {
    vector<int> ask = {a, b};
    for (int i = 1; i <= n; i += 1) {
        if (i == a or i == b) {
            continue;
        }

        ask.push_back(i);
    }
    
    auto A = Ask(ask.data());
    swap(ask[0], ask[1]);
    auto B = Ask(ask.data());

    if (A < B) {
        return true;
    } else {
        return false;
    }
}


bool Compare(int a, int b) {
    if (a == b) { return false; }

    if (kUseSmartCompare) {
        return SmartCompare(a, b);
    } else {
        return DummySort(a, b);
    }
}

void Merge(int len, int perm[]) {
    if (len == 1) {
        return;
    }

    Merge(len / 2, perm);
    Merge(len / 2, perm + len / 2);

    int a = len / 2;
    int b = len / 2;
    int *A = perm;
    int *B = perm + len / 2;

    vector<int> ans;

    random_shuffle(perm, perm + n);

    while (a > 0 and b > 0) {
        if (Compare(*A, *B)) {
            ans.push_back(*A);
            A++;
            a -= 1;
        } else {
            ans.push_back(*B);
            B++;
            b -= 1;
        }
    }

    while (a > 0) {
        ans.push_back(*A);
        a -= 1;
        A++;
    }

    while (b > 0) {
        ans.push_back(*B);
        b -= 1;
        B++;
    }

    for (int i = 0; i < len; i += 1) {
        perm[i] = ans[i];
    }
}

void Solve(int , int _n, int _perm[]) {
    n = _n;
    perm = _perm;

    // Init first elements
    if (kUseSmartCompare) {
        for (int i = 1; i <= n; i += 1) {
            vector<int> ask = {i};
            for (int j = 1; j <= n; j += 1) {
                if (j == i) {
                    continue;
                }

                ask.push_back(j);
            }

            first_element[i] = Ask(ask.data());
        }
    }

    if (kSortType == MergeSort) {
        Merge(n, perm);
    } else if (kSortType == STLSort) {
        sort(perm + 0, perm + n, Compare);
    } else if (kSortType == StableSort) {
        stable_sort(perm + 0, perm + n, Compare);
    }

    GiveSolution(perm);
}
