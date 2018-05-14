#include <bits/stdc++.h>
 
using namespace std;
 
const int kInf = 0x3f3f3f3f;

class ModInteger {
  public:
    ModInteger(const int n=0): n_((kMod + n % kMod) % kMod) {}
    ModInteger& operator +=(const ModInteger& rhs) {
        n_ += rhs.n_;
        if (n_ >= kMod) {
            n_ -= kMod;
        }
        return *this;
    }

    ModInteger& operator -=(const ModInteger& rhs) {
        n_ -= rhs.n_;
        if (n_ < 0) {
            n_ += kMod;
        }
        return *this;
    }

    ModInteger& operator *=(const ModInteger& rhs) {
        n_ = (long long)n_ * rhs.n_ % kMod;
        return *this;
    }

    ModInteger operator +(const ModInteger& rhs) const {
        return ModInteger(*this) += rhs;
    }

    ModInteger operator *(const ModInteger& rhs) const {
        return ModInteger(*this) *= rhs;
    }

    ModInteger operator -(const ModInteger& rhs) const {
        return ModInteger(*this) -= rhs;
    }

    ModInteger operator -() const {
        return ModInteger(kMod - n_);
    }

    friend ostream& operator <<(ostream& os, const ModInteger& rhs) {
        return os << rhs.n_;
    }
  private:
    static constexpr int kMod = 1e9 + 7;
    int n_;
};

using cost_t = ModInteger;

struct NodeInfo {
    cost_t x, x_left, x_right, x_left_right;
 
    NodeInfo(const cost_t x_=0, const cost_t x_left_=0, const cost_t x_right_=0, const cost_t x_left_right_=0)
        : x(x_), x_left(x_left_), x_right(x_right_), x_left_right(x_left_right_) { }
 
    NodeInfo(const tuple<int, int, int>& point) : x(cost_t(get<0>(point))), 
        x_left(cost_t(get<0>(point)) * get<1>(point)), x_right(cost_t(get<0>(point)) * get<2>(point)),
        x_left_right(cost_t(get<0>(point)) * get<1>(point) * get<2>(point)) { }
     
    NodeInfo& operator +=(const NodeInfo& rhs) {
        x += rhs.x;
        x_left += rhs.x_left;
        x_right += rhs.x_right;
        x_left_right += rhs.x_left_right;
        return *this;
    }
    NodeInfo operator +(const NodeInfo& rhs) const {
        return NodeInfo(*this) += rhs;
    }
 
    NodeInfo operator -() const {
        return NodeInfo(-x, -x_left, -x_right, -x_left_right);
    }
};
 
struct KDInfo {
    int mn_x, mn_y, mx_x, mx_y;
    KDInfo() : mn_x(kInf), mn_y(kInf), mx_x(-kInf), mx_y(-kInf) { }
    KDInfo(const tuple<int, int, int>& p) : mn_x(get<1>(p)), mn_y(get<2>(p)), mx_x(get<1>(p)), mx_y(get<2>(p)) { }
 
    KDInfo& operator +=(const KDInfo& rhs) {
        mn_x = min(mn_x, rhs.mn_x);
        mn_y = min(mn_y, rhs.mn_y);
        mx_x = max(mx_x, rhs.mx_x);
        mx_y = max(mx_y, rhs.mx_y);
        return *this;
    }
    KDInfo operator +(const KDInfo& rhs) const {
        return KDInfo(*this) += rhs;
    }
 
    bool outside(const int x0, const int y0, const int x1, const int y1) const {
        return (mn_x > x1 or x0 > mx_x or mn_y > y1 or y0 > mx_y);
    }
 
    bool inside(const int x0, const int y0, const int x1, const int y1) const {
        return (x0 <= mn_x and mx_x <= x1 and y0 <= mn_y and mx_y <= y1);
    }
};
 
class KDTree {
  public:
    KDTree(vector<tuple<int, int, int>> points) 
            : point_(points.size()), node_info_(points.size()), kd_info_(points.size()) { 
        Build(points, 0, (int)points.size() - 1);
    }
 
    void Erase(const tuple<int, int, int>& point) {
        DeletePoint(0, (int)point_.size() - 1, false, point);
    }
 
    NodeInfo Query(const int x0, const int y0, const int x1, const int y1) {
        return Count(0, (int)point_.size() - 1, x0, y0, x1, y1);
    }
 
  private:
    KDTree() = delete;
 
    void DeletePoint(int low, int high, bool dir, const tuple<int, int, int>& point) {
        if (low > high) {
            return;
        }
 
        int middle = (low + high) / 2;
        node_info_[middle] += -NodeInfo(point);
        if (point_[middle] == point) {
            get<0>(point_[middle]) = 0;
            return;
        } else if ((!dir and (get<2>(point) < get<2>(point_[middle]) or (get<2>(point) == get<2>(point_[middle]) and get<1>(point) < get<1>(point_[middle])))) 
                 or (dir and (get<1>(point) < get<1>(point_[middle]) or (get<1>(point) == get<1>(point_[middle]) and get<2>(point) < get<2>(point_[middle]))))) {
            DeletePoint(low, middle - 1, !dir, point);
        } else {
            DeletePoint(middle + 1, high, !dir, point);
        }
    }
 
    NodeInfo Count(int low, int high, const int x0, const int y0, const int x1, const int y1) {
        NodeInfo ans;
        if (low > high) {
            return ans;
        }
 
        int middle = (low + high) / 2;
        if (kd_info_[middle].outside(x0, y0, x1, y1)) {
            return ans;
        }
        if (kd_info_[middle].inside(x0, y0, x1, y1)) {
            return node_info_[middle];
        }
 
        ans += Count(low, middle - 1, x0, y0, x1, y1);
        ans += Count(middle + 1, high, x0, y0, x1, y1);
 
        if (x0 <= get<1>(point_[middle]) and get<1>(point_[middle]) <= x1
                and y0 <= get<2>(point_[middle]) and get<2>(point_[middle]) <= y1) {
            ans += NodeInfo(point_[middle]);
        }
        return ans;
    }
 
