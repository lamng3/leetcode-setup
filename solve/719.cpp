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

class Solution {
// LeetCode method function
// void solve() {}
public:
    int smallestDistancePair(vi& nums, int k) {
        int n = (int)nums.size();
        sort(nums.begin(), nums.end());

        int L = 0, R = 1e6;

        while (L < R) {
            int X = L + (R - L) / 2;
            int count = 0;
            int j = 0;
            for (int i = 0; i < n; i++) {
                while (nums[i] - nums[j] > X) {
                    j++;
                }
                count += i - j;
            }
            if (count < k) L = X + 1;
            else R = X;
        }

        return L;
    }
};

#if defined(LOCAL) || defined(ONLINE_JUDGE)
void preprocess() {

}

// cout << Solution().solve() << '\n';
void solve() {
    int n, k;
    cin >> n;
    vi nums(n);
    REP(i, n) cin >> nums[i];
    cin >> k;
    cout << Solution().smallestDistancePair(nums, k) << '\n';
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
