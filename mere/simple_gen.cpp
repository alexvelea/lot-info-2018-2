#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <inputGenerator.hpp>
#include <cassert>

using namespace std;
using namespace inputGenerator;

pair<int, int> get_range(int position, int last_time, int speed, int current_time, int MAX_COORD) {
    int64_t minimum = position - 1LL * (current_time - last_time) * speed;
    int64_t maximum = position + 1LL * (current_time - last_time) * speed;
    if (minimum < -MAX_COORD)
        minimum = -MAX_COORD;
    if (maximum > MAX_COORD)
        maximum = MAX_COORD;
    return make_pair(minimum, maximum);
}

bool included(pair<int, int> a, pair<int, int> b) {
    return a.first >= b.first && a.second <= b.second;
}

vector<pair<int, int>> diff(pair<int, int> a, pair<int, int> b) {
    vector< pair<int, int> > answer;
    if (a.first < b.first)
        answer.emplace_back(a.first, min(b.first - 1, a.second));
    if (b.second < a.second)
        answer.emplace_back(max(b.second + 1, a.first), a.second);
    return answer;
}

bool intersect(pair<int, int> a, pair<int, int> b) {
    if (a.second < b.first)
        return false;
    if (b.second < a.first)
        return false;
    return true;
}

int pick(vector<pair<int, int>> where) {
    int sum = 0;
    for (auto &x : where)
        sum += x.second - x.first + 1;
    int which = randomInt(0, sum - 1);
    for (auto &p : where)
        if (which < p.second - p.first + 1)
            return which + p.first;
        else
            which -= p.second - p.first + 1;
    assert(false);
}

int main(int argc, char **argv) {
    vector<string> arguments(argv + 1, argv + argc);

    if (arguments.size() < 6) {
        cerr << "USAGE: N VA VB MAXT MAXX PROB_BOTH\n";
        return 1;
    }

    int N = stoi(arguments[0]);
    int VA = stoi(arguments[1]);
    int VB = stoi(arguments[2]);
    int MAXT = stoi(arguments[3]);
    int MAXX = stoi(arguments[4]);
    double PROB_BOTH = stod(arguments[5]);

    Seed::logging = false;
    if (arguments.size() > 6) {
        reSeed(stoul(arguments[6]));
    }

    ios::sync_with_stdio(false);

    auto T = randomSample(N, 1, MAXT);
    int poz1 = 0, poz2 = 0, time1 = 0, time2 = 0;

    cout << N << " " << VA << " " << VB << "\n";
    for (int i = 0; i < N; ++i) {
        auto range1 = get_range(poz1, time1, VA, T[i], MAXX);
        auto range2 = get_range(poz2, time2, VB, T[i], MAXX);

        if (range1 == range2 || (intersect(range1, range2) && randomDouble(0.0, 1.0) <= PROB_BOTH)) {
            auto range = make_pair(max(range1.first, range2.first), min(range1.second, range2.second));
            int X = randomInt(range.first, range.second);
            cout << X << " " << T[i] << "\n";

            assert(-1000 * 1000 * 1000 <= X && X <= 1000 * 1000 * 1000);
            if (randomElement({true, false})) {
                poz1 = X;
                time1 = T[i];
            } else {
                poz2 = X;
                time2 = T[i];
            }
        } else {
            if (!included(range1, range2) && (included(range2, range1) || randomElement({true, false}))) {
                auto where = diff(range1, range2);
                int X = pick(where);
                assert(-1000 * 1000 * 1000 <= X && X <= 1000 * 1000 * 1000);
                cout << X << " " << T[i] << "\n";
                poz1 = X;
                time1 = T[i];
            } else {
                auto where = diff(range2, range1);
                int X = pick(where);
                assert(-1000 * 1000 * 1000 <= X && X <= 1000 * 1000 * 1000);
                cout << X << " " << T[i] << "\n";
                poz2 = X;
                time2 = T[i];
            }
        }
    }
}
