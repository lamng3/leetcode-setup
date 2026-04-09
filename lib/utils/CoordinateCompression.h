#pragma once
#include <bits/stdc++.h>
using namespace std;

template<class T> struct CoordinateCompressor {
    vector<T> A; 
    
    CoordinateCompressor(vector<T>& arr) {
        A = arr;
        sort(A.begin(), A.end());
        A.erase(unique(A.begin(), A.end()), A.end());
        
        for (auto& x : arr) {
            x = lower_bound(A.begin(), A.end(), x) - A.begin();
        }
    }
    
    T value_at(int rank) const { 
        return A[rank]; 
    }
    
    int rank_of(T x) const { 
        return lower_bound(A.begin(), A.end(), x) - A.begin(); 
    }
    
    int size() const { 
        return A.size(); 
    }
};