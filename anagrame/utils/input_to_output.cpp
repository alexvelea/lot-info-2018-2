#include <fstream>
using namespace std;

int main() {
    ifstream fin("anagrame.in");
    ofstream fout("anagrame.out");

    int n;
    fin >> n;

    for (int i = 0; i < n; ++i) {
        int x;
        fin >> x;
        fout << x << ' ';
    }
    fout << '\n';
}
