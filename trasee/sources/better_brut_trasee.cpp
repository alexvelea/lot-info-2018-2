#include <iostream>
#include <vector>
#include <cassert>
#include <fstream>
#include <cstring>
#include <algorithm>
using namespace std;

int a[25][25];

int check(int n, int m, string &now) {
    memset(a, 0, sizeof(a));
    int x = 0, y = 0;
    a[0][0] = 1;

    for(int i = 0; i < int(now.size()); ++i) {
        if(now[i] == 'B') {
            y += 1;
            if(y >= m) 
                y -= m;
        } else {
            x += 1;
            if(x >= n)
                x -= n;
        }
        if(a[x][y] == 1 and i != int(now.size() - 1)) 
            return 0; 
        if(a[x][y] == 1 and (x != 0 or y != 0))
            return 0;
        a[x][y] = 1;
    }

    return 1;
};

int main() {
    ifstream cin("trasee.in");
    ofstream cout("trasee.out");
    
    int n, m, k; cin >> n >> m >> k;
        
    int sz = min(n, m);
    int many = 0;

    for(int mask = 0; mask < (1 << sz); ++mask) {
        string temp = "";
        for(int i = 0; i < sz; ++i) {
            if((1 << i) & mask) {
                temp += 'T';
            } else {
                temp += 'B';
            }
        }

        reverse(temp.begin(), temp.end());

        string now = "";
        for(int i = 0; i < n * m / sz; ++i) {
            now += temp;
        }

        many += check(n, m, now);
        if(many == k) {
            cout << now << "\n";
            return 0;
        }
    }

    assert(false);
} 
