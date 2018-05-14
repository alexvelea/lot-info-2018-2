#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <set>

using namespace std;

static const int kModulo  = 1000 * 1000 * 1000 + 7;

// ax + b
struct Function {
    Function(int64_t _a = 0, int64_t _b = 0):
        a(_a),
        b(_b) {}

    int64_t a;
    int64_t b;

    int operator()(int x) const {
        return (a % kModulo * x + b) % kModulo;
    }

    Function& operator+=(const Function& that) {
        a += that.a;
        b += that.b;
        return *this;
    }
};

// axy + bx + cy + d
struct Function2 {
    Function2(int64_t _a = 0, int64_t _b = 0, int64_t _c = 0, int64_t _d = 0):
        a(_a),
        b(_b),
        c(_c),
        d(_d) {}

    int64_t a;
    int64_t b;
    int64_t c;
    int64_t d;

    int operator()(int x, int y) const {
        return (1LL * a * x % kModulo * y + 1LL * b * x + 1LL * c * y + d) % kModulo;
    }

    Function2& operator+=(const Function2& that) {
        a += that.a;
        b += that.b;
        c += that.c;
        d += that.d;
        return *this;
    }
};

template<class T>
class FenwickTree {
  public:
    FenwickTree(int size):
        m_data(size) {}

    int size() const {
        return m_data.size();
    }

    void add(int pos, const T& to_add) {
        for (++pos; pos <= size(); pos += (pos & -pos))
            m_data[pos - 1] += to_add;
    }

    T query(int pos) {
        T answer;
        for (++pos; pos > 0; pos -= (pos & -pos))
            answer += m_data[pos - 1];
        return answer;
    }

  private:
    vector<T> m_data;
};

class Solver {
  public:
    Solver(vector<int> V):
        m_values(V),
        m_last(V.size(), -1),
        m_next(V.size(), V.size()),
        m_fenwick_tree(V.size()),
        m_fenwick_tree2(V.size()),
        m_total_cost(0),
        m_pos(V.size())
    {}

    int total_cost() {
        vector<int> where(size(), -1);
        for (int i = 0; i < size(); ++i) {
            m_last[i] = where[m_values[i]];
            where[m_values[i]] = i;
        }

        fill(where.begin(), where.end(), size());
        for (int i = size() - 1; i >= 0; --i) {
            m_next[i] = where[m_values[i]];
            where[m_values[i]] = i;
        }

        divide(0, size());

        for (int i = 0; i < size(); ++i) {
            int min_begin = m_last[i] + 1;
            int max_begin = i;
            int min_end = i;

            m_total_cost = (m_total_cost + static_cast<int64_t>(max_begin - min_begin + 1) * (size() - min_end) % kModulo * (m_values[i] + 1)) % kModulo;
        }

        return m_total_cost;
    }

    int size() const {
        return m_values.size();
    }

  private:
    void divide(int from, int to) {
        if (to - from == 1)
            return;
        int mid = (from + to) / 2;
        divide(from, mid);
        divide(mid, to);

        // we have i in [from, mid) and j in [mid, to)
        case1(from, mid, to);
        case2(from, mid, to);
        case3(from, mid, to);
        case4(from, mid, to);
    }

    void case1(int from, int mid, int to) {
        // case 1, V[i] < V[j] and last[j] <= last[i]
        for (int i = from; i < to; ++i)
            m_pos[i] = i;
        sort(m_pos.begin() + from, m_pos.begin() + to, [&](int x, int y) {
            if (m_last[x] != m_last[y])
                return m_last[x] < m_last[y];
            return m_values[x] > m_values[y];
        });

        for (int i = to - 1; i >= from; --i) {
            int x = m_pos[i];
            if (m_last[x] >= mid)
                continue;
            if (x >= mid) {
                auto f = m_fenwick_tree.query(m_values[x] - 1);
                m_total_cost = (m_total_cost + 1LL * f(x) * (m_values[x] + 1)) % kModulo;
            } else {
                m_fenwick_tree.add(m_values[x], Function{m_last[x] - x, 1LL * (x - m_last[x]) * size()});
            }
        }

        for (int i = to - 1; i >= from; --i) {
            int x = m_pos[i];
            if (m_last[x] >= mid)
                continue;
            if (x < mid)
                m_fenwick_tree.add(m_values[x], Function{x - m_last[x], 1LL * (m_last[x] - x) * size()});
        }
    }

