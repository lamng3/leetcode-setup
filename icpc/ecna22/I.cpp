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

struct Edge {
    int to, len, id;
};

int N, M;
vector<vector<Edge>> g;
vi d;

int dijkstra(int a, int b) {
    // find shortest path between a and b
    d.assign(N+1, INF);
    d[a] = 0;

    priority_queue<pii, vector<pii>, greater<pii>> pq;
    pq.push({0, a});

    while (!pq.empty()) {
        auto p = pq.top();
        pq.pop();

        // current node
        int du = p.fi, u = p.se;
        if (du > d[u]) continue;

        if (u == b) break;
        
        // go from u to v
        for (auto edge : g[u]) {
            int v = edge.to, len = edge.len;
            
            if (du + len < d[v]) {
                d[v] = du + len;
                pq.push({d[v], v});
            }
        }
    }

    return d[b];
}

// cout << Solution().solve() << '\n';
void solve() {
    int n, m, a, b;
    cin >> n >> m >> a >> b;

    N = n; M = m;
    g.assign(n+1, vector<Edge>());

    int tot = 0;

    REP(i, m) {
        int u, v, l;
        cin >> u >> v >> l;
        g[u].pb({v, l, i});
        g[v].pb({u, l, i});
        tot += l;
    }

    int minlen = dijkstra(a, b);
    
    vi paved(M, 0);
    vi vis(N+1, 0);

    queue<int> q;
    q.push(b);

    int paved_len = 0;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (auto edge : g[u]) {
            int v = edge.to, len = edge.len, id = edge.id;

            if (d[v] + len == d[u]) {
                if (!paved[id]) {
                    paved[id] = 1;
                    paved_len += len;
                }
                if (!vis[v]) {
                    vis[v] = 1;
                    q.push(v);
                }
            }
        }
    }

    cout << (tot - paved_len) << '\n';
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