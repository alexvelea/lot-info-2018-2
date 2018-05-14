#include <iostream>
#include <cassert>
#include <fstream>
#include <vector>
using namespace std;

constexpr int mod = 1e9 + 7;
using ll = long long;

long long brut(vector<int> v){
    long long rez = 0;
    for(int i = 0; i < v.size(); ++i){
        for(int j = i+1; j <= v.size(); ++j){
            vector<int> vv(begin(v) + i, begin(v) + j);
            sort(begin(vv), end(vv));
            vv.erase(unique(begin(vv), end(vv)), end(vv));
            long long delta = 0;
            for(int k = 0; k < vv.size(); ++k){
                delta += (vv[k] + 1) * (k + 1); }
            rez += delta; } }
    return rez; }

class arbint2d{
    int n;
    vector<vector<int>> buf;
    vector<vector<int>> points;
    int poz_in_line(int l, int x){
        return lower_bound(begin(points[l]), end(points[l]), x) - begin(points[l]); }
    int line_sum(int l, int x, int y){
        x = poz_in_line(l, x), y = poz_in_line(l, y + 1);
        long long rez = 0;
        for(x += points[l].size(), y += points[l].size(); x < y; x /= 2, y /= 2){
            if(x%2) rez += buf[l][x++];
            if(y%2) rez += buf[l][--y]; }
        return rez % mod; }
public:
    arbint2d(int N, vector<pair<int, int>>& v) : n(N), buf(2*n), points(2*n){
        for(auto x : v) points[x.first + n].push_back(x.second);
        for(int i = n; i < 2 * n; ++i){
            sort(begin(points[i]), end(points[i]));
            points[i].erase(unique(begin(points[i]), end(points[i])), end(points[i])); }

        for(int i = n-1; i; --i){
            merge(begin(points[2*i]), end(points[2*i]),
                begin(points[2*i+1]), end(points[2*i+1]),
                back_inserter(points[i]));
            points[i].erase(unique(begin(points[i]), end(points[i])), end(points[i]));
            points[i].shrink_to_fit(); }
        for(int i = 1; i < 2 * n; ++i)
            buf[i].resize(2 * points[i].size()); }
    void update(int x, int y, int delta){
        delta -= query(x, x, y, y);
        delta %= mod;
        if(delta < 0) delta += mod;

        for(x += n; x; x /= 2){
            assert(binary_search(begin(points[x]), end(points[x]), y));
            for(int y_ = poz_in_line(x, y) + points[x].size(); y_; y_ /= 2){
                buf[x][y_] += delta;
                buf[x][y_] %= mod; } } }
    int query(int st, int dr, int x, int y){
        long long rez = 0;
        for(st += n, dr += n + 1; st < dr; st /= 2, dr /= 2){
            if(st%2) rez += line_sum(st++, x, y);
            if(dr%2) rez += line_sum(--dr, x, y); }
        return rez % mod; } };

vector<int> build_next(vector<int>& v){
    vector<int> next(v.size()), tmp(v.size(), v.size());
    for(int i = v.size() - 1; i >= 0; --i){
        next[i] = abs(tmp[v[i]] - i);
        tmp[v[i]] = i; }
    return next; }

int main(){
    srand(time(nullptr));
    ifstream f("sortall.in");
    ofstream g("sortall.out");
#ifdef TEST
    for(int xxx = 0; ; ++xxx){
#endif
    int n;
#ifndef TEST
    f >> n;
    vector<int> s(n);
    for(auto& x : s) f >> x, --x;
#endif
#ifdef TEST
    n = 100;
    vector<int> s(n);
    for(auto& x : s) x = rand()%n;
#endif

    auto next = build_next(s);
    reverse(begin(s), end(s));
    auto prev = build_next(s);
    reverse(begin(s), end(s));
    reverse(begin(prev), end(prev));

    int delta = 0;
    vector<int> dst_(n, 1), ddr_(n, n);

    for(int i = 0; i < n; ++i)
        ddr_[s[i]] = min(ddr_[s[i]], i);

    auto set_dst = [&](int x, int y){ dst_[x] = y - delta; };
    auto set_ddr = [&](int x, int y){ ddr_[x] = y + delta; };

    vector<pair<int, int>> puncte;

    for(int i = 0; i < n; ++i)
        puncte.emplace_back(s[i], i);
    for(int i = 0; i < n; ++i)
        puncte.emplace_back(i, n);

    arbint2d a1(n, puncte), a2(n, puncte), a3(n, puncte), b(n, puncte), b_(n, puncte);

    long long rez = 0;

    for(int i = 0; i < n; ){
        if(i == 0){
            for(int j = 0; j < n; ++j){
                a1.update(j, ddr_[j], ((ll)dst_[j] * ddr_[j]) % mod);
                a2.update(j, ddr_[j], ((ddr_[j] - dst_[j])%mod + mod)%mod);
                a3.update(j, ddr_[j], 1);
                b.update(j, ddr_[j], dst_[j]);
                b_.update(j, ddr_[j], 1); } }
        long long here = 0;
        here += ((((ll)(s[i] + 1) * (i + 1))%mod) * next[i])%mod;
        here += mod - a1.query(0, s[i]-1, i, i + next[i] - 1);
        here += mod - ((ll)delta * a2.query(0, s[i]-1, i, i + next[i] - 1))%mod;
        here += ((((ll)delta * delta)%mod) * a3.query(0, s[i]-1, i, i + next[i] - 1))%mod;
        here -= (long long)next[i] * ((b.query(0, s[i]-1, i + next[i], n) + (long long)delta * b_.query(0, s[i]-1, i + next[i], n))%mod);
        here %= mod;

        rez += (long long)(s[i] + 1) * here;
        rez %= mod;

        a1.update(s[i], i + next[i], 0);
        a2.update(s[i], i + next[i], 0);
        a3.update(s[i], i + next[i], 0);
        b.update(s[i], i + next[i], 0);
        b_.update(s[i], i + next[i], 0);

        set_dst(s[i], 0);
        set_ddr(s[i], next[i]);

        a1.update(s[i], i + next[i], ((long long)dst_[s[i]] * ddr_[s[i]])%mod);
        a2.update(s[i], i + next[i], ddr_[s[i]] - dst_[s[i]]);
        a3.update(s[i], i + next[i], 1);
        b.update(s[i], i + next[i], dst_[s[i]]);
        b_.update(s[i], i + next[i], 1);

        ++i;
        delta += 1; }

    g << rez << endl;
#ifdef TEST
    cout << xxx << endl;
    assert(rez == brut(s)); }
#endif
    return 0; }
