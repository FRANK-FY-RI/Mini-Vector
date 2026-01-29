#include "vector.h"
#include <vector>
#include <chrono>
#include <iostream>

using ll = long long;

const size_t N = 5e7;
const size_t M = 5e4;

static uint32_t rng_state = 123456789;
inline uint32_t fast_rand() {
    uint32_t x = rng_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    rng_state = x;
    return x;
}

inline void iota(auto st, auto end, int st_val) {
    while(st != end) {
        *st = st_val;
        st_val++;
        st++;
    }
}

class timer {
    std::string s;
    decltype(std::chrono::high_resolution_clock::now()) start;
    public:
    timer (std::string&& str) {
        s = std::move(str);
        start = std::chrono::high_resolution_clock::now();
    }
    ~timer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
        std::string ans(35, ' ');
        for(decltype(s.size()) i = 0; i<s.size(); i++) {
            ans[i] = s[i];
        }
        std::cout<<ans<<duration.count()<<" ms\n";
    }
};

template <typename Vec>
void benchmark(const std::string& s) {
    std::cout<<"Benchmarking "<<s<<"\n";
    std::cout<<"------------------------------------------------"<<"\n";

    //push_back
    {
        Vec v;
        timer t("push back");
        for(size_t i = 0; i<N; i++) {
            v.push_back(i);
        }
    }
    
    //emplace_back
    {
        Vec v;
        timer t("emplace_back");
        for(size_t i = 0; i<N; i++) {
            v.emplace_back(i);
        }
    }
    
    //reserve+push_back
    {
        Vec v;
        timer t("reserve+push back");
        v.reserve(N);
        for(size_t i = 0; i<N; i++) {
            v.push_back(i);
        }
    }
    
    //sequential read
    {
        Vec v(N);
        iota(v.begin(), v.end(), 0);
    
        timer t("sequential read");
        volatile ll sum = 0;
        for(size_t i = 0; i<N; i++) {
            sum += v[i];
        }
    }
    
    //random read
    {
        Vec v(N);
        iota(v.begin(), v.end(), 0);
    
        timer t("random read");
        ll sum = 0;
        for(size_t i = 0; i<N; i++) {
            int ind = fast_rand() % N;
            sum += v[ind];
        }
    }
    
    //sequential write
    {
        Vec v(N);
        timer t("sequential write");
        for(size_t i = 0; i<N; i++) {
            v[i] = i;
        }
    }
    
    //random write
    {
        Vec v(N);
        timer t("random write");
        for(size_t i = 0; i<N; i++) {
            int ind = fast_rand() % N;
            v[ind] = i;
        }
    }
    
    //range-for iteration
    {
        Vec v(N);
        iota(v.begin(), v.end(), 0);
        volatile ll sum = 0;
        timer t("range-for iteration");
        for(const auto it:v) {
            sum += it;
        }
    }
    
    //front erase stress test
    {
        Vec v(M);
        iota(v.begin(), v.end(), 0);
        timer t("front erase stress test");
        for(size_t i = 0; i<M; i++) {
            v.erase(v.begin());
        }
    }
    
    //front insert stress test
    {
        Vec v;
        timer t("front insert stress test");
        for(size_t i = 0; i<M; i++) {
            v.insert(v.begin(), i);
        }
    }
    
    //pop_back
    {
        Vec v(N);
        iota(v.begin(), v.end(), 0);
        timer t("pop_back");
        for(size_t i = 0; i<N; i++) {
            v.pop_back();
        }
    }
    
    //clear+refill
    {
        Vec v(N);
        iota(v.begin(), v.end(), 0);
        timer t("clear+refill");
        v.clear();
        for(size_t i = 0; i<N; i++) {
            v.emplace_back(i);
        }
    }
    
    //copy contr
    {
        Vec v1(N);
        iota(v1.begin(), v1.end(), 0);
        timer t("Copy Constructor");
        Vec v2(v1);
    }
    
    //copy assignment
    {
        Vec v1(N);
        iota(v1.begin(), v1.end(), 0);
        Vec v2(100000);
        iota(v2.begin(), v2.end(), 0);
        timer t("Copy Assignment");
        v2 = v1;
    }
    
    //move contr
    {
        Vec v1(N);
        iota(v1.begin(), v1.end(), 0);
        timer t("Move Constructor");
        Vec v2(std::move(v1));
    }
    
    //move assignment
    {
        Vec v1(N);
        iota(v1.begin(), v1.end(), 0);
        Vec v2(100000);
        iota(v2.begin(), v2.end(), 0);
        auto start = std::chrono::high_resolution_clock::now();
        v2 = std::move(v1);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start);
        std::string s = "Move Assignment";
        std::string ans(35, ' ');
        for(decltype(s.size()) i = 0; i<s.size(); i++) {
            ans[i] = s[i];
        }
        std::cout<<ans<<duration.count()<<" ns\n";
    }
    std::cout<<"\n";
}

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(nullptr);
    benchmark<Vector<int>>("MyVector<int>");
    benchmark<std::vector<int>>("std::vector<int>");
    return 0;
}