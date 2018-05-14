#include <bits/stdc++.h>
#include "mere.h"

using namespace std;

const int NMAX   = 200000;
const int VALMAX = 1000000000;
const int MOD    = 1000000000 + 7;

void add(int &where, const int val) {
    where += val;
    if (where >= MOD)
        where -= MOD;
}

typedef long long int lint;

int N, V1, V2;
bool canReachV1(const int t0, const int x0, const int t1, const int x1) {
    //assert(t0 <= t1);
    return x0 + 1LL * t0 * V1 <= x1 + 1LL * t1 * V1 &&
           1LL * t0 * V1 - x0 <= 1LL * t1 * V1 - x1;
}
bool canReachV2(const int t0, const int x0, const int t1, const int x1) {
    //assert(t0 <= t1);
    return x0 + 1LL * t0 * V2 <= x1 + 1LL * t1 * V2 &&
           1LL * t0 * V2 - x0 <= 1LL * t1 * V2 - x1;
}

lint x1[NMAX + 5], y1_[NMAX + 5];
lint x2[NMAX + 5], y2[NMAX + 5];

template <typename T>
inline void do_unique(vector <T> &v) {
    sort(v.begin(), v.end());
    v.resize(unique(v.begin(), v.end()) - v.begin());
}

template <typename T>
inline int upper_search(vector <T> &v, const T val) {
    return upper_bound(v.begin(), v.end(), val) - v.begin();
}

vector <lint> all_x1, all_y1, all_x2, all_y2;

inline int lsb(int where) {
    return where & (-where);
}

struct AIB {
    vector <int> all_pos, aib;
    void build() {
        do_unique(all_pos);
        aib.resize(all_pos.size() + 1);
    }
    void fakeUpdate(const int where) { all_pos.push_back(where); }
    void update(const int where, const int val) {
        int pos = upper_search(all_pos, where);
        for (; pos < aib.size(); pos += lsb(pos))
            add(aib[pos], val);
    }
    int query(const int where) {
        int pos = upper_search(all_pos, where);
        int ans = 0;
        for (; pos; pos -= lsb(pos))
            add(ans, aib[pos]);
        return ans;
    }
};

struct AIB2D {
    vector <AIB> aib;
    void init(int N) { aib.resize(N + 1); }
    void build() { for (auto &it: aib) it.build(); }
    void fakeUpdate(int x, const int y) { for (; x < aib.size(); x += lsb(x)) aib[x].fakeUpdate(y); }
    void update(int x, int y, const int val) {
        for (; x < aib.size(); x += lsb(x))
            aib[x].update(y, val);
    }
    int query(int x, int y) {
        int ans = 0;
        for (; x; x -= lsb(x))
            add(ans, aib[x].query(y));
        return ans;
    }
};

int X[NMAX + 15], T[NMAX + 15];

int dp1[NMAX + 15]; // dp1[i] = The second player is at i, 0 otherwise (# of ways)
int dp2[NMAX + 15]; // dp2[i] = The first  player is at i, 0 otherwise (# of ways)

AIB2D aib1, aib2;

int getCnt1(const int Xbound, const int Ybound) {
    //assert(Xbound >= 0 && Ybound >= 0);
    if (Xbound < 0 || Ybound < 0)
        return 0;
    return aib2.query(Xbound, Ybound);
    /*int ans = 0;
    for (int j = 1; j <= N; ++ j)
        if (x1[j] <= Xbound && y1[j] <= Ybound)
            add(ans, dp2[j]);
    return ans;*/
}

int getCnt2(const int Xbound, const int Ybound) {
    //assert(Xbound >= 0 && Ybound >= 0);
    if (Xbound < 0 || Ybound < 0)
        return 0;
    return aib1.query(Xbound, Ybound);
    /*int ans = 0;
    for (int j = 1; j <= N; ++ j)
        if (x2[j] <= Xbound && y2[j] <= Ybound)
            add(ans, dp1[j]);
    return ans;*/
}

inline int getOpposite(int nr) {
    if (nr == 0)
        return 0;
    else
        return MOD - nr;
}

