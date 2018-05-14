#ifndef ANAGRAME_MAIN
#define ANAGRAME_MAIN

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include "anagrame.h"

#ifndef __linux__
#include <windows.h>
#endif

using namespace std;

namespace comisia {

int n;
vector<int> perm;

int num_queries;
bool got_solution;

vector<int> ReadData() {
    ifstream cin("anagrame.in");
    ofstream cout("anagrame.out");

    cin >> n;
    vector<int> perm(n);
    for (auto &itr : perm) {
        cin >> itr;
    }

    return perm;
}

void StartInteraction(vector<int> p) {
    // set global data
    n = p.size();
    perm = p;

    num_queries = 0;
    got_solution = false;
    
    // pass data to contestant
    int *contestant_arr = new int[n];
    for (int i = 0; i < n; i += 1) {
        contestant_arr[i] = perm[i];
    }

    // sort the permutation
    sort(contestant_arr, contestant_arr + n);
    
    // perform sanity check for input!
    if (contestant_arr[0] != 1) {
        cerr << "Sirul dat nu respecta proprietatiile din enunt!\n";
        for (int i = 0; i < n; i += 1) { cerr << contestant_arr[i] << '\t'; } cerr << '\n';
        exit(1);
    }

    for (int i = 1; i < n; i += 1) {
        if (contestant_arr[i] - contestant_arr[i - 1] > 1) {
            cerr << "Sirul dat nu respecta proprietatiile din enunt!\n";
            for (int i = 0; i < n; i += 1) { cerr << contestant_arr[i] << '\t'; } cerr << '\n';
            exit(1);
        }
    }

    Solve(-1, n, contestant_arr);

    if (not got_solution) {
        cerr << "Nici un raspuns dat!\n";
        exit(1);
    }
}

void Main() {
    StartInteraction(ReadData());
}

int Ask(const int *arr) {
    vector<int> c_arr(arr, arr + comisia::n);
    // check if the array is an anagram
    vector<int> viz_ok(n + 1);
    vector<int> viz_c(n + 1);

    for (int i = 0; i < n; i += 1) {
        viz_c[c_arr[i]] += 1;
        viz_ok[perm[i]] += 1;
    }

    if (viz_c != viz_ok) {
        cerr << "Sirul de query nu este o anagrama a sirului din input\n";
        cerr << "+ Sir input:    "; for (auto itr : perm) { cerr << itr << '\t'; } cerr << '\n';
        cerr << "- Sir de query: "; for (auto itr : c_arr) { cerr << itr << '\t'; } cerr << '\n';
        exit(1);
    }

    num_queries += 1;

    int ans = 0;
    for (int i = 0; i < n; i += 1) {
        // looking for perm[i]
        int where = i;
        while (c_arr[where] != perm[i]) {
            where += 1;
        }

        while (where > i) {
            swap(c_arr[where], c_arr[where - 1]);
            ans += 1;
            where -= 1;
        }
    }

    return ans;
}
};

int Ask(int arr[]) {
    return comisia::Ask(arr);
}

void GiveSolution(int arr[]) {
    
#ifndef __linux__
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

    vector<int> contestant_ans(arr, arr + comisia::n);
    if (contestant_ans == comisia::perm) {
#ifdef __linux__
        cerr << "\033[22;32mOK!\033[0m ";
#else
        SetConsoleTextAttribute(hConsole, 2);
        cerr << "OK! ";
        SetConsoleTextAttribute(hConsole, 7);
#endif
        cerr << comisia::num_queries << " queriuri folosite\n";
    } else {
#ifdef __linux__
        cerr << "\033[22;31mGresit!\033[0m ";
#else
        SetConsoleTextAttribute(hConsole, 4);
        cerr << "Gresit! ";
        SetConsoleTextAttribute(hConsole, 7);
#endif

        cerr << comisia::num_queries << " queriuri folosite\n";
        cerr << "Raspuns corect: "; for (auto itr : comisia::perm) { cerr << itr << '\t'; } cerr << '\n';
        cerr << "Raspuns primit: "; for (auto itr : contestant_ans) { cerr << itr << '\t'; } cerr << '\n';
    }

    comisia::got_solution = true;
}

void CustomRun(const vector<int>& els) {
    comisia::StartInteraction(els);
}

int main() {
    comisia::Main();

//     CustomRun({1, 2, 3, 4});

    return 0;
}

#endif // ANAGRAME_MAIN
