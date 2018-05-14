#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <random>
#define SZ(x) ((int) (x).size())
using namespace std;

vector<int> norm(vector<int> a) {
    vector<int> b = a;
    sort(b.begin(), b.end());
    b.erase(unique(b.begin(), b.end()), b.end());
    for (int& x: a) {
        x = lower_bound(b.begin(), b.end(), x) - b.begin();
        ++x;
    }
    return a;
}

int main() {
    srand(time(0));
    int step = 0;
    while (true) {
        cerr << "Generating #" << ++step << endl;
        int n = 30;
        int valMin = 1, valMax = 10;
        vector<int> a(n);
        for (int i = 0; i < n; ++i) {
            a[i] = rand() % (valMax - valMin + 1) + valMin;
        }
        a = norm(a);
        {
            ofstream fout("anagrame.in");
            fout << n << '\n';
            for (int x: a) {
                fout << x << ' ';
            }
            fout << '\n';
            fout.close();
            system("cat anagrame.in");
        }
        system("./cmake-build-debug/input_to_output");
        system("cp anagrame.out expected.out");
        system("./cmake-build-debug/legit");
        system("cp anagrame.out got.out");
        if (system("diff -b expected.out got.out") != 0) {
            cerr << "Wrong answer!" << endl;
            break;
        }
    }
}
