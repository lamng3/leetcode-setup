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

// 2458. Height of Binary Tree After Subtree Removal Queries [Hard]
#ifdef LOCAL
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
#endif

class Solution {
public:
    map<TreeNode*, int> S, E;
    map<int, TreeNode*> nodes;
    map<TreeNode*, int> H;
    vi euler;

    void flatten(TreeNode* node, int h) {
        if (!node) return;
        H[node] = h;
        S[node] = euler.size();
        nodes[node->val] = node;
        euler.pb(node->val);
        if (node->left) flatten(node->left, h+1);
        if (node->right) flatten(node->right, h+1);
        E[node] = euler.size();
    }

    vi treeQueries(TreeNode* root, vi& queries) {
        int m = (int)queries.size();

        flatten(root, 0);

        int n = (int)euler.size();

        // prefix max and suffix max
        vi pref(n);
        for (int i = 0; i < n; i++) {
            int prev = i > 0 ? pref[i-1] : -1;
            TreeNode* u = nodes[euler[i]];
            pref[i] = max(prev, H[u]);
        }

        vi suff(n);
        for (int i = n-1; i >= 0; i--) {
            int prev = i < n-1 ? suff[i+1] : -1;
            TreeNode* u = nodes[euler[i]];
            suff[i] = max(prev, H[u]);
        }

        vi ans(m);
        REP(i, m) {
            // independent queries
            int x = queries[i];
            TreeNode* u = nodes[x];
            
            // remove subtree rooted at node x
            // height with prefix and suffix max
            int L = S[u] > 0 ? pref[S[u]-1] : -1;
            int R = E[u] < n ? suff[E[u]] : -1;
            ans[i] = max(L, R);
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