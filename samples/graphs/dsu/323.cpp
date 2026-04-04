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

struct DSU {
    int n;
    vector<int> parent, sz;
    DSU(int _n) : n(_n) {
        sz.assign(n, 1);
        parent.resize(n);
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    int find(int v) {
        if (v == parent[v]) return v;
        // path compression
        return parent[v] = find(parent[v]);
    }
    void unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return;
        if (sz[a] < sz[b]) swap(a, b);
        parent[b] = a;
        sz[a] += sz[b];
    }
};

class Solution {
// LeetCode method function
// void solve() {}
public:
    int countComponents(int n, vii& edges) {
        DSU dsu(n);
        for (auto& e : edges) {
            int a = e[0], b = e[1];
            dsu.unite(a, b);
        }
        unordered_set<int> components;
        for (int i = 0; i < n; i++) {
            int pi = dsu.find(i);
            components.insert(pi);
        }
        return (int)components.size();
    }
};

#if defined(LOCAL) || defined(ONLINE_JUDGE)
void preprocess() {
    
}

// cout << Solution().solve() << '\n';
void solve() {
    int n; 
    cin >> n;
    int m;
    cin >> m;
    vii edges(m, vi(2));
    for (int i = 0; i < m; i++) {
        int a, b; 
        cin >> a >> b;
        edges.pb({a, b});
    }
    cout << Solution().countComponents(n, edges) << '\n';
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