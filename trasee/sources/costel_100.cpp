#include <bits/stdc++.h>

using namespace std;

typedef long long int lint;

const int PRODMAX = 1000000;
const lint INF = 1000000000000000000LL;

inline int gcd(int a, int b) {
    int r = a % b;
    while (r)
        a = b, b = r, r = a % b;
    return b;
}

int main() {
    ifstream cin("trasee.in");
    ofstream cout("trasee.out");

    int N, M;
    lint K;
    cin >> N >> M >> K;

    assert(1 <= N && 1 <= M && 1 <= 1LL * N * M && 1LL * N * M <= PRODMAX);

    const int period = gcd(N, M);
    vector <vector <lint> > dp(N + 1, vector <lint>(M + 1, 0));

    for (int i = N; i >= 0; -- i)
        for (int j = M; j >= 0; -- j)
            if (i + j <= period) {
                if (i + j == period)
                    dp[i][j] = (gcd(i, N) == 1 && gcd(j, M) == 1);
                else {
                    dp[i][j] = dp[i + 1][j] + dp[i][j + 1];
                    if (dp[i][j] > INF)
                        dp[i][j] = INF;
                }
            }

    assert(1 <= K && K <= dp[0][0]);

    string sol;
    for (int i = 0, l = 0, c = 0; i < N * M; ++ i)
        if (i < period) {
            if (K <= dp[l][c + 1])
                sol += "B", ++ c;
            else
                K -= dp[l][c + 1], sol += "T", ++ l;
        }
        else
            sol += sol[i % period];
    cout << sol << endl;
    return 0;
}
