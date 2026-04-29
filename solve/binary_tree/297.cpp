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

// 297. Serialize and Deserialize Binary Tree [Hard]
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

class Codec {
public:
    const int NULLNODE = 1005;

    string bfs(TreeNode* node) {
        string res = "";
        if (!node) return res;

        queue<TreeNode*> q;
        q.push(node);

        while (!q.empty()) {
            int sz = (int)q.size();
            string tmp = "";

            REP(i, sz) {
                auto u = q.front();
                q.pop();

                string curr = "#" + to_string(u->val);
                tmp += curr;

                if (u->val != NULLNODE) {
                    if (u->left) q.push(u->left);
                    else q.push(new TreeNode(NULLNODE));

                    if (u->right) q.push(u->right);
                    else q.push(new TreeNode(NULLNODE));
                }
            }

            res += tmp;
        }

        return res;
    }

    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        string s = bfs(root);
        return s;
    }

    vi to_vector(const string& data) {
        vi res;
        string run = "";
        for (auto& c : data) {
            if (c == '#') {
                if (!run.empty()) {
                    int x = stoi(run);
                    res.pb(x);
                    run = "";
                }
            }
            else run += c;
        }
        if (!run.empty()) {
            int x = stoi(run);
            res.pb(x);
        }
        return res;
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
        vi A = to_vector(data);
        int n = (int)A.size();
        
        if (n == 0 || A[0] == NULLNODE) return nullptr;

        TreeNode* root = new TreeNode(A[0]);

        queue<TreeNode*> q;
        q.push(root);

        int i = 1;
        while (!q.empty() && i < n) {
            auto u = q.front();
            q.pop();

            if (i < n && A[i] != NULLNODE) {
                u->left = new TreeNode(A[i]);
                q.push(u->left);
            }
            i++;

            if (i < n && A[i] != NULLNODE) {
                u->right = new TreeNode(A[i]);
                q.push(u->right);
            }
            i++;
        }

        return root;
    }
};

// Your Codec object will be instantiated and called as such:
// Codec ser, deser;
// TreeNode* ans = deser.deserialize(ser.serialize(root));

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