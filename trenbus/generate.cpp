#include <fstream>
#include <algorithm>
#include <vector>
#include <set>
#include <iostream>

using namespace std;


void generate(int n, int maxValue, int c) {
    ofstream fout("trenbus.in");
    fout << c << ' ' << n << '\n';
    int minValue = 1;
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

struct Value {
    int repr, pr, size;
    bool operator<(const Value& o) const {
        return make_pair(size, pr) < make_pair(o.size, o.pr);
    }
};

void generateMaxMinMaxMerge(int n, int c) {
    ofstream fout("trenbus.in");
    fout << c << ' ' << n << '\n';
    for (int _ = 0; _ < 2; ++_) {
        set<Value> s;
        for (int i = 0; i < n; ++i) {
            s.insert({i, rand(), 1});
        }
        int val = 1;
        while (s.size() > 1) {
            Value a = *s.begin();
            s.erase(s.begin());
            Value b = *s.begin();
            s.erase(s.begin());
            fout << a.repr + 1 << ' ' << b.repr + 1 << ' ' << val++ << '\n';
            Value v = {-1, rand(), a.size + b.size};
            if (rand() % 2) {
                v.repr = a.repr;
            } else {
                v.repr = b.repr;
            }
            s.insert(v);
        }
    }
}

bool filterByCount(int c) {
    system("./cmake-build-debug/numarare");
    int value; int64_t count;
    ifstream fok("trenbus.out");
    fok >> value >> count;
    cerr << count << endl;
    return count >= c;
}


void generateBigAnswer(int n, int threshold, int minValue, int maxValue, int c) {
    ofstream fout("trenbus.in");
    fout << c << ' ' << n << '\n';
    int val = minValue - 1;
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
    int val1 = minValue - 2;
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
}

/*
 * 0 - ex1
 * 1 - ex2
 * 2 - subtask 1
 *
 * fara numarare:
 *
 * 50.000 min-max - asta ar putea disparea
 * 50.000 tamio
 * 50.000 tamio
 * 50.000 tamio
 * 50.000 random - fara filtru
 *
 * 100.000 min-max
 * 100.000 tamio
 * 100.000 tamio
 * 100.000 tamio
 * 100.000 random - fara filtru
 *
 * cu numarare:
 * 50.000 min-max
 * 50.000 tamio
 * 50.000 random
 * 50.000 cu raspuns mare, putine muchii diferite
 * 50.000 cu raspuns mediu, multe muchii diferite
 *
 * 100.000 min-max
 * 100.000 tamio
 * 100.000 random
 * 100.000 cu raspuns mare, putine muchii diferite
 * 100.000 cu raspuns mediu, multe muchii diferite
 *
 */

void run1() {
    system("./cmake-build-debug/fara_numarare");
}

void run2() {
    system("./cmake-build-debug/numarare");
}

void redo19() {
    // test 19
    system("cp ./trenbus_dummy/trenbus-7.in ./trenbus_final/19-trenbus.in");
    system("cp ./trenbus_dummy/trenbus-7.in ./trenbus.in");
    run2();
    system("cp ./trenbus.out ./trenbus_final/19-trenbus.ok");
}

int main() {
    redo19();
    return 0;
    // test 0
    system("cp ./trenbus_dummy/trenbus_example_1.in ./trenbus_final/0-trenbus.in");
    system("cp ./trenbus_dummy/trenbus_example_1.ok ./trenbus_final/0-trenbus.ok");

    // test 1
    system("cp ./trenbus_dummy/trenbus_example_2.in ./trenbus_final/1-trenbus.in");
    system("cp ./trenbus_dummy/trenbus_example_2.ok ./trenbus_final/1-trenbus.ok");

    // test 2
    generate(100, 100, 1);
    run1();
    system("cp trenbus.in ./trenbus_final/2-trenbus.in");
    system("cp trenbus.out ./trenbus_final/2-trenbus.ok");

    /// FARA NUMARARE:

    // test 3
    generateMaxMinMaxMerge(50000, 1);
    run1();
    system("cp trenbus.in ./trenbus_final/3-trenbus.in");
    system("cp trenbus.out ./trenbus_final/3-trenbus.ok");

    // test 4
    system("cp ./trenbus_dummy/trenbus-0.in ./trenbus_final/4-trenbus.in");
    system("cp ./trenbus_dummy/trenbus-0.in ./trenbus.in");
    run1();
    system("cp ./trenbus.out ./trenbus_final/4-trenbus.ok");


    // test 5
    system("cp ./trenbus_dummy/trenbus-1.in ./trenbus_final/5-trenbus.in");
    system("cp ./trenbus_dummy/trenbus-1.in ./trenbus.in");
    run1();
    system("cp ./trenbus.out ./trenbus_final/5-trenbus.ok");

    // test 6
    system("cp ./trenbus_dummy/trenbus-2.in ./trenbus_final/6-trenbus.in");
    system("cp ./trenbus_dummy/trenbus-2.in ./trenbus.in");
    run1();
    system("cp ./trenbus.out ./trenbus_final/6-trenbus.ok");

    // test 7
    generate(50000, 1000000000, 1);
    run1();
    system("cp trenbus.in ./trenbus_final/7-trenbus.in");
    system("cp trenbus.out ./trenbus_final/7-trenbus.ok");

    // test 8
    generateMaxMinMaxMerge(100000, 1);
    run1();
    system("cp trenbus.in ./trenbus_final/8-trenbus.in");
    system("cp trenbus.out ./trenbus_final/8-trenbus.ok");

    // test 9
    system("cp ./trenbus_dummy/trenbus-3.in ./trenbus_final/9-trenbus.in");
    system("cp ./trenbus_dummy/trenbus-3.in ./trenbus.in");
    run1();
    system("cp ./trenbus.out ./trenbus_final/9-trenbus.ok");


    // test 10
    system("cp ./trenbus_dummy/trenbus-4.in ./trenbus_final/10-trenbus.in");
    system("cp ./trenbus_dummy/trenbus-4.in ./trenbus.in");
    run1();
    system("cp ./trenbus.out ./trenbus_final/10-trenbus.ok");

    // test 11
    system("cp ./trenbus_dummy/trenbus-5.in ./trenbus_final/11-trenbus.in");
    system("cp ./trenbus_dummy/trenbus-5.in ./trenbus.in");
    run1();
    system("cp ./trenbus.out ./trenbus_final/11-trenbus.ok");

    // test 12
    generate(100000, 1000000000, 1);
    run1();
    system("cp trenbus.in ./trenbus_final/12-trenbus.in");
    system("cp trenbus.out ./trenbus_final/12-trenbus.ok");

    /// CU NUMARARE:

    // test 13
    generateMaxMinMaxMerge(50000, 2);
    run2();
    system("cp trenbus.in ./trenbus_final/13-trenbus.in");
    system("cp trenbus.out ./trenbus_final/13-trenbus.ok");

    // test 14
    system("cp ./trenbus_dummy/trenbus-6.in ./trenbus_final/14-trenbus.in");
    system("cp ./trenbus_dummy/trenbus-6.in ./trenbus.in");
    run2();
    system("cp ./trenbus.out ./trenbus_final/14-trenbus.ok");


    // test 15
    do {
        generate(50000, 100, 2);
    } while (!filterByCount(2));
    run2();
    system("cp trenbus.in ./trenbus_final/15-trenbus.in");
    system("cp trenbus.out ./trenbus_final/15-trenbus.ok");

    // test 16
    generateBigAnswer(50000, 20, 1000, 50000, 2);
    run2();
    system("cp trenbus.in ./trenbus_final/16-trenbus.in");
    system("cp trenbus.out ./trenbus_final/16-trenbus.ok");

    // test 17
    generateBigAnswer(50000, 5000, 1000, 50000, 2);
    run2();
    system("cp trenbus.in ./trenbus_final/17-trenbus.in");
    system("cp trenbus.out ./trenbus_final/17-trenbus.ok");

    // test 18
    generateMaxMinMaxMerge(100000, 2);
    run2();
    system("cp trenbus.in ./trenbus_final/18-trenbus.in");
    system("cp trenbus.out ./trenbus_final/18-trenbus.ok");

    // test 19
    system("cp ./trenbus_dummy/trenbus-7.in ./trenbus_final/19-trenbus.in");
    system("cp ./trenbus_dummy/trenbus-7.in ./trenbus.in");
    run1();
    system("cp ./trenbus.out ./trenbus_final/19-trenbus.ok");

    // test 20
    generate(100000, 10000, 2);
    run2();
    system("cp trenbus.in ./trenbus_final/20-trenbus.in");
    system("cp trenbus.out ./trenbus_final/20-trenbus.ok");


    // test 21
    generateBigAnswer(100000, 20, 1000, 100000, 2);
    run2();
    system("cp trenbus.in ./trenbus_final/21-trenbus.in");
    system("cp trenbus.out ./trenbus_final/21-trenbus.ok");

    // test 22
    generateBigAnswer(100000, 10000, 1000, 100000, 2);
    run2();
    system("cp trenbus.in ./trenbus_final/22-trenbus.in");
    system("cp trenbus.out ./trenbus_final/22-trenbus.ok");
}