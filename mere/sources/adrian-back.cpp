#include <vector>
#include <algorithm>

#include "mere.h"

using namespace std;

int abs(int x) {
    if (x < 0)
        return -x;
    return x;
}

bool can_reach(pair<int, int> a, pair<int, int> b, int speed) {
    return abs(a.first - b.first) <= 1LL * (b.second - a.second) * speed;
}

int count(int N, int VA, int VB, int *X, int *T) {
    vector< pair<int, int> > events(N + 1, make_pair(0, 0));
    for (int i = 0; i < N; ++i)
        events[i + 1] = make_pair(X[i], T[i]);

    int answer = 0;
    for (int i = 0; i < (1 << N); ++i) {
        int last_x = 0;
        int last_y = 0;
        bool ok = true;
        for (int j = 0; j < N; ++j)
            if ((1 << j) & i) {
                ok = ok && can_reach(events[last_x], events[j + 1], VA);
                last_x = j + 1;
            } else {
                ok = ok && can_reach(events[last_y], events[j + 1], VB);
                last_y = j + 1;
            }
        if (ok)
            ++answer;
    }
    return answer;
}
