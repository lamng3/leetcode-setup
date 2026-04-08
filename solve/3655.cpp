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

ll binpow(ll a, ll b) {
    a %= MOD;
    ll r = 1;
    while (b) {
        if (b % 2) r = (r * a) % MOD;
        a = (a * a) % MOD;
        b >>= 1;
    }
    return r;
}

ll modInverse(ll x) {
    return binpow(x, MOD-2);
}

// 3655. XOR After Range Multiplication Queries II [Hard]
class Solution {
public:
    int xorAfterQueries(vi& nums, vii& queries) {
        int n = (int)nums.size();
        int sqrtn = sqrt(n);

        vii M(sqrtn, vi(n, 1));

        for (auto& q : queries) {
            int L = q[0], R = q[1], k = q[2], v = q[3];
            if (k >= sqrtn) {
                for (int i = L; i <= R; i+=k) {
                    nums[i] = ((ll)nums[i] * v) % MOD;
                }
            }
            else {
                int invv = modInverse((ll)v);
                M[k][L] = ((ll)M[k][L] * v) % MOD;
                int E = L + ((R-L)/k+1)*k;
                if (E < n) M[k][E] = ((ll)M[k][E] * invv) % MOD;
            }
        }

        for (int k = 1; k < sqrtn; k++) {
            for (int i = k; i < n; i++) {
                M[k][i] = ((ll)M[k][i] * M[k][i-k]) % MOD;
            }
            for (int i = 0; i < n; i++) {
                nums[i] = ((ll)nums[i] * M[k][i]) % MOD;
            }
        }

        int ans = 0;
        for (int x : nums) ans ^= x;
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