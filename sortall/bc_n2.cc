#include <bits/stdc++.h>

using namespace std;

using u32 = unsigned;

const int kMaxN = 1e3;

int v[kMaxN];

u32 Divide(int left, int right) {
    if (left == right) {
        return v[left];
    } else {
        int middle = (left + right) / 2;
        u32 answer = Divide(left, middle) + Divide(middle + 1, right);

        vector<int> all_values;
        map<int, int> left_idx; 
        for (int i = middle; i >= left; --i) {
            if (left_idx.find(v[i]) == left_idx.end()) {
                left_idx[v[i]] = i;
                all_values.push_back(v[i]);
            }
        }

        map<int, int> right_idx;
        for (int i = middle + 1; i <= right; ++i) {
            if (right_idx.find(v[i]) == right_idx.end()) {
                right_idx[v[i]] = i;
                all_values.push_back(v[i]);
            }
        }

        sort(all_values.begin(), all_values.end());
        all_values.erase(unique(all_values.begin(), all_values.end()), all_values.end());

        for (auto&& value : all_values) {
            if (left_idx.find(value) == left_idx.end()) {
                left_idx[value] = left - 1;
            }

            if (right_idx.find(value) == right_idx.end()) {
                right_idx[value] = right + 1;
            }
        }

        for (int i = 0; i < (int)all_values.size(); ++i) {
            const int y = all_values[i];
            {
                u32 coef = 0;
                coef += (left_idx[y] - left + 1) * (right - middle);
                coef += (middle - left + 1) * (right - right_idx[y] + 1);
                coef -= (left_idx[y] - left + 1) * (right - right_idx[y] + 1);
                answer += y * coef;
            }

            for (int j = i + 1; j < (int)all_values.size(); ++j) {
                const int x = all_values[j];

                u32 coef = 0;
                coef += (min(left_idx[x], left_idx[y]) - left + 1) * (right - middle);
                coef += (left_idx[x] - left + 1) * (right - right_idx[y] + 1);
                coef -= (min(left_idx[x], left_idx[y]) - left + 1) * (right - right_idx[y] + 1);

                coef += (left_idx[y] - left + 1) * (right - right_idx[x] + 1);
                coef += (middle - left + 1) * (right - max(right_idx[x], right_idx[y]) + 1);
                coef -= (left_idx[y] - left + 1) * (right - max(right_idx[x], right_idx[y]) + 1);

                coef -= (min(left_idx[x], left_idx[y]) - left + 1) * (right - right_idx[x] + 1);
                coef -= (left_idx[x] - left + 1) * (right - max(right_idx[x], right_idx[y]) + 1);
                coef += (min(left_idx[x], left_idx[y]) - left + 1) * (right - max(right_idx[x], right_idx[y]) + 1);

                answer += coef * x;
            }   
        }

        return answer;
    }
}

int main() {
    int n; cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> v[i];
    }

    cout << Divide(0, n - 1) << endl;

    return 0;
}