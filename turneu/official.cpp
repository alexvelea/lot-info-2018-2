/*
 *  Alex Velea
 *  Timp: O(NlogN + QlogN)
 *  Mem:  O(N)
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

const int kMaxPow = 20;

int* order;
int n, p;

int best_win[1 << kMaxPow];
vector<int> min_el_to_win[kMaxPow];

// init how many things a person would win if there are no swaps
void InitBestWin() {
vector<int> current_order(n);
    for (int i = 0; i < n; i += 1) {
        current_order[i] = order[i];
    }

    for (int step = 0; step < p; step += 1) {
        for (int i = 0; i < (1 << (p - step)); i += 2) {
            if (current_order[i] > current_order[i + 1]) {
                best_win[current_order[i]] = step + 1;
                current_order[i / 2] = current_order[i];
            } else {
                best_win[current_order[i + 1]] = step + 1;
                current_order[i / 2] = current_order[i + 1];
            }
        }
    }
}

void InitTree() {
vector<int> current_order(n);
    for (int i = 0; i < n; i += 1) {
        current_order[i] = order[i];
    }
    
    vector<int> merge_result(n);
    for (int step = 0; step < p; step += 1) {
        min_el_to_win[step] = vector<int>(1 << step, n + 1);
    }

    for (int step = 0; step < p; step += 1) {
        int l = (1 << step);
        for (int bucket = 0; bucket < (1 << (p - step)); bucket += 1) {
            int first_el = bucket * l;
            for (int i = 1; i < l; i += 1) {
                min_el_to_win[step][l - i] = min(min_el_to_win[step][l - i], current_order[first_el + i - 1]);
            }
        }

        // merge the things
        for (int bucket = 0; bucket < (1 << (p - step)); bucket += 2) {
            int first_el = bucket * l;
            std::merge(current_order.begin() + first_el, current_order.begin() + first_el + l,
                    current_order.begin() + first_el + l, current_order.begin() + first_el + 2 * l,
                    merge_result.begin());

            for (int i = 0; i < 2 * l; i += 1) {
                current_order[first_el + i] = merge_result[i];
            }
        }
    }
}

void Init() {
    while ((1 << p) < n) {
        p += 1;
    }
    
    InitBestWin();
    InitTree();    
}

void SetPermutation(int _n, int* _perm) {
    order = _perm;
    n = _n;

    Init();
}

int Solve(int x, int k) {
    k = min(k, x);

    int ans = best_win[x];
    for (int num_win = 1; num_win < p and x >= (1 << num_win) - 1; num_win += 1) {
        if (min_el_to_win[num_win][min(k, (1 << num_win) - 1)] <= x) {
            ans = max(ans, num_win);
        }
    }

    return ans;
}

const int kBufferSize = 1e4;
int bufferInd = kBufferSize;
char buffer[kBufferSize];

void nextChar() {
    if (++bufferInd >= kBufferSize) {
        cin.read(buffer, kBufferSize);
        bufferInd = 0;
    }
}

bool valid(char c) {
    return (c == '-' or ('0' <= c and c <= '9'));
}

char currentChar() {
    if (bufferInd > kBufferSize) {
        nextChar();
    }
    return buffer[bufferInd];
}

template<typename number>
void readNumber(number &a) {
    a = 0;
    bool signedNumber = false;
    if (currentChar() == '-') {
        signedNumber = true;
    }

    for (; not valid(currentChar()); nextChar())
        ;

    for (; valid(currentChar()); nextChar()) {
        a *= 10;
        a += currentChar() - '0';
    }

    if (signedNumber) {
        a *= -1;
    }
    return ;
}

void Mock() {
    int n; readNumber(n);
    vector<int> read_order(n);
    for (auto& itr : read_order) {
        readNumber(itr);
    }

    SetPermutation(n, &read_order[0]);

    int q; readNumber(q);
    while (q--) {
        int x, k; readNumber(x); readNumber(k);
        cout << Solve(x, k) << '\n';
    }
}

int main() {
    Mock();   
    return 0;
}
