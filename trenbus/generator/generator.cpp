#include "tree.h"
#include "random.h"
#include <fstream>
#include <iostream>
using namespace std;

int main(){
    ifstream f("a");
    int n, x, y;
    f >> n >> x >> y;
    cerr << n << ' ' << x << ' ' << y << endl;

    auto t = jngen::Tree::randomPrim(n, x), u = jngen::Tree::randomPrim(n, y);
    for(int i = 0; i < n-1; ++i)
        t.setEdgeWeight(i, (int)rnd.next(20) + 1);
    for(int i = 0; i < n-1; ++i)
        u.setEdgeWeight(i, (int)rnd.next(20) + 1);

    cout << t.shuffled().printN().add1() << '\n' << u.shuffled().add1() << endl;
    return 0; }
