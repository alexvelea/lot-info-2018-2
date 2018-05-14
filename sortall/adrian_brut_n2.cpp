#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

static const int kModulo = 1000 * 1000 * 1000 + 7;

int main() {
    ifstream cin("sortall.in");
    ofstream cout("sortall.out");

    int N; cin >> N;
    vector<int> V(N);
    for (int i = 0; i < N; ++i)
        cin >> V[i];
    vector<int> last(N, -1);
    vector<int> where(N + 1, -1);
    for (int i = 0; i < N; ++i) {
        last[i] = where[V[i]];
        where[V[i]] = i;
    }

    int answer = 0;
    for (int i = 0; i < N; ++i) {
        int64_t aux = 0;
        for (int j = 0; j < N; ++j)
            if (V[i] < V[j]) {
                int min_begin = max(last[i], last[j]) + 1;
                int max_begin = min(i, j);
                int min_end = max(i, j);

                if (min_begin > max_begin)
                    continue;
                aux += static_cast<int64_t>(max_begin - min_begin + 1) * (N - min_end) * V[j];
            }
        answer = (answer + aux) % kModulo;
    }

    for (int i = 0; i < N; ++i) {
        int min_begin = last[i] + 1;
        int max_begin = i;
        int min_end = i;

        answer = (answer + static_cast<int64_t>(max_begin - min_begin + 1) * (N - min_end) % kModulo * V[i]) % kModulo;
    }

    cout << answer << "\n";
}
