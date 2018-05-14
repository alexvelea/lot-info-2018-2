#include <algorithm>
#include <vector>
#include <array>
#include <algorithm>
#include <cassert>

#include "mere.h"

using namespace std;

template<class T, class F, class Data>
class FenwickTree {
  public:
    FenwickTree():
        FenwickTree(vector<T>(0)) {}
    FenwickTree(const vector<T>& keys):
        m_map(F()),
        m_keys(keys),
        m_data(keys.size(), 0) {
        sort(m_keys.begin(), m_keys.end(), [&](T x, T y) {
            return m_map(x) < m_map(y);
        });
    }

    void add(T key, Data value) {
        int pos = lower_bound(m_keys.begin(), m_keys.end(), key, [&](T x, T y) {
            return m_map(x) < m_map(y);
        }) - m_keys.begin();
        for (++pos; pos <= int(m_data.size()); pos += (pos & -pos))
            m_data[pos - 1] += value;
    }

    Data query(T key) const {
        int pos = lower_bound(m_keys.begin(), m_keys.end(), key, [&](T x, T y) {
            return m_map(x) < m_map(y);
        }) - m_keys.begin();
        Data answer;
        for (++pos; pos > 0; pos -= (pos & -pos))
            answer += m_data[pos - 1];
        return answer;
    }

    Data total() const {
        int pos = m_data.size();
        Data answer;
        for (; pos > 0; pos -= (pos & -pos))
            answer += m_data[pos - 1];
        return answer;
    }

  private:
    F m_map;
    vector<T> m_keys;
    vector<Data> m_data;
};

class ZP {
  public:
    ZP(int x = 0):
        m_data(x % kModulo) {
            if (m_data < 0)
                m_data += kModulo;
        }

    operator int() {
        return m_data;
    }

    ZP& operator+=(const ZP& that) {
        m_data += that.m_data;
        if (m_data >= kModulo)
            m_data -= kModulo;
        return *this;
    }

    ZP operator+(const ZP& that) const {
        ZP result = *this;
        result += that;
        return result;
    }

  private:
    static const int kModulo = 1000 * 1000 * 1000 + 7;
    int m_data;
};

struct sum_coords {
    int64_t operator()(pair<int64_t, int> p) const {
        return p.first + p.second;
    }
};

struct diff_coords {
    int64_t operator()(pair<int64_t, int> p) const {
        return p.first - p.second;
    }
};

int abs(int x) {
    if (x < 0)
        return -x;
    return x;
}

bool can_reach(pair<int64_t, int> a, pair<int64_t, int> b) {
    return (b.first - a.first) >= abs(b.second - a.second);
}

int count(int N, int VA, int VB, int *X, int *T) {
    assert(1 <= N && N <= 200 * 1000);
    assert(1 <= VA && VA <= 1000 * 1000 * 1000);
    assert(1 <= VB && VB <= 1000 * 1000 * 1000);

    vector< pair<int, int> > events(N + 1);
    events[0] = make_pair(0, 0);
    for (int i = 0; i < N; ++i) {
        assert(-1000 * 1000 * 1000 <= X[i] && X[i] <= 1000 * 1000 * 1000);
        assert(1 <= T[i] && T[i] <= 1000 * 1000 * 1000);

        if (i > 0)
            assert(T[i] > T[i - 1]);
        events[i + 1] = make_pair(T[i], X[i]);
    }

    events.emplace_back(2000 * 1000 * 1000, 0);
    N += 2;

    array<int, 2> speed = {{VA, VB}};

    vector< pair<int64_t, int> > events_normalized[2];
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < 2; ++j)
            events_normalized[j].emplace_back(1LL * events[i].first * speed[j], events[i].second);
    }

    FenwickTree<pair<int64_t, int>, sum_coords, ZP> typeA[2];
    FenwickTree<pair<int64_t, int>, diff_coords, ZP> typeB[2];
    for (int i = 0; i < 2; ++i) {
        typeA[i] = FenwickTree<pair<int64_t, int>, sum_coords, ZP>(events_normalized[i]);
        typeB[i] = FenwickTree<pair<int64_t, int>, diff_coords, ZP>(events_normalized[i]);
    }

    vector<ZP> dp[2];
    dp[0] = dp[1] = vector<ZP>(N, 0);
    for (int i = 0; i < 2; ++i)
        if (can_reach(events_normalized[i][0], events_normalized[i][1]))
            dp[i][1] = 1;

    vector<int> can_go_back[2];
    can_go_back[0] = can_go_back[1] = vector<int>(N);
    for (int i = 1; i < N; ++i)
        can_go_back[0][i] = can_go_back[1][i] = i;

    for (int i = 0; i < 2; ++i)
        for (int j = 2; j < N; ++j) {
            if (can_reach(events_normalized[i][j - 1], events_normalized[i][j]))
                can_go_back[i][j] = can_go_back[i][j - 1];
        }

    for (int i = 1; i < N; ++i) {
        if (i > 1) {
            for (int j = 0; j < 2; ++j)
                for (int k = max(can_go_back[j][i - 2], 1); k < can_go_back[j][i - 1]; ++k) {
                    typeA[1 - j].add(events_normalized[1 - j][k - 1], -dp[j][k]);
                    typeB[1 - j].add(events_normalized[1 - j][k - 1], -dp[j][k]);
                }
        }

        for (int j = 0; j < 2; ++j) {
            dp[j][i] += typeA[j].query(events_normalized[j][i]) + typeB[j].query(events_normalized[j][i]) - typeA[j].total();
        }

        for (int j = 0; j < 2; ++j) {
            typeA[j].add(events_normalized[j][i - 1], dp[1 - j][i]);
            typeB[j].add(events_normalized[j][i - 1], dp[1 - j][i]);
        }
    }

    return dp[0].back() + dp[1].back();
}
