#include <vector>
#include <algorithm>

#include "mere.h"

using namespace std;

static const int kModulo = 1000 * 1000 * 1000 + 7;

int count(int N, int VA, int VB, int *X, int *T) {
    vector< pair<int, int> > events(N + 1, make_pair(0, 0));

    for (int i = 0; i < N; ++i) {
        events[i + 1] = make_pair(X[i], T[i]);
    }

    vector< vector<int> > dp(N + 1, vector<int>(N + 1, 0));
    dp[0][0] = 1;
    int answer = 0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (dp[i][j]) {
                int k = max(i, j) + 1;
                if (abs(events[k].first - events[i].first) <= 1LL * VA * (events[k].second - events[i].second))
                    dp[k][j] = (dp[k][j] + dp[i][j]) % kModulo;
                if (abs(events[k].first - events[j].first) <= 1LL * VB * (events[k].second - events[j].second))
                    dp[i][k] = (dp[i][k] + dp[i][j]) % kModulo;
            }

    for (int i = 0; i < N; ++i) {
        answer = (answer + dp[i][N]) % kModulo;
        answer = (answer + dp[N][i]) % kModulo;
    }

    return answer;
}
