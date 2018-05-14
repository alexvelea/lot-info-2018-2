#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

static const int kHashSize = (1<<23);
static const int kPrime1 = 666013;
static const int kPrime2 = 10000007;

static const pair<int, int> kEmpty = make_pair(-1, -1);

class OpenAddressingHash {
  public:
    OpenAddressingHash():
        m_keys(kHashSize, kEmpty),
        m_value(kHashSize),
        m_combinations(0) {}

    int64_t combinations() const {
        return m_combinations;
    }

    void change(int key1, int key2, int value) {
        int where = position(key1, key2);
        int old_value = m_value[where];
        m_value[where] += value;
        int new_value = m_value[where];

        if (new_value < old_value) { // lost some stuff
            m_combinations -= new_value;
        } else {
            m_combinations += old_value;
        }
    }

    int position(int key1, int key2) {
        int64_t number = 1LL * kPrime1 * key1 + 1LL * kPrime2 * key2;
        int hash = number % kHashSize;

        while (m_keys[hash] != make_pair(key1, key2)) {
            if (m_keys[hash] == kEmpty)
                break;
            hash = (hash + 1) % kHashSize;
        }
        m_keys[hash] = make_pair(key1, key2);
        return hash;
    }

  private:
    vector< pair<int, int> > m_keys;
    vector<int> m_value;
    int64_t m_combinations;
};

struct Edge {
    int from, to;
    int cost;

    bool operator<(const Edge& that) const {
        return cost < that.cost;
    }
};

class Tree {
  public:
    Tree(int size = 0):
        m_root(size),
        m_size(size, 1),
        m_edges(size) {
        for (int i = 0; i < size; ++i)
            m_root[i] = i;
        }

    int size() const {
        return m_edges.size();
    }

    void add_edge(int from, int to, int cost) {
        m_edges[from].emplace_back(to, cost);
        m_edges[to].emplace_back(from, cost);
    }

    bool is_tree() {
        vector<bool> visited(size(), false);
        return dfs_check_tree(0, visited) == size();
    }

    int root(int node) const {
        return m_root[node];
    }

    vector<Edge> edges() const {
        vector<Edge> edges;
        edges.reserve(size() - 1);
        for (int i = 0; i < size(); ++i)
            for (auto &x : m_edges[i])
                if (i < x.first)
                    edges.emplace_back(Edge{i, x.first, x.second});
        return edges;
    }

    pair<Tree, vector<int>> set_join() const {
        Tree set_join(size());
        vector<int> roots;

        auto edges = this->edges();
        sort(edges.begin(), edges.end());
        for (auto &edge : edges) {
            int new_root = set_join.join(set_join.root(edge.from), set_join.root(edge.to), edge.cost, [&](int, int, int) {});
            roots.push_back(new_root);
        }

        return make_pair(set_join, roots);
    }

    template<class CB>
    int join(int x, int y, int cost, CB cb) {
        assert(m_root[x] == x);
        assert(m_root[y] == y);

        int from, to;
        if (m_size[x] <= m_size[y]) {
            from = x;
            to = y;
        } else {
            from = y;
            to = x;
        }

        dfs_cb(from, [&](int node) {
            m_root[node] = to;
            cb(node, from, to);
        });
        m_size[to] += m_size[from];
        m_edges[to].emplace_back(from, cost);
        return to;
    }

    template<class CB>
    void split(int root, CB cb) {
        assert(m_root[root] == root);
        assert(!m_edges[root].empty());

        int new_root = m_edges[root].back().first;
        m_edges[root].pop_back();
        dfs_cb(new_root, [&](int node) {
            m_root[node] = new_root;
            cb(node);
        });
    }

    int cost_to_erase(int root) {
        assert(m_root[root] = root);

        // it's the last son
        assert(!m_edges[root].empty());
        return m_edges[root].back().second;
    }

  private:
    int dfs_check_tree(int node, vector<bool> &visited) {
        if (visited[node])
            return 0;
        visited[node] = true;
        int total = 1;
        for (auto &x : m_edges[node])
            total += dfs_check_tree(x.first, visited);
        return total;
    }

    template<class CB>
    void dfs_cb(int node, CB cb) {
        cb(node);
        for (auto &x : m_edges[node])
            dfs_cb(x.first, cb);
    }

    vector<int> m_root;
    vector<int> m_size;
    vector< vector< pair<int, int> > > m_edges;
};

istream& operator>>(istream& stream, Tree& tree) {
    for (int i = 1; i < tree.size(); ++i) {
        int x, y, cost; stream >> x >> y >> cost;
        assert(1 <= x && x <= tree.size());
        assert(1 <= y && y <= tree.size());
        assert(x != y);
        assert(1 <= 1000 * 1000 * 1000);
        tree.add_edge(x - 1, y - 1, cost);
    }

    assert(tree.is_tree());
    return stream;
}

int main() {
    ifstream cin("trenbus.in");
    ofstream cout("trenbus.out");

    int C, N; assert(cin >> C >> N);
    assert(1 <= N && N <= 200 * 1000);

    Tree T1(N), T2(N);
    assert(cin >> T1 >> T2);

    Tree set_join1;
    vector<int> roots;
    tie(set_join1, roots) = T1.set_join();

    OpenAddressingHash hash_map;
    for (int i = 0; i < N; ++i)
        hash_map.change(set_join1.root(i), i, +1);

    auto edges = T2.edges();
    sort(edges.begin(), edges.end());
    reverse(edges.begin(), edges.end());

    Tree set_join2(N);
    int best = numeric_limits<int>::max();
    int64_t count = 0;
    // last value of an edge added from right side
    int last_value_added = -1;
    while (!roots.empty()) {
        // first solve before erasing
        // this is the cost on the left side
        int cost_of_edge = set_join1.cost_to_erase(roots.back());

        // but we might not be connected
        while (!edges.empty() && (hash_map.combinations() == 0 || edges.back().cost == last_value_added)) {
            // add edges in order, but add all of the same cost always
            auto edge = edges.back();
            edges.pop_back();
            set_join2.join(set_join2.root(edge.from), set_join2.root(edge.to), edge.cost, [&](int node, int old_root, int new_root) {
                hash_map.change(set_join1.root(node), old_root, -1);
                hash_map.change(set_join1.root(node), new_root, 1);
            });
            last_value_added = edge.cost;
        }

        if (cost_of_edge + last_value_added < best) {
            best = cost_of_edge + last_value_added;
            count = 0;
        }
        if (cost_of_edge + last_value_added == best)
            count += hash_map.combinations();

        // then erase all with same value
        while (!roots.empty()) {
            // for each root we have to erase it's last added son (probably last son in order)
            if (set_join1.cost_to_erase(roots.back()) != cost_of_edge)
                break;

            // this will become the new root for the separated value
            set_join1.split(roots.back(), [&](int node) {
                // for these values, the pair (root1, root2) changes, so update accordingly
                hash_map.change(roots.back(), set_join2.root(node), -1);
                hash_map.change(set_join1.root(node), set_join2.root(node), +1);
            });
            roots.pop_back();
        }
    }

    if (C == 2) {
        cout << best << " " << count << "\n";
    } else {
        cout << best << '\n';
    }
}
