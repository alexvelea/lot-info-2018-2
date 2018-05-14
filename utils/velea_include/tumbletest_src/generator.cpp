#include "tumbletest.hpp"
#include "vector_utils.hpp"
#include "string_utils.hpp"

#include <algorithm>
#include <iostream>
#include <string>
using namespace std;

string Stringify(int x) {
    return StrCat(x, '\n');
}

string Rand() { return Stringify(rand()); }

int main() {
    addTest(Rand());
    addTest(Rand());
    addTest(Rand());
    addTest(Rand());
    RunTumbletest();
    TestSources(1, "official.cpp");
    return 0;
}
