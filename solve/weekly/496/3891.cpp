// g++ -std=c++17 -DLOCAL template.cpp -o solution
#include <bits/stdc++.h>
using namespace std;

#pragma region Debug
#ifdef LOCAL
template<typename T, typename U>
ostream& operator<<(ostream& os, const pair<T,U>& p) {
    return os << "(" << p.first << ", " << p.second << ")";
}
template<typename T>
ostream& operator<<(ostream& os, const vector<T>& v) {
    os << "[";
    for (int i = 0; i < (int)v.size(); i++) os << (i ? ", " : "") << v[i];
    return os << "]";
}
template<typename T>
ostream& operator<<(ostream& os, const set<T>& s) {
    os << "{";
    int i = 0;
    for (auto& x : s) os << (i++ ? ", " : "") << x;
    return os << "}";
}
template<typename K, typename V>
ostream& operator<<(ostream& os, const map<K,V>& m) {
    os << "{";
    int i = 0;
    for (auto& [k, v] : m) os << (i++ ? ", " : "") << k << ": " << v;
    return os << "}";
}
void _dbg() { cerr << endl; }
template<typename T, typename... A>
void _dbg(T t, A... a) { cerr << " " << t; if constexpr(sizeof...(a)) cerr << ","; _dbg(a...); }
#define dbg(...) cerr << "\033[35m[" << #__VA_ARGS__ << "]\033[0m:", _dbg(__VA_ARGS__)
#else
#define dbg(...)
#endif
#pragma endregion

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

// 3891. Minimum Increase to Maximize Special Indices [Medium]
class Solution {
public:
    ll minIncrease(vi& nums) {
        /*
            at most floor(n/2) special
            if there are more, supposed floor(n/2)+1, 
                then there are at least floor(n/2)+1 numbers that are < that special pairwise
                => there will be (2 * floor(n/2) + 2) > n (contradicts!)
            we can definitely have floor(n/2) special indices
                our task now become what is the minimum number of operations needed?
        */
        int n = (int)nums.size();

        if (n % 2) {
            ll ans = 0;
            for (int i = 1; i < n-1; i+=2) {
                int cost = max(0, max(nums[i-1], nums[i+1]) + 1 - nums[i]);
                ans += cost;
            }
            return ans;
        }

        int K = (n-2)/2;

        // can only change at most 1 time to move from odd to even
        // 1 degree of freedom
        vector<ll> pref(K+1);
        pref[0] = 0;
        for (int i = 1; i < n-1; i+=2) {
            int cost = max(0, max(nums[i-1], nums[i+1]) + 1 - nums[i]);
            pref[i/2+1] += pref[i/2] + cost;
        }
        // dbg(pref);

        vector<ll> suff(K+1);
        suff[0] = 0;
        for (int i = n-2; i > 0; i-=2) {
            int cost = max(0, max(nums[i-1], nums[i+1]) + 1 - nums[i]);
            suff[K-(i/2)+1] += suff[K-(i/2)] + cost;
        }
        // dbg(suff);

        ll ans = LLINF;
        for (int i = 0; i <= K; i++) {
            ans = min(ans, pref[i] + suff[K-i]);
        }
        return ans;
    }
};

#if !defined(CPTEST) && (defined(LOCAL) || defined(ONLINE_JUDGE))
void preprocess() {
    
}

// cout << Solution().solve() << '\n';
void solve() {
    
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