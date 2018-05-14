#include <list>
#include <iostream>
#include <cassert>
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

        int x0 = r.get_root(x), y0 = r.get_root(y);
        if(r.get_size(x0) < r.get_size(y0)) swap(x0, y0);

        r.set_tata(y0, x0);
        r.set_cost(y0, c); }

    return r; }

int main(){
    ifstream f("trenbus.in");
    ofstream g("trenbus.out");
    int n;
    f >> n;
    vector<tuple<int, int, int>> v1(n-1), v2(n-1);

    for(auto& x : v1) f >> get<1>(x) >> get<2>(x) >> get<0>(x), --get<1>(x), --get<2>(x);
    for(auto& x : v2) f >> get<1>(x) >> get<2>(x) >> get<0>(x), --get<1>(x), --get<2>(x);

    auto k = build_kruskal_tree(n, v2);
    auto ord = inv(k.preorder());
    auto by_ord = [&ord](int x, int y){ return ord[x]  < ord[y]; };

    sort(begin(v1), end(v1));
    struct comanda{
        list<int> which;
        int surs, targ, val; };

    vector<int> which_set(n, 0);
    vector<list<int>> sets(n, list<int>{});
    vector<list<int>::iterator> where(n);
    vector<comanda> delta;

    for(int i = 0; i < n; ++i){
        which_set[i] = i;
        sets[i].push_back(i);
        where[i] = sets[i].begin(); }

    for(auto edge : v1){
        int c, x, y;
        tie(c, x, y) = edge;

        if(sets[which_set[x]].size() > sets[which_set[y]].size()) swap(x, y);
        int wsx = which_set[x], wsy = which_set[y];

        delta.push_back(comanda { sets[wsx], wsx, wsy, c });

        for(auto xx : sets[wsx]){
            which_set[xx] = wsy;
            sets[wsy].push_front(xx);
            where[xx] = begin(sets[wsy]); }
        sets[wsx].clear(); }

    for(auto& s : sets)
        s.sort(by_ord);

    struct considered{
        int delta, x, y; };

    vector<considered> cc;

    reverse(begin(delta), end(delta));

    for(auto &d : delta){
        for(auto& x : d.which){
            auto it = where[x], b = begin(sets[which_set[x]]), e = end(sets[which_set[x]]);
            assert(which_set[x] == d.targ);
            if(it != b)
                cc.push_back(considered { d.val, *it, *prev(it) });
            if(next(it) != e)
                cc.push_back(considered { d.val, *it, *next(it) });
            sets[d.surs].push_front(x);
            sets[which_set[x]].erase(where[x]);
            which_set[x] = d.surs;
            where[x] = begin(sets[d.surs]);
        } }
            
    int rez = 2e9;
    
    for(auto x : cc) {
        rez = min(rez, x.delta + k.lca_path_max(x.x, x.y)); }

    g << rez << endl;
    return 0; }
