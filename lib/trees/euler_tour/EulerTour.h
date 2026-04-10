#pragma once
#include <bits/stdc++.h>
using namespace std;

struct EulerTour {
public:
    vector<vector<int>> g;
    vector<int> S, E;
    vector<int> sz;
    int timer;

    EulerTour(int n, const vector<vector<int>>& _g, const vector<int>& _values) {
        g = _g;
        S.resize(n);
        E.resize(n);
        sz.resize(n);
        timer = 0;
    }

    int dfs(int u, int p) {
        S[u] = timer++;
        for (int v : g[u]) {
            if (v != p) {
                dfs(v, u);
            }
        }
        E[u] = timer;
        sz[S[u]] = E[u] - S[u];
    }
};