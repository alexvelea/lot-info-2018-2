#include "anagrame.h"
#include <cassert>
#include <algorithm>
using namespace std;

int arr[20000];

void Solve(int, int n, int perm[]) {
    random_shuffle(perm, perm + n);
    for (int i = 0; i < 20000; i += 1) {
        arr[i] = perm[i % n];
    }

    for (int i = 0; i < 20000; i += 1) {
        Ask(arr + i);
    }
}
