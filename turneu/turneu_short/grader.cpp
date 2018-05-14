#include <bits/stdc++.h>
#include "turneu.h"
using namespace std;

namespace __comisia__ {

const int kBufferSize = 1e4;
int bufferInd = kBufferSize;
char buffer[kBufferSize];

ifstream cin("turneu.in");
ofstream cout("turneu.out");

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

template <typename number>
void readNumber(number& a) {
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
    return;
}

void comisie_main() {
    int n;
    readNumber(n);
    vector<int> els(n);
    for (auto& itr : els) {
        readNumber(itr);
    }

    init(n, &(els[0]));

    int q;
    readNumber(q);
    while (q--) {
        int x, y;
        readNumber(x);
        readNumber(y);
        cout << query(x, y) << '\n';
    }
}
}

int main() {
    __comisia__::comisie_main();
    return 0;
}

