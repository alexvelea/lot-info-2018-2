#include <vector>
#include <algorithm>

#include "mere.h"

static const int kModulo = 1000 * 1000 * 1000 + 7;

using namespace std;

int abs(int x) {
    if (x < 0)
        return -x;
    return x;
}

bool can_reach(pair<int, int> event1, pair<int, int> event2, int speed) {
    return static_cast<int64_t>(event2.second - event1.second) * speed >= abs(event2.first - event1.first);
}

int count(int N, int VA, int VB, int *X, int *T) {
    vector< pair<int, int> > events;
    events.emplace_back(0, 0);
    for (int i = 0; i < N; ++i)
        events.emplace_back(X[i], T[i]);

    events.emplace_back(0, 2 * 1000 * 1000 * 1000);
    N += 2;

    vector<int> dp[2];
    dp[0] = dp[1] = vector<int>(events.size(), 0);
    vector<int> speed = {VA, VB};

    for (int i = 0; i < 2; ++i)
        if (can_reach(events[0], events[1], speed[i]))
            dp[i][1] = 1;

    for (int i = 2; i < N; ++i) {
        for (int k = 0; k < 2; ++k) {
            for (int j = i - 1; j > 0; --j) {
                if (can_reach(events[j - 1], events[i], speed[k])) {
                    dp[k][i] += dp[1 - k][j];
                    if (dp[k][i] >= kModulo)
                        dp[k][i] -= kModulo;
                }
                if (!can_reach(events[j - 1], events[j], speed[1 - k])) {
                    break;
                }
            }
        }
    }

    return (dp[0].back() + dp[1].back()) % kModulo;
}
