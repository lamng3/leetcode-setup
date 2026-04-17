#pragma once
#include <bits/stdc++.h>
using namespace std;

struct DSURollback {
    int n;
    vector<int> parent, sz, history;
    DSURollback(int _n) : n(_n) {
        sz.assign(n, 1);
        parent.resize(n);
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    int find(int v) {
        if (v == parent[v]) return v;
        // no path compression
        return find(parent[v]);
    }
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;

        if (sz[a] < sz[b]) swap(a, b);
        
        parent[b] = a;
        sz[a] += sz[b];

        // remember who became child, extend to History struct as needed
        history.push_back(b); 
        return true;
    }
    void rollback() {
        if (history.empty()) return;

        int b = history.back();
        history.pop_back();

        int a = parent[b];

        sz[a] -= sz[b];
        parent[b] = b;
    }
};