#include <algorithm>
#include <fstream>
#include <vector>
using namespace std;

int gcd(int a, int b) {
    while (b) {
        int c = a % b;
        a = b;
        b = c;
    }
    return a;
}

int main() {
    ifstream fin("trasee.in");
    ofstream fout("trasee.out");

    int n, m;
    int64_t k;
    fin >> n >> m >> k;
    k--;

    int len = gcd(n, m);
    vector<vector<int64_t>> comb(len + 1, vector<int64_t>(len + 1, 0));
    for (int i = 0; i <= len; ++i) {
        comb[i][0] = 1;
        for (int j = 1; j <= i; ++j) {
            comb[i][j] = min((int64_t) 1e18, comb[i - 1][j - 1] + comb[i - 1][j]);
        }
    }
    vector<int> good;
    for (int i = 0; i <= len; ++i) {
        if (gcd(i, m) == 1 && gcd(len - i, n) == 1) {
            good.push_back(i);
        }
    }
    string ans;
    int b = 0, t = 0;
    for (int i = 0; i < len; ++i) {
        int64_t cnt = 0;
        int cb = b + 1;
        for (int B: good) {
            if (cb > B) {
                continue;
            }
            cnt = min((int64_t) 1e18, cnt + comb[len - t - cb][B - cb]);
        }
        if (cnt <= k) {
            k -= cnt;
            ans += 'T';
            t++;
        } else {
            ans += 'B';
            b++;
        }
    }
    string fans;
    for (int i = 0; i < n * m / len; ++i) {
        fans += ans;
    }
    fout << fans << '\n';
}