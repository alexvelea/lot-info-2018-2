#include <cassert>
#include <iostream>
#include <stack>
#include <set>
#include <numeric>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>
using namespace std;

vector<int> inv(vector<int> v){
    vector<int> r(v.size(), 0);
    for(int i = 0; i < v.size(); ++i)
        r[v[i]] = i;
    return r; }

class rooted_tree{
    int n;
    vector<int> tata;
    vector<int> cost;
    vector<vector<int>> fii;
    vector<int> rz;
    vector<int> sz;
    void _preorder(int cur){
        rz.push_back(cur);
        sort(begin(fii[cur]), end(fii[cur]),
            [cur, this](int x, int y){ return cost[x] < cost[y]; });
        for(auto next : fii[cur])
            _preorder(next); }
public:
    rooted_tree(int N): n(N), tata(n), cost(n), fii(n), sz(n, 1){
        for(int i = 0; i < n; ++i) tata[i] = i; }
    void set_tata(int x, int y){
        tata[x] = y;
        sz[y] += sz[x];
        fii[y].push_back(x); }
    int get_root(int x){
        return x == tata[x] ? x : get_root(tata[x]); }
    int get_size(int x){
        return sz[x]; }
    int find_depth(int x){
        return tata[x] == x ? 0 : 1 + find_depth(tata[x]); }
    void set_cost(int son, int x){
        cost[son] = x; }

    vector<int> preorder(){
        rz.clear();
        _preorder(get_root(0));
        return rz; }

    int lca_path_max(int x, int y){
        //assert(find_depth(x) < 20);
        assert(find_depth(y) < 100);
        static auto ord = inv(preorder());
        int rez = 0;
        while(x != y){
            if(ord[x] > ord[y]) swap(x, y);
            rez = max(rez, cost[y]);
            y = tata[y]; }
        return rez; } };

rooted_tree build_kruskal_tree(int n, vector<tuple<int, int, int>> v){
    sort(begin(v), end(v));
    rooted_tree r(n);

    for(auto edge : v){
        int c, x, y;
        tie(c, x, y) = edge;

        assert(r.find_depth(x) < 100);
        assert(r.find_depth(y) < 100);
        int x0 = r.get_root(x), y0 = r.get_root(y);
        assert(x0 != y0);
        if(r.get_size(x0) < r.get_size(y0)) swap(x0, y0);

        r.set_tata(y0, x0);
        r.set_cost(y0, c); }

    return r; }

vector<int> ord;

struct by_ord {
    bool operator()(int x, int y) const {
        return ord[x] < ord[y];
    }
};

int main(){
    ifstream f("trenbus.in");
    ofstream g("trenbus.out");
    int C, n;
    f >> C >> n;
    vector<tuple<int, int, int>> v1(n-1), v2(n-1);

    for(auto& x : v1) f >> get<1>(x) >> get<2>(x) >> get<0>(x), --get<1>(x), --get<2>(x);
    for(auto& x : v2) f >> get<1>(x) >> get<2>(x) >> get<0>(x), --get<1>(x), --get<2>(x);

    auto k = build_kruskal_tree(n, v2);
    ord = inv(k.preorder());

    sort(begin(v1), end(v1));
    vector<set<int, by_ord>> sets;

    sets.resize(n, set<int, by_ord>());

    vector<int> which_set(n);

    for(int i = 0; i < sets.size(); ++i){
        sets[i].insert(i);
        which_set[i] = i; }
    
    int rez = 2e9;

    int t0, t1;
    for(auto edge : v1){
        int c, x, y;
        tie(c, x, y) = edge;

        if(sets[which_set[x]].size() > sets[which_set[y]].size()) swap(x, y);
        const int wsy = which_set[y];
        auto& sx = sets[which_set[x]];
        auto& sy = sets[which_set[y]];
        for(auto xx : sx){
            t0 -= time(nullptr);
            auto y1 = sy.lower_bound(xx);
            t0 += time(nullptr);
            t1 -= time(nullptr);
            if(y1 != end(sy))
                rez = min(rez, c + k.lca_path_max(xx, *y1));
            if(y1 != begin(sy)){
                rez = min(rez, c + k.lca_path_max(xx, *prev(y1))); }
            t1 += time(nullptr);
            which_set[xx] = which_set[y];
            sy.insert(y1, xx); } }

    cout << "\t\t\t" << t0 << ' ' << t1 << endl;
    g << rez << endl;
    return 0; }
