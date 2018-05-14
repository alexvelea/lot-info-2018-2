#include <tumbletest>
#include <generator_utils>

#include <algorithm>
#include <iostream>
#include <string>
using namespace std;

struct Query {
    int x, k;
};

struct Input {
    int p;
    int n;
    vector<int> els;
    vector<Query> queries;

    void Asserts() const {
        assert(1 <= p and p <= 20);
        assert((1 << p) == n);

        vector<int> visited(n);
        assert((int)els.size() == n);
        for (auto itr : els) {
            visited[itr] = true;
        }

        for (auto& itr : visited) { assert(itr); }

        assert(1 <= (int)queries.size() and (int)queries.size() <= 5e5);
        for (auto itr : queries) {
            assert(0 <= itr.x and itr.x < n);
            assert(0 <= itr.k and itr.k < n);
        }
    }
};

string Stringify(Input x) {
    x.Asserts();
    string result = StrCat(1 << x.p, '\n');
    for (auto itr : x.els) {
        result += StrCat(itr, ' ');
    } result.pop_back(); result += "\n";

    result += StrCat(x.queries.size(), '\n');
    for (auto itr : x.queries) {
        result += StrCat(itr.x, ' ', itr.k, /*' ', itr.ans, */ '\n');
    }

    return result;
}

vector<int> NicePartition(int n, int num_els = -1, int mn_val = 1) {
    if (num_els != -1) {
        n -= num_els * mn_val;
    }
    assert(n >= 0);
    vector<function<int(int)>> gen_functions = {
        [](int n) { return n; },
        [](int n) { return sqrt(n); },
        [](int n) { return n / log2(n + 2); },
        [](int n) { return log2(n + 2); }
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
            int x = itr(N);
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

vector<Query> GetQueries(int q, const Input& x) {
    vector<Query> queries(q);
    for (auto& itr : queries) {
        itr.x = Rand(0, x.n);
        itr.k = Rand(0, x.n);
    }

    return queries;
}

vector<Query> GetQueriesSmart(int q, const Input& x) {
    auto& order = x.els;
    int n = x.n;

    int p = 0;
    while ((1 << p) < n) {
        p += 1;
    }

    vector<vector<int>> num_required_to_win(n);
    for (int i = 0; i < n; i += 1) {
        num_required_to_win[i].resize(p);
    }

    vector<int> where(n);
    for (int i = 0; i < n; i += 1) {
        where[order[i]] = i;
    }

    for (int step = 0; step < p; step += 1) {
        vector<int> subtree_size(n >> step);
        int mx = 0;
        for (int i = 0; i < n; i += 1) {
            int w = where[i];
            num_required_to_win[i][step] = (1 << step) - max(min(mx, (1 << step) - 1), subtree_size[w >> step] + 1);
            subtree_size[w >> step] += 1;
            mx = max(mx, subtree_size[w >> step]);

            if (i + 1 < (1 << step)) {
                num_required_to_win[i][step] = -1;
            }
        }
    }

    vector<Query> queries;
    while (q--) {
        queries.push_back(Query());
        auto& Q = queries.back();
        if (rand() % 30 == 0) {
            Q.x = rand() % n;
            Q.k = 0;
            continue;
        }

        if (rand() % 30 == 0) {
            Q.x = rand() % n;
            Q.k = Rand(Q.x, n);
            continue;
        }

        while (1) {
            int x = rand() % n;
            int height = rand() % p;

            int k = num_required_to_win[x][height];
            if (k <= 0) {
                continue;
            }

            int op = rand() % 15;
            if (op < 1) {
                k -= 3;
            } else if (op < 3) {
                k -= 2;
            } else if (op < 6) {
                k -= 1;
            } else if (op < 9) {
            } else if (op < 12) {
                k += 1;
            } else if (op < 14) {
                k += 2;
            } else {
                k += 3;
            }

            k = max(k, 0);
            k = min(k, n - 1);

            Q.x = x;
            Q.k = k;

            break;
        }
    }

    return queries;
}

vector<int> GetNumbers(vector<int> available_numbers) {
    int n = available_numbers.size();
    if (n == 1) {
        return available_numbers;
    }

    Sort(available_numbers);
    reverse(available_numbers.begin(), available_numbers.end());

    auto p = NicePartition(n);
    vector<int> left, right;
    for (auto itr : p) {
        while (itr--) {
            left.push_back(available_numbers.back());
            available_numbers.pop_back();
        }
        swap(left, right);
    }

    reverse(right.begin(), right.end());
    for (auto itr : right) {
        left.push_back(itr);
    }

    auto ans1 = GetNumbers(vector<int>(left.begin(), left.begin() + n / 2));
    auto ans2 = GetNumbers(vector<int>(left.begin() + n/2, left.begin() + n));
    for (auto itr : ans2) {
        ans1.push_back(itr);
    }

    return ans1;
}

string Random(int p, int q, bool sort_queries=false) {
    int n = (1 << p);
    Input x;
    x.p = p;
    x.n = n;
    x.els = GetNumbers(Range(n));
    RandomShuffle(x.els);
    x.queries = GetQueriesSmart(q, x);
    if (sort_queries) {
        sort(x.queries.begin(), x.queries.end(), [&](auto& a, auto& b) { return a.x < b.x; });
    }
    return Stringify(x);
}

int main() {
    addTest(Stringify(Input({
        2, 4, {3, 2, 0, 1}, {
            {1, 0},
            {0, 2},
            {3, 1},
            {2, 2}
        }
    })));

    addTest(Stringify(Input({
        3, 8, {2, 7, 3, 0, 1, 4, 6, 5}, {
            {3, 1},
            {3, 0},
            {1, 5},
            {4, 7},
            {7, 7},
        }
    })));

    // 15 points small shit
    addTest(Random(5, 100));
    addTest(Random(7, 1000));
    addTest(Random(10, 1000));

    // 20 points sorted shit
    addTest(Random(14, 1e5, true));
    addTest(Random(15, 2e5, true));
    addTest(Random(16, 3e5, true));
    addTest(Random(17, 5e5, true));

    // 10 points NlogN mem online
    addTest(Random(18, 5e5));
    addTest(Random(18, 5e5));

    // 10 points NlogN mem optimizat online
    addTest(Random(19, 5e5));
    addTest(Random(19, 5e5));

    // 45
    addTest(Random(20, 5e5));
    addTest(Random(20, 5e5));
    addTest(Random(20, 5e5));
    addTest(Random(20, 5e5));
    addTest(Random(20, 5e5));
    addTest(Random(20, 5e5));
    addTest(Random(20, 5e5));
    addTest(Random(20, 5e5));
    addTest(Random(20, 5e5));

    RunTumbletest();
    return 0;
}