    void case2(int from, int mid, int to) {
        // case 2, V[i] < V[j] and last[j] > last[i]
        // luckily the order in which we process is reverse of case 1
        set<int> to_erase;
        for (int i = from; i < to; ++i) {
            int x = m_pos[i];
            if (m_last[x] >= mid)
                continue;

            while (!to_erase.empty() && *to_erase.begin() <= m_last[x]) {
                int y = *to_erase.begin();
                to_erase.erase(y);
                m_fenwick_tree2.add(m_values[y], Function2{-1, +y, +size(), -1LL * y * size()});
            }

            if (x >= mid) {
                auto f = m_fenwick_tree2.query(m_values[x] - 1);
                m_total_cost = (m_total_cost + 1LL * f(x, m_last[x]) * (m_values[x] + 1)) % kModulo;
            } else {
                m_fenwick_tree2.add(m_values[x], Function2{1, -x, -size(), 1LL * x * size()});
                to_erase.insert(x);
            }
        }

        for (auto &x : to_erase) {
            m_fenwick_tree2.add(m_values[x], Function2{-1, +x, +size(), -1LL * x * size()});
        }
    }

    void case3(int from, int mid, int to) {
        // V[i] > V[j], and last[j] <= last[i]
        for (int i = from; i < to; ++i) {
            int x = m_pos[i];
            if (m_last[x] >= mid)
                continue;
            if (x >= mid) {
                m_fenwick_tree.add(m_values[x], Function{size() - x, 0});
            } else {
                auto f = m_fenwick_tree.query(m_values[x] - 1);
                m_total_cost = (m_total_cost + 1LL * f(x - m_last[x]) % kModulo * (m_values[x] + 1)) % kModulo;
            }
        }

        for (int i = from; i < to; ++i) {
            int x = m_pos[i];
            if (m_last[x] >= mid)
                continue;
            if (x >= mid) {
                m_fenwick_tree.add(m_values[x], Function{x - size(), 0});
            }
        }
    }

    void case4(int from, int mid, int to) {
        // V[i] > V[j], and last[j] > last[i]
        set<int> to_add;
        for (int i = from; i < to; ++i) {
            int x = m_pos[i];
            if (m_last[x] >= mid)
                continue;

            while (!to_add.empty() && *to_add.begin() <= m_last[x]) {
                int y = *to_add.begin();
                to_add.erase(y);
                auto f = m_fenwick_tree.query(m_values[y] - 1);
                m_total_cost = (m_total_cost + 1LL * f(y) * (m_values[y] + 1)) % kModulo;
            }

            if (x >= mid) {
                m_fenwick_tree.add(m_values[x], Function{size() - x, -1LL * m_last[x] * (size() - x)});
            } else {
                // decrease now, we'll fix it later
                auto f = m_fenwick_tree.query(m_values[x] - 1);
                m_total_cost = (m_total_cost - 1LL * f(x) * (m_values[x] + 1)) % kModulo;
                m_total_cost = (m_total_cost + kModulo) % kModulo;
                to_add.insert(x);
            }
        }

        while (!to_add.empty()) {
            int y = *to_add.rbegin();
            to_add.erase(y);
            auto f = m_fenwick_tree.query(m_values[y] - 1);
            m_total_cost = (m_total_cost + 1LL * f(y) * (m_values[y] + 1)) % kModulo;
        }

        for (int i = from; i < to; ++i) {
            int x = m_pos[i];
            if (m_last[x] >= mid)
                continue;
            if (x >= mid) {
                m_fenwick_tree.add(m_values[x], Function{x - size(), 1LL * m_last[x] * (size() - x)});
            }
        }
    }

    vector<int> m_values, m_last, m_next;
    FenwickTree<Function> m_fenwick_tree;
    FenwickTree<Function2> m_fenwick_tree2;
    int m_total_cost;
    vector<int> m_pos;
};

int main() {
    ifstream cin("sortall.in");
    ofstream cout("sortall.out");

    int N; assert(cin >> N);
    assert(1 <= N && N <= 100 * 1000);
    vector<int> V(N);
    for (int i = 0; i < N; ++i) {
        assert(cin >> V[i]);
        assert(1 <= V[i] && V[i] <= N);
        --V[i];
    }

    Solver S(V);
    cout << S.total_cost() << "\n";
}
