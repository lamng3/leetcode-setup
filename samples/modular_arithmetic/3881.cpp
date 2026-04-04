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

ll binpow(ll a, ll b) {
    a %= MOD;
    ll res = 1;
    while (b) {
        if (b % 2) res = (res * a) % MOD;
        a = (a * a) % MOD;
        b >>= 1;
    }
    return res;
}

ll modInverse(ll a) {
    return binpow(a, MOD-2);
}

struct ModFact {
    int n;
    vector<ll> fac, inv;

    ModFact(int _n) : n(_n) {
        fac.resize(n+1);
        inv.resize(n+1);
        fac[0] = 1;
        for (int i = 1; i <= n; i++) fac[i] = (fac[i-1] * i) % MOD;
        // inv[n] = (1 * 2 * ... * n) ^ (-1)
        // inv[n-1] = inv[n] * n
        inv[n] = modInverse(fac[n]);
        for (int i = n; i >= 1; i--) inv[i-1] = (inv[i] * i) % MOD;
    }

    ll nCk(int n, int k) {
        if (n < k || n < 0 || k < 0) return 0;
        return fac[n] * inv[k] % MOD * inv[n-k] % MOD;
    }
};

class Solution {
// LeetCode method function
// void solve() {}
public:
    int countVisiblePeople(int n, int pos, int k) {
        ModFact mf(n);
        ll ans = mf.nCk(n-1, k);
        ans = (ans * 2) % MOD;
        return (int)ans;
    }
};

#if defined(LOCAL) || defined(ONLINE_JUDGE)
void preprocess() {
    
}

// cout << Solution().solve() << '\n';
void solve() {
    int n, pos, k;
    cin >> n >> pos >> k;
    cout << Solution().countVisiblePeople(n, pos, k) << '\n';
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