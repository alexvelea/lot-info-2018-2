#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

int n, m, k;
int have[20][20];
int many = 0;
int all = 0;
string now;

int moveDown(int x) {
    x += 1;
    if(x >= n)
        x -= n;
    return x;
}

int moveRight(int x) {
    x += 1;
    if(x >= m)
        x -= m;
    return x;
}


void rec(int x, int y, int step, string &ans) {
    if(step == n * m and x == 0 and y == 0) {
        many += 1;
        if(many == k) {
            ans = now;
        }
        return;
    }

    if(have[x][y])
        return;
    
    have[x][y] = 1;

    now[step] = 'B';
    rec(x, moveRight(y), step + 1, ans);
    if(ans != "")
        return;
    now[step] = 'T';
    rec(moveDown(x), y, step + 1, ans);

    have[x][y] = 0;
}

int main() {
    ifstream cin("trasee.in");
    ofstream cout("trasee.out");
    
    cin >> n >> m >> k;
    
    string ans = "";
    now = string(n * m, 'x');
    rec(0, 0, 0, ans);

    cout << ans << "\n";
}
