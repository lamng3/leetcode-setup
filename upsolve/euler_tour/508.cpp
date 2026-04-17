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

// 508. Most Frequent Subtree Sum [Medium]
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

struct Fenwick {
    int n;
    vi a, ft;
    
    Fenwick(int _n) {
        n = _n;
        ft.assign(n+1, 0);
    }

    void init(const vi& arr) {
        a = arr;
        for (int i = 1; i <= n; i++) add(i, a[i-1]);
    }

    void add(int i, int u) {
        for (; i <= n; i += i & -i) ft[i] += u;
    }

    int sum(int i) {
        int r = 0;
        for (; i > 0; i -= i & -i) r += ft[i];
        return r;
    }

    int sum(int L, int R) {
        if (L > R) return 0;
        return sum(R) - sum(L-1);
    }
};

class Solution {
public:
    map<TreeNode*, int> S, E;
    vector<TreeNode*> nodes;
    vi euler;

    void flatten(TreeNode* node) {
        if (!node) return;
        S[node] = euler.size();
        nodes.pb(node);
        euler.pb(node->val);
        if (node->left) flatten(node->left);
        if (node->right) flatten(node->right);
        E[node] = euler.size();
    }

    vi findFrequentTreeSum(TreeNode* root) {
        if (!root) return {};
        flatten(root);

        int n = (int)euler.size();
        Fenwick fenwick(n);
        fenwick.init(euler);

        map<int,int> f;
        int best = 0;
        for (TreeNode* node : nodes) {
            int L = S[node]+1, R = E[node];
            int x = fenwick.sum(L, R);
            f[x]++;
            best = max(best, f[x]);
        }

        vi ans;
        for (auto [k, v] : f) {
            if (v == best) ans.pb(k);
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