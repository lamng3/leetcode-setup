#pragma once
#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    int n;
    vector<T> ft;
    FenwickTree(int _n) : n(_n), ft(_n + 1, 0) {}
    FenwickTree(const vector<T>& a) {
        init(a);
    }
    void init(const vector<T>& a) {
        n = a.size();
        ft.assign(n+1, 0);
        for (int i = 1; i <= n; i++) add(i, a[i-1]);
    }
    void add(int i, T u) {
        for (; i <= n; i += i & -i) ft[i] += u;
    }
    T sum(int i) {
        T s = 0;
        for (; i > 0; i -= i & -i) s += ft[i];
        return s;
    }
    T sum(int L, int R) {
        if (L > R) return 0;
        return sum(R) - sum(L-1);
    }
};