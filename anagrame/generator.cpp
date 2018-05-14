#include <tumbletest>
#include <generator_utils>

#include <algorithm>
#include <iostream>
#include <string>
using namespace std;

enum InputType {
    SORT,
    DIST_2,
    PERM_LIGHT,
    ANAGRAM_LIGHT,
    PERM_TIGHT,
    ANAGRAM_TIGHT,
};

struct TestConfig {
    static int current_test;
    
    int test_number;
    InputType type;
    int n;
    int num_queries;

    TestConfig(InputType type, int n, int num_queries) :
        test_number(current_test++), type(type), n(n), num_queries(num_queries) {
    }
};

int TestConfig::current_test = 0;

struct Input {
    int test_number;
    InputType type;
    int num_queries;
    vector<int> els;

    Input(TestConfig c) : test_number(c.test_number), type(c.type), num_queries(c.num_queries), els(c.n) {
    }

    void Asserts() const {
        int n = els.size();
        
        if (type == PERM_LIGHT or type == PERM_TIGHT or type == SORT) {
            map<int, int> visited;
            for (auto& itr : els) {
                assert(1 <= itr and itr <= n);
                assert(visited[itr] == 0);
                visited[itr] = 1;
            }
        } else if (type == ANAGRAM_LIGHT or type == ANAGRAM_TIGHT) {
            auto aux = els;
            Sort(aux);
            assert(aux[0] == 1);
            for (int i = 1; i < (int)els.size(); i += 1) {
                assert(aux[i] - aux[i - 1] <= 1);
            }
        } else if (type == DIST_2) {
            map<int, int> viz;
            for (auto itr : els) {
                viz[itr] = 1;
            }
            assert(viz[1] and viz[2] and viz.size() == 2);
        }

        if (type == PERM_TIGHT or type == ANAGRAM_TIGHT) {
            assert(num_queries == n);
        }

        if (type == PERM_LIGHT or type == ANAGRAM_LIGHT or type == DIST_2) {
            assert(num_queries == 2 * n);
        }
    }
};

string Stringify(const Input& x) {
    x.Asserts();

    string result = StrCat(x.els.size(), ' ', x.num_queries, ' ', x.test_number, '\n');
    for (auto itr : x.els) {
        result += StrCat(itr, ' ');
    }
    result.pop_back();
    result += "\n";

    return result;
}


vector<int> NicePartition(int n, int num_els = -1, int mn_val = 1) {
    if (num_els != -1) {
        n -= num_els * mn_val;
    }
    assert(n >= 0);
    vector<function<int(int)>> gen_functions = {
        [](int n) { return n; },
        [](int n) { return n; },
        [](int n) { return max(1, int(n / log2(n + 2))); },
        [](int n) { return int(sqrt(n) + 0.5); },
        [](int n) { return int(log2(n + 2)); },
        [](int  ) { return 1; },
    };

    vector<int> p;
    while (n) {
        long double bonus = 1.0;
        int N = n;
        if (n > 1e7) {
            N = 1e7;
            bonus = 1.0 * n / N;
        }

        int found = 0;
        vector<int> nums;
        vector<long long> chxs;
        for (const auto& itr : gen_functions) {
            int x = rand() % itr(N) + 1;
            nums.push_back(x);
            chxs.push_back(1e15 / x);
        }
        long long sum = 0;
        for (int i = 0; i < (int)nums.size(); i += 1) {
            sum += chxs[i];
        }

        long long c = Rand<long long>(0, sum);

        for (int i = 0; i < (int)nums.size(); i += 1) {
            if (chxs[i] > c) {
                found = nums[i];
                break;
            } else {
                c -= chxs[i];
            }
        }
    
        found *= bonus;
        n -= found;
        p.push_back(found);
    }

    if (num_els != -1) {
        while ((int)p.size() < num_els) {
            p.push_back(0);
        }

        RandomShuffle(p);

        while ((int)p.size() > num_els) {
            p[rand() % num_els] += p.back();
            p.pop_back();
        }
        for (auto& itr : p) {
            itr += mn_val;
        }
    }

    RandomShuffle(p);
    return p;
}

string Fixed(TestConfig config, vector<int> els) {
    config.n = els.size();
    
    Input x(config);
    x.els = els;

    return Stringify(x);
}

string Perm(TestConfig config) {
    Input x(config);
    int n = config.n;

    x.els = xRange(1, n);
    RandomShuffle(x.els);

    return Stringify(x);
}

string TwoEls(TestConfig config) {
    Input x(config);
    int n = config.n;

    int num_a = xRand(0, n);
    x.els = vector<int>(num_a, 1) + vector<int>(n - num_a, 2);
    RandomShuffle(x.els);

    return Stringify(x);
}

string Anagram(TestConfig config) {
    Input x(config);
    int n = config.n;

    auto p = NicePartition(n);
    RandomShuffle(p);

    map<int, int> viz;
    for (auto itr : p) {
        viz[itr] += 1;
    }

    vector<int> els;
    for (int i = 0; i < (int)p.size(); i += 1) {
        for (int j = 0; j < p[i]; j += 1) {
            els.push_back(i + 1);
        }
    }

    RandomShuffle(els);
    x.els = els;

    return Stringify(x);
}

int main() {
    // 0     - Example              - 0 points 
    addTest(Fixed({SORT, 4, 4*3*2*1}, {3, 2, 1, 4}));
    // 1|2   - std::sort            - 7 points
    addTest(Perm({SORT, 1 << 4, (1 << 8)}));
    addTest(Perm({SORT, 1 << 5, (1 << 10)}));
    // 3|4   - std::stable sort     - 6 points
    addTest(Perm({SORT, 1 << 5, (1 << 5) * 5 * 2}));
    addTest(Perm({SORT, 1 << 6, (1 << 6) * 6 * 2}));
    // 5|6   - merge_sort + smart   - 14 points
    addTest(Perm({SORT, 1 << 6, (1 << 6) * 6}));
    addTest(Perm({SORT, 1 << 7, (1 << 7) * 7}));
    // 7|8   - 2 elemenents         - 11 points
    addTest(TwoEls({DIST_2, 1 << 9, 1 << 10})); 
    addTest(TwoEls({DIST_2, 1 << 10, 1 << 11})); 
    // 9|10  - 2 * N perm           - 19 points
    addTest(Perm({PERM_LIGHT, 1 << 9, 1 << 10}));
    addTest(Perm({PERM_LIGHT, 1 << 10, 1 << 11}));
    // 11|12 - 2 * N Anagram        - 18 points
    addTest(Anagram({ANAGRAM_LIGHT, 1 << 9, 1 << 10}));   
    addTest(Anagram({ANAGRAM_LIGHT, 1 << 10, 1 << 11}));   
    // 13|14 - N perm               - 11 points
    addTest(Perm({PERM_TIGHT, 1 << 9, 1 << 9}));
    addTest(Perm({PERM_TIGHT, 1 << 10, 1 << 10}));
    // 15|16 - N perm Anagram       - 14 points
    addTest(Anagram({ANAGRAM_TIGHT, 1 << 9, 1 << 9}));
    addTest(Anagram({ANAGRAM_TIGHT, 1 << 10, 1 << 10}));

//    TestSources(10, {"official.cpp"});
     RunTumbletest();
//    TestSources(200, {"official.cpp", "popa_sort_nlogn.cpp", "popa_2n.cpp", "just_permutations.cpp", "velea_nlogn_optimizat.cpp"});
//    TestSources(0, {"bc_gauss.cc"});
    return 0;
}
