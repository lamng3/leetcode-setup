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

const int MAXW = 1e6+5;

int trie[MAXW][26];
int nextnode;
int isleaf[MAXW];

// cout << Solution().solve() << '\n';
void solve() {
    memset(trie, -1, sizeof trie);
    nextnode = 1;
    memset(isleaf, 0, sizeof isleaf);

    auto addWord = [&](const string& w) {
        int i = 0, v = 0;
        while (i < w.size()) {
            if (trie[v][w[i]-'a'] == -1) {
                v = trie[v][w[i++]-'a'] = nextnode++;
            }
            else {
                v = trie[v][w[i++]-'a'];
            }
        }
        isleaf[v] = 1;
    };

    // auto searchWord = [&](const string& w) {
    //     int i = 0, v = 0;
    //     while (i < w.size()) {
    //         if (trie[v][w[i]-'a'] == -1) {
    //             return 0;
    //         }
    //         v = trie[v][w[i++]-'a'];
    //     }
    //     return isleaf[v];
    // };

    string s; cin >> s;

    int k; cin >> k;
    REP(i, k) {
        string w; cin >> w;
        addWord(w);
    }

    int n = (int)s.size();
    
    vector<ll> dp(n+1, 0);
    dp[0] = 1;

    for (int L = 0; L < n; L++) {
        if (dp[L] == 0) continue;
        int v = 0;
        // s[L..R]
        for (int R = L+1; R <= n; R++) {
            int c = s[R - 1] - 'a';
            if (trie[v][c] == -1) break; 
            v = trie[v][c];
            if (isleaf[v]) dp[R] = (dp[R] + dp[L]) % MOD;
        }
    }

    cout << dp[n] << '\n';
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