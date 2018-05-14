#include "anagrame.h"
#include <cassert>
#include <algorithm>
#include <vector>
using namespace std;

vector<int> Solve(int a, int b) {
    vector<int> prefix;

    while (1) {
        if (a == 0 or b == 0) {
            break;
        }
        vector<int> current = prefix;
        current.push_back(1);
        current.push_back(2);
        for (int j = 1; j < a; j += 1) {
            current.push_back(1);
        }

        for (int j = 1; j < b; j += 1) {
            current.push_back(2);
        }

        auto A = Ask(current.data());
        swap(current[prefix.size()], current[prefix.size() + 1]);
        auto B = Ask(current.data());

        if (A < B) {
            prefix.push_back(1);
            a -= 1;
        } else {
            prefix.push_back(2);
            b -= 1;
        }
    }

    while (a--) { prefix.push_back(1); }
    while (b--) { prefix.push_back(2); }

    return prefix;
}

void Solve(int test_number, int n, int perm[]) {
    if (test_number == 7 or test_number == 8) {
    } else {
        return;
    }

    int num[2] = {0, 0};
    for (int i = 0; i < n; i += 1) {
        num[perm[i] - 1] += 1;
    }

    GiveSolution(Solve(num[0], num[1]).data());
}
