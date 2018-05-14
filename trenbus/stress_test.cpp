#include <fstream>
#include <algorithm>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

void generate() {
    ofstream fout("trenbus.in");
    int n = 100000;
    fout << n << '\n';
    int minValue = 1, maxValue = 5;
    for (int i = 1; i < n; ++i) {
        int f = rand() % i + 1;
        int cost = rand() % (maxValue - minValue + 1) + minValue;
        fout << f << ' ' << i + 1 << ' ' << cost << '\n';
    }
    for (int i = 1; i < n; ++i) {
        int f = rand() % i + 1;
        int cost = rand() % (maxValue - minValue + 1) + minValue;
        fout << f << ' ' << i + 1 << ' ' << cost << '\n';
    }
}

void generateMaxMinMaxMerge() {
    ofstream fout("trenbus.in");
    int n = 1 << 17;
    fout << n << '\n';
    vector<int> order(n);
    for (int i = 0; i < n; ++i) {
        order[i] = i;
    }
    random_shuffle(order.begin(), order.end(), [](int n) {return rand() % n;});
    int cost = 1;
    for (int step = 1; step < n; step *= 2) {
        for (int i = 0; i + step < n; i += 2 * step) {
            fout << order[i] + 1 << ' ' << order[i + step] + 1 << ' ' << cost++ << '\n';
        }
    }
    random_shuffle(order.begin(), order.end(), [](int n) {return rand() % n;});
    for (int step = 1; step < n; step *= 2) {
        for (int i = 0; i + step < n; i += 2 * step) {
            fout << order[i] + 1 << ' ' << order[i + step] + 1 << ' ' << cost++ << '\n';
        }
    }
}

void genarateBigAnswer() {
    ofstream fout("trenbus.in");
    int n = 3 * 33333;
    fout << n << '\n';
    vector<int> order(n);
    for (int i = 0; i < n; ++i) {
        order[i] = i;
    }
    shuffle(order.begin(), order.end(), default_random_engine(rand()));
    for (int i = 0; i + 1 < n / 3; ++i) {
        fout << order[i] + 1 << ' ' << order[i + 1] << '\n';
    }
}

void generateBigAnswer1() {
    ofstream fout("trenbus.in");
    int n = 100000;
    fout << n << '\n';
    int minValue = 1000, maxValue = 100000;
    int threshold = 20;
    int val = 999;
    int cnt = 0;
    for (int i = 1; i < n; ++i) {
        int f = rand() % i + 1;
        int cost;
        if (rand() % n >= threshold) {
            cost = val;
        } else {
            cost = rand() % (maxValue - minValue + 1) + minValue;
            cnt++;
        }
        fout << f << ' ' << i + 1 << ' ' << cost << '\n';
    }
    int val1 = 998;
    for (int i = 1; i < n; ++i) {
        int f = rand() % i + 1;
        int cost;
        if (rand() % n >= threshold) {
            cost = val1;
        } else {
            cost = rand() % (maxValue - minValue + 1) + minValue;
            cnt++;
        }
        fout << f << ' ' << i + 1 << ' ' << cost << '\n';
    }
    cerr << "cnt: " << cnt << endl;
}

void test() {
    int cnt = 1;
    while (true) {
        cerr << "Generating #" << cnt++ << endl;
        generateBigAnswer1();
        system("./cmake-build-debug/numarare");
        system("cp trenbus.out got.out");
//        system("./cmake-build-debug/numarare_brut");
//        system("cp trenbus.out expected.out");
        system("./cmake-build-debug/adi");
        system("cp trenbus.out adi.out");
        ifstream fgot("got.out");
        int gotValue;
        int64_t gotCount;
        fgot >> gotValue >> gotCount;

//        ifstream fexpected("expected.out");
//        int expectedValue;
//        int64_t expectedCount;
//        fexpected >> expectedValue >> expectedCount;

        ifstream fadi("adi.out");
        int adiValue;
        int64_t adiCount;
        fadi >> adiValue >> adiCount;


        cerr << gotCount << ' ' << adiCount << endl;

//        if (make_pair(gotValue, gotCount) != make_pair(expectedValue, expectedCount)) {
//            cerr << "Wrong answer!" << endl;
//            break;
//        }
        if (make_pair(adiValue, adiCount) != make_pair(gotValue, gotCount)) {
            cerr << "Adi wrong answer!" << endl;
        }
    }
}


int main() {
    srand(time(0));
}

