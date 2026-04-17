#pragma once
#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct SegmentTree {
    int N;
    vector<T> A;
    vector<T> st;

    SegmentTree(int n, const vector<T>& a) {
        N = n;
        A = a;
        st.resize(4 * N);
    }

    virtual T combine(const T& left, const T& right) = 0;
    virtual T identity() = 0;

    void build(int v, int tl, int tr) {
        if (tl == tr) {
            st[v] = A[tl];
            return;
        }
        int tm = tl + (tr - tl) / 2;
        build(v*2+1, tl, tm);
        build(v*2+2, tm+1, tr);
        st[v] = combine(st[v*2+1], st[v*2+2]);
    }

    T query(int v, int tl, int tr, int ql, int qr) {
        if (ql > qr) return identity();
        if (ql <= tl && tr <= qr) return st[v];
        int tm = tl + (tr - tl) / 2;
        T left = query(v*2+1, tl, tm, ql, min(tm, qr));
        T right = query(v*2+2, tm+1, tr, max(ql, tm+1), qr);
        return combine(left, right);
    }
};