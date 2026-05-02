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

class Solution {
// LeetCode method function
// void solve() {}
public:

};

#if !defined(CPTEST) && (defined(LOCAL) || defined(ONLINE_JUDGE))
void preprocess() {
    
}

int get_rank(char c) {
    if ('2' <= c && c <= '9') return c - '0';
    if (c == 'A') return 1;
    if (c == 'T') return 10;
    if (c == 'J') return 11;
    if (c == 'Q') return 12;
    if (c == 'K') return 13;
    return 0;
}

ll comb_sum(vi& cards, map<int,int>& f) {
    // get unique cards
    int n = (int)cards.size();
    vi vals(n, 0);
    REP(i, n) vals[i] = cards[i] >= 10 ? 10 : cards[i];
    sort(vals.begin(), vals.end());

    // dp[i][c] = # combinations ending at i that has sum = c
    vector<vector<ll>> dp(n+1, vector<ll>(20, 0));
    dp[0][0] = 1;

    for (int i = 1; i <= n; i++) {
        for (int c = vals[i-1]; c <= 15; c++) {
            for (int j = 0; j < i; j++) {
                if (dp[j][c-vals[i-1]] > 0) {
                    dp[i][c] += dp[j][c-vals[i-1]];
                    // dbg(i, c, dp[i][c]);
                }
            }
        }
    }
    ll res = 0;
    for (int i = 1; i <= n; i++) res += dp[i][15] * 2;
    return res;
}

ll pair_sum(vi& cards, map<int,int>& f) {
    ll res = 0;
    for (auto [k, v] : f) {
        int count = v * (v-1);
        res += count;
        // dbg(k, count);
    }
    return res;
}

ll run_sum(vi& cards, map<int,int>& f) {
    // get unique cards
    set<int> uq(cards.begin(), cards.end());
    vi cuq(uq.begin(), uq.end());
    int m = (int)cuq.size();

    vector<pii> runs;
    int start = cuq[0];
    for (int i = 1; i < m; i++) {
        if (cuq[i] == cuq[i-1] + 1) continue;
        else {
            runs.pb({start, cuq[i-1]});
            start = cuq[i];
        }
    }
    runs.pb({start, cuq[m-1]});

    // dbg(runs);
    ll res = 0;
    for (auto& p : runs) {
        int L = p.fi, R = p.se;
        int sz = R - L + 1;
        if (sz >= 3) {
            ll S = 1;
            for (int i = L; i <= R; i++) {
                S *= f[i];
            }
            res += S * sz;
        }
    }
    return res;
}

// cout << Solution().solve() << '\n';
void solve() {
    int n; cin >> n;

    vi cards(n);
    map<int,int> f;
    REP(i, n) {
        char c; cin >> c;
        cards[i] = get_rank(c);
        f[cards[i]]++;
    }
    sort(cards.begin(), cards.end());

    ll S_comb = comb_sum(cards, f);
    ll S_pair = pair_sum(cards, f);
    ll S_run = run_sum(cards, f);

    // dbg(S_comb, S_pair, S_run);

    ll ans = S_comb + S_pair + S_run;
    cout << ans << '\n';
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