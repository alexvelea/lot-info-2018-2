#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <limits>
#include <queue>
#include <cassert>
using namespace std;

const int MAX_IT = 305;

int main() {
    srand(time(0));

    ifstream cin("trenbus.in");
    ofstream cout("trenbus.out");

    int n; cin >> n;
        
    vector<vector<vector<pair<int, int>>>> g(2, vector<vector<pair<int, int>>> (n));

    for(int t = 0; t < 2; ++t) {
        for(int i = 0; i < n - 1; ++i) {
            int a, b, c; cin >> a >> b >> c;
            a -= 1, b -= 1;
            g[t][a].push_back(make_pair(b, c));
            g[t][b].push_back(make_pair(a, c));
        }
    }
    
    auto getDistances = [&] (int start, int t) {
        queue<int> q;
        q.push(start);
        vector<int> ans(n, numeric_limits<int>::max());
        ans[start] = 0;

        while(not q.empty()) {
            int node = q.front();
            q.pop();

            for(const auto& temp : g[t][node]) {
                if(max(ans[node], temp.second) < ans[temp.first]) {
                    ans[temp.first] = max(ans[node], temp.second);
                    q.push(temp.first);
                }
            }
        }

        return ans;
    };
    
    int start = rand() % n;
    int node = start;
    
    int ans = numeric_limits<int>::max();
    
    vector<bool> has(n, 0);
    vector<int> added(n + 10, 0);

    for(int it = 0; it < MAX_IT; ++it) {
        added[++added[0]] = node;
        has[node] = 1;

        auto first = getDistances(node, 0);
        auto second = getDistances(node, 1);

        int index = -1;
        
        int temp = numeric_limits<int>::max();

        for(int i = 0; i < n; ++i) {
            if(i == node)
                continue;

            if(first[i] + second[i] <= ans) {
                ans = first[i] + second[i];
            }

            if(first[i] + second[i] <= temp) {
                temp = first[i] + second[i];
                index = i;
            }
        }
        
        if(has[index]) {
            node = rand() % n;
            for(int i = 1; i <= added[0]; ++i)
                has[added[i]] = 0;
            added[0] = 0;
        } else {
            node = index;   
        }
    }

    cout << ans << "\n";
}
