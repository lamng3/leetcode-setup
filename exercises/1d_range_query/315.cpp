// g++ -std=c++17 -DLOCAL template.cpp -o solution
#include <bits/stdc++.h>
using namespace std;

template<typename T>
ostream& operator<<(ostream& os, const vector<T>& v) {
    os << "[";
    for (int i = 0; i < v.size(); i++) {
        os << v[i] << (i == v.size() - 1 ? "" : ", ");
    }
    return os << "]";
}

using ll = long long;
using vi = vector<int>;
using vii = vector<vector<int>>;
using pii = pair<int, int>;

#define REP(i, n) for (int i = 0; i < (n); i++)
#define FOR(i, a, b) for (int i = (a); i <= (b); i++)
#define FORD(i, a, b) for (int i = (a); i >= (b); i--)
#define RFOR(i, n) for (int i = (n) - 1; i >= 0; i--)

#define fi first
#define se second
#define pb push_back

const int INF = 1e9;
const ll LLINF = 2e18;

const int MOD = 1e9+7;
const int MOD_NTT = 998244353; // number theoretic transform (NTT)

template <typename T>
struct FenwickTree {
    int n;
    vector<T> ft;
    FenwickTree(int _n) : n(_n), ft(_n + 1, 0) {}
    FenwickTree(const vector<T>& a) {
        init(a);
    }
    void init(const vector<T>& a) {
        n = a.size();
        ft.assign(n+1, 0);
        for (int i = 1; i <= n; i++) add(i, a[i-1]);
    }
    void add(int i, T u) {
        for (; i <= n; i += i & -i) ft[i] += u;
    }
    T sum(int i) {
        T s = 0;
        for (; i > 0; i -= i & -i) s += ft[i];
        return s;
    }
    T sum(int L, int R) {
        if (L > R) return 0;
        return sum(R) - sum(L-1);
    }
};

class Solution {
// LeetCode method function
// void solve() {}
public:
    vi countSmaller(vi& nums) {
        int n = (int)nums.size();

        // coordinate compression
        set<int> uq(nums.begin(), nums.end());
        vi a(uq.begin(), uq.end());
        map<int,int> compress;
        for (int i = 0; i < a.size(); i++) compress[a[i]] = i+1;

        // compute ranking
        vi rank(n);
        for (int i = 0; i < n; i++) rank[i] = compress[nums[i]];
        
        FenwickTree<int> ft(a.size());

        vi counts(n, 0);
        for (int i = n-1; i >= 0; i--) {
            counts[i] = ft.sum(rank[i]-1);
            ft.add(rank[i], 1);
        }
        return counts;
    }
};

#if defined(LOCAL) || defined(ONLINE_JUDGE)
void preprocess() {
    
}

// cout << Solution().solve() << '\n';
void solve() {
    int n; cin >> n;
    vi nums(n);
    REP(i, n) cin >> nums[i];
    cout << Solution().countSmaller(nums) << '\n';
}

int main() {
    // freopen("name.in", "r", stdin);
    // freopen("name.out", "w", stdout);
    ios::sync_with_stdio(0);
    cin.tie(0);
    preprocess();
    int tt = 1;
    // cin >> tt;
    while (tt--) solve();
    return 0;
}
#endif