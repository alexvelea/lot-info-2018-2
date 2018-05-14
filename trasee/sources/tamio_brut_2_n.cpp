#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

ifstream f("trasee.in");
ofstream g("trasee.out");

using ll = long long;

ll len;
int n, m, k;
char viz[100][100] = {};
char delta_x[2] = {}, delta_y[2] = {};
char x_jump[200] = {}, y_jump[200] = {};

bool is_good(ll conf, bool xxx = false){
    memset(viz, 0, sizeof(viz));
    int x = 0, y = 0;

    for(int i = n * m - 1; i >= 0; --i){
        if(viz[x][y]) return false;
        viz[x][y] = 1;
        x += delta_x[(conf>>(i%len))&1];
        y += delta_y[(conf>>(i%len))&1];
        x = x_jump[x];
        y = y_jump[y]; }
    return x == 0 && y == 0; }

int gcd(int x, int y){
    return x == 0 ? y : gcd(y % x, x); }

int main(){
    f >> n >> m >> k;
    swap(n, m);

    for(int i = 0; i < 100; ++i)
        x_jump[i] = i % n,
        y_jump[i] = i % m;
    delta_x[0] = 1;
    delta_y[1] = 1;

    len = gcd(n, m);
    const ll lim = 1 << len;

    for(ll i = 0; i < lim; ++i){   
        if(is_good(i, true) && --k == 0){
            is_good(i, true);
            for(ll j = n * m - 1; j >= 0; --j){
                g << (((i>>(j%len))&1) ? 'T' : 'B'); }
            return 0; } }
    return 0; }
