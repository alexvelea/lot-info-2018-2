#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

ifstream f("trasee.in");
ofstream g("trasee.out");

using ll = long long;

int n, m, k;
char viz[20][20] = {};
char delta_x[2] = {}, delta_y[2] = {};
char x_jump[30] = {}, y_jump[30] = {};

bool is_good(ll conf, bool xxx = false){
    memset(viz, 0, sizeof(viz));
    int x = 0, y = 0;

    for(int i = n * m - 1; i >= 0; --i){
        if(viz[x][y]) return false;
        viz[x][y] = 1;
        x += delta_x[(conf>>i)&1];
        y += delta_y[(conf>>i)&1];
        x = x_jump[x];
        y = y_jump[y]; }
    return x == 0 && y == 0; }

int main(){
    f >> n >> m >> k;
    swap(n, m);

    for(int i = 0; i < 30; ++i)
        x_jump[i] = i % n,
        y_jump[i] = i % m;
    delta_x[0] = 1;
    delta_y[1] = 1;

    const ll lim = 1ll << (n * m);
    for(ll i = 0; i < lim; ++i){   
        if(is_good(i) && --k == 0){
            is_good(i, true);
            for(ll j = n * m - 1; j >= 0; --j){
                g << (((i>>j)&1) ? 'T' : 'B'); }
            return 0; } }
    return 0; }
