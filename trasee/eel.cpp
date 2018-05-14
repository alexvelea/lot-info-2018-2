#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

static const int64_t kInfinite = 1000LL * 1000 * 1000 * 1000  * 1000 * 1000;

int gcd(int x, int y) {
    if (y == 0)
        return x;
    return gcd(y, x % y);
}

int main() {
    int N, M;
    assert(cin >> N >> M);
    assert(1 <= N && N <= 1000 * 1000);
    assert(1 <= M && M <= 1000 * 1000);
    assert(1LL * N * M <= 1000LL * 1000);

    vector< vector<int> > index(N, vector<int>(M, -1));
    index[0][0] = 0;
    for (int x = M - 1, y = 1 % N, k = 1; x != 0 || y != 0; x = (x + M - 1) % M, y = (y + 1) % N)
        index[y][x] = k++;

    vector< vector<int64_t> > dp(N, vector<int64_t>(M, 0));
    int group = N * M / gcd(N, M);
    if (N == 1 && M == 1)
        dp[0][0] = 1;
    for (int i = N - 1; i >= 0; --i)
        for (int j = M - 1; j >= 0; --j) {
            if (index[i][j] <= 0) {
                dp[i][j] = dp[(i + 1) % N][j] + dp[i][(j + 1) % M];
                if (dp[i][j] > kInfinite)
                    dp[i][j] = kInfinite;
            } else if (gcd(index[i][j], group) == 1)
                dp[i][j] = 1;
        }

    cout << dp[0][0] << "\n";
}
