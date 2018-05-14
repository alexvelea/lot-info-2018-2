#include <bits/stdc++.h>
#ifdef TUMBLETEST
#include "batch.hpp"
#endif
using namespace std;

const int SAFE_ONE = 118153912;
const int SAFE_TWO = 598692164;

void result(string msg, int p) {
#ifndef TUMBLETEST
    printf("%d %s", p, msg.c_str());
    exit(0);
#else
    if (p == 0) {
        WA(msg);
    } else {
        OK(msg);
    }
#endif
}

enum Status { FINISH_OK, QUERY_LIMIT_KILL, WRONG_INTERACTION, NO_ANSWER };

int main(int argc, char **argv) {
#ifdef TUMBLETEST
    Init(argc, argv); // keep this
#else
    ifstream in("anagrame.in");
    ifstream ok("anagrame.ok");
    ifstream out("anagrame.out");
#endif

    assert(in);
    assert(ok);
    assert(out);

    int safety;
    string msg;
    assert(out >> safety);

    if(safety != (SAFE_ONE ^ SAFE_TWO)) {
        result("Eroare de securitate", 0);
    }

    int s = 0; assert(out >> s);
    int num_calls = 0; assert(out >> num_calls);

    int n; in >> n;
    int max_ask_calls; in >> max_ask_calls;
    int test_number; in >> test_number;

    if (s == QUERY_LIMIT_KILL) {
        result("> " + to_string(num_calls) + " (max " + to_string(max_ask_calls) + ") apeluri ale functiei ask", 0);
    }

    if (s == WRONG_INTERACTION) {
        result("Interactiune gresita! Sirul nu este o anagrama", 0);
    }

    if (s == NO_ANSWER) {
        result("Nici un raspuns setat", 0);
    }

    if (num_calls > max_ask_calls) {
        result(to_string(num_calls) + " (max " + to_string(max_ask_calls) + ") apeluri ale functiei ask", 0);
    }

    for (int i = 0; i < n; i += 1) {
        int a, b; 
        in >> a;
        out >> b;
        if (a != b) {
            result("Raspuns gresit! " + to_string(num_calls) + " (max " + to_string(max_ask_calls) + ") apeluri ale functiei ask", 0);
        }
    }

#ifdef TUMBLETEST
    OK("Corect! " + to_string(num_calls) + " intrebari folosite");
#else
    result("Corect! " + to_string(num_calls) + " intrebari folosite", stoi(argv[1]));
#endif

    return 0;
}