    void Build(vector<tuple<int, int, int>>& points, int low, int high, bool dir=false)  {
        if (low > high) {
            return;
        }
 
        int middle = (low + high) / 2;
        if (dir) {
            nth_element(points.begin() + low, points.begin() + middle, points.begin() + high + 1, 
                    [&](const tuple<int, int, int>& a, const tuple<int, int, int>& b) {
                return (get<1>(a) < get<1>(b)) or (get<1>(a) == get<1>(b) and get<2>(a) < get<2>(b));
            });
        } else {
            nth_element(points.begin() + low, points.begin() + middle, points.begin() + high + 1, 
                    [&](const tuple<int, int, int>& a, const tuple<int, int, int>& b) {
                return (get<2>(a) < get<2>(b)) or (get<2>(a) == get<2>(b) and get<1>(a) < get<1>(b));
            });
        }
 
        point_[middle] = points[middle];
        for (int i = low; i <= high; ++i) {
            kd_info_[middle] += KDInfo(points[i]);
            node_info_[middle] += NodeInfo(points[i]);
        }
 
        Build(points, low, middle - 1, !dir);
        Build(points, middle + 1, high, !dir);
    }
 
    vector<tuple<int, int, int>> point_;
    vector<NodeInfo> node_info_;
    vector<KDInfo> kd_info_;
};
 
template<typename T>
cost_t Solve(T begin, T end) {
    if (begin + 1 == end) {
        return cost_t(*begin);
    } else {
        size_t length = end - begin;
        cost_t answer = Solve(begin, begin + length / 2) + Solve(begin + length / 2, end);
 
        vector<int> all_values;
        map<int, int> left_idx; 
        for (int i = length / 2 - 1; i >= 0; --i) {
            if (left_idx.find(begin[i]) == left_idx.end()) {
                left_idx[begin[i]] = i + 1;
                all_values.push_back(begin[i]);
            }
        }
 
        map<int, int> right_idx;
        for (int i = length / 2; i < int(length); ++i) {
            if (right_idx.find(begin[i]) == right_idx.end()) {
                right_idx[begin[i]] = length - i;
                all_values.push_back(begin[i]);
            }
        }
 
        sort(all_values.begin(), all_values.end());
        all_values.erase(unique(all_values.begin(), all_values.end()), all_values.end());
 
        vector<tuple<int, int, int>> points;
        points.reserve(all_values.size());
        for (auto&& value : all_values) {
            if (left_idx.find(value) == left_idx.end()) {
                left_idx[value] = 0;
            }
 
            if (right_idx.find(value) == right_idx.end()) {
                right_idx[value] = 0;
            }
            points.emplace_back(value, left_idx[value], right_idx[value]);
        }
 
        KDTree tree(points);
 
        for (int i = 0; i < (int)all_values.size(); ++i) {
            const int y = all_values[i];
            {
                cost_t coef = 0;
                coef += left_idx[y] * ((length + 1) / 2);
                coef += (length / 2) * right_idx[y];
                coef -= left_idx[y] * right_idx[y];
                answer += coef * y;
            }
             
            tree.Erase(points[i]);
 
            cost_t sum_left = tree.Query(0, 0, length / 2, (length + 1) / 2).x_left;
            cost_t sum_right = tree.Query(0, 0, length / 2, (length + 1) / 2).x_right;
 
            cost_t sum_min_left = tree.Query(0, 0, left_idx[y] - 1, (length + 1) / 2).x_left 
                             + tree.Query(left_idx[y], 0, length / 2, (length + 1) / 2).x * left_idx[y];
             
            cost_t sum_min_right = tree.Query(0, 0, length / 2, right_idx[y] - 1).x_right 
                              + tree.Query(0, right_idx[y], length / 2, (length + 1) / 2).x * right_idx[y];
 
            answer += sum_min_left * ((length + 1) / 2);
            answer += sum_left * right_idx[y];
            answer -= sum_min_left * right_idx[y];
 
            answer += sum_right * left_idx[y]; 
            answer += sum_min_right * (length / 2);
            answer -= sum_min_right * left_idx[y];
 
            answer -= tree.Query(0, 0, left_idx[y] - 1, (length + 1) / 2).x_left_right 
                    + tree.Query(left_idx[y], 0, length / 2, (length + 1) / 2).x_right * left_idx[y];
             
            answer -= tree.Query(0, 0, length / 2, right_idx[y] - 1).x_left_right 
                    + tree.Query(0, right_idx[y], length / 2, (length + 1) / 2).x_left * right_idx[y];
 
            answer += tree.Query(0, 0, left_idx[y] - 1, right_idx[y] - 1).x_left_right
                    + tree.Query(left_idx[y], 0, length / 2, right_idx[y] - 1).x_right * left_idx[y]
                    + tree.Query(0, right_idx[y], left_idx[y] - 1, (length + 1) / 2).x_left * right_idx[y]
                    + tree.Query(left_idx[y], right_idx[y], length / 2, (length + 1) / 2).x * left_idx[y] * right_idx[y];
        }
 
        return answer;
    }
}
 
int main() {
    int n; cin >> n;
    vector<int> v(n);
    for (auto& iter : v) {
        cin >> iter;
    }
 
    cout << Solve(v.begin(), v.end()) << endl;
}
