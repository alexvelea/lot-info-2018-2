#include <iostream>
#include <fstream>
#include <cassert>
using namespace std;

int main(){
    ifstream fout("trenbus.out");
    ifstream fok("trenbus.ok");

    int points = 0;
    fok >> points;
    int x, y;
    fout >> x;
    fok >> y;

    if(x != y) cerr << "WA" << endl, cout << 0 << endl;
    if(x == y){
        cerr << "OK" << endl;
        cout << points << endl; }
    if(x < y) cout << "MINES BETTER";
    return 0; }