void reset1(const int l, const int r) {
    for (int i = l; i <= r; ++ i) {
        aib1.update(x2[i], y2[i], getOpposite(dp1[i]));
        dp1[i] = 0;
    }
}

void reset2(const int l, const int r) {
    for (int i = l; i <= r; ++ i) {
        aib2.update(x1[i], y1_[i], getOpposite(dp2[i]));
        dp2[i] = 0;
    }
}

void setVal1(const int where, const int val) {
    dp1[where] = val;
    aib1.update(x2[where], y2[where], dp1[where]);
}

inline void fakeSetVal1(const int where) { aib1.fakeUpdate(x2[where], y2[where]); }

void setVal2(const int where, const int val) {
    dp2[where] = val;
    aib2.update(x1[where], y1_[where], dp2[where]);
}

inline void fakeSetVal2(const int where) { aib2.fakeUpdate(x1[where], y1_[where]); }

int count(int _N, int _VA, int _VB, int *_X, int *_T) {
    // Check input for validity and create data copies
    assert(1 <= _N && _N <= NMAX);
    assert(1 <= _VA && _VA <= VALMAX);
    assert(1 <= _VB && _VB <= VALMAX);
    N = _N, V1 = _VA, V2 = _VB;

    for (int i = 0; i < _N; ++ i) {
        assert(-VALMAX <= _X[i] && _X[i] <= VALMAX);
        assert(      1 <= _T[i] && _T[i] <= VALMAX);
        X[i + 3] = _X[i], T[i + 3] = _T[i];
    }
    for (int i = 0; i + 1 < _N; ++ i)
        assert(_T[i] < _T[i + 1]);

    // Initialization
    N += 2;

    for (int i = 1; i <= N; ++ i) {
        x1[i] = X[i] + 1LL * T[i] * V1;
        y1_[i] = 1LL * T[i] * V1 - X[i];
        x2[i] = X[i] + 1LL * T[i] * V2;
        y2[i] = 1LL * T[i] * V2 - X[i];
        all_x1.push_back(x1[i]);
        all_y1.push_back(y1_[i]);
        all_x2.push_back(x2[i]);
        all_y2.push_back(y2[i]);
    }

    do_unique(all_x1), do_unique(all_y1), do_unique(all_x2), do_unique(all_y2);
    for (int i = 1; i <= N; ++ i) {
        x1[i] = upper_search(all_x1, x1[i]);
        y1_[i] = upper_search(all_y1, y1_[i]);
        x2[i] = upper_search(all_x2, x2[i]);
        y2[i] = upper_search(all_y2, y2[i]);
    }

    // Create aibs
    aib1.init(all_x2.size());
    aib2.init(all_x1.size());

    // Do all fake updates
    for (int i = 1; i <= N; ++ i)
        fakeSetVal1(i), fakeSetVal2(i);

    aib1.build();
    aib2.build();

    setVal1(1, 0);
    setVal1(2, 0);
    setVal2(1, 1);
    setVal2(2, 0);

    dp2[1] = 1;

    int res1 = 1, res2 = 1;
    for (int i = 3; i <= N; ++ i) {
        const lint Xbound1 = X[i] + 1LL * T[i] * V1;
        const lint Ybound1 = 1LL * T[i] * V1 - X[i];
        int ans1 = getCnt1(upper_search(all_x1, Xbound1), upper_search(all_y1, Ybound1));

        const lint Xbound2 = X[i] + 1LL * T[i] * V2;
        const lint Ybound2 = 1LL * T[i] * V2 - X[i];
        int ans2 = getCnt2(upper_search(all_x2, Xbound2), upper_search(all_y2, Ybound2));

        if (!canReachV1(T[i - 1], X[i - 1], T[i], X[i]))
            reset1(res1, i - 2), res1 = i - 1;
        if (!canReachV2(T[i - 1], X[i - 1], T[i], X[i]))
            reset2(res2, i - 2), res2 = i - 1;

        setVal1(i - 1, ans1), setVal2(i - 1, ans2);
    }

    int ans = 0;
    for (int i = 1; i < N; ++ i)
        add(ans, dp1[i]), add(ans, dp2[i]);
    return ans;
}
