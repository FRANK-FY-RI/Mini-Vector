#include "vector.h"
#include <vector>
#include <chrono>
#include <iostream>
#include <unordered_map>
#include <iomanip>
#include <numeric>

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

static int t = 0;

std::unordered_map<std::string, std::vector<double>> benchv;

class timer {
    std::string s;
    decltype(std::chrono::steady_clock::now()) start;
    public:
    timer (std::string&& str) {
        s = std::move(str);
        start = std::chrono::steady_clock::now();
    }
    ~timer() {
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start);
        if(!benchv.contains(s)) benchv[s] = std::vector<double>(2);
        benchv[s][t&1] += static_cast<double>(duration.count());
    }
};


template <typename Vec>
void benchmark() {
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
        std::iota(v.begin(), v.end(), 0);
    
        timer t("sequential read");
        volatile ll sum = 0;
        for(size_t i = 0; i<N; i++) {
            sum += v[i];
        }
    }
    
    //random read
    {
        Vec v(N);
        std::iota(v.begin(), v.end(), 0);
    
        timer t("random read");
        ll sum = 0;
        for(size_t i = 0; i<N; i++) {
            uint32_t r = fast_rand();
            uint64_t ind = (uint64_t(r)*N) >> 32;
            sum += v[ind];
        }
    }
    
    //sequential write
    {
        Vec v(N);
        timer t("sequential write");
        volatile ll sum = 0;
        for(size_t i = 0; i<N; i++) {
            v[i] = i;
            sum += i;
        }
    }
    
    //random write
    {
        Vec v(N);
        timer t("random write");
        for(size_t i = 0; i<N; i++) {
            uint32_t r = fast_rand();
            uint64_t ind = (uint64_t(r)*N) >> 32;
            v[ind] = i;
        }
    }
    
    //range-for iteration
    {
        Vec v(N);
        std::iota(v.begin(), v.end(), 0);
        volatile ll sum = 0;
        timer t("range-for iteration");
        for(const auto it:v) {
            sum += it;
        }
    }
    
    //front erase stress test
    {
        Vec v(M);
        std::iota(v.begin(), v.end(), 0);
        timer t("front erase stress test");
        for(size_t i = 0; i<M; i++) {
            v.erase(v.begin());
        }
    }

    //range erase stress test
    {
        Vec v(10000000);
        std::iota(v.begin(), v.end(), 0);
        timer t("range erase stress test");
        for(int i = 0; i<100; i++) {
            v.erase(v.begin(), v.begin()+100000);
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
        std::iota(v.begin(), v.end(), 0);
        timer t("pop_back");
        for(size_t i = 0; i<N; i++) {
            v.pop_back();
        }
    }
    
    //clear+refill
    {
        Vec v(N);
        std::iota(v.begin(), v.end(), 0);
        timer t("clear+refill");
        v.clear();
        for(size_t i = 0; i<N; i++) {
            v.emplace_back(i);
        }
    }

    //resize
    {
        Vec v(M);
        std::iota(v.begin(), v.end(), 0);
        timer t("resize");
        v.resize(N);
    }
    
    //copy contr
    {
        Vec v1(N);
        std::iota(v1.begin(), v1.end(), 0);
        timer t("Copy Constructor");
        Vec v2(v1);
    }
    
    //copy assignment
    {
        Vec v1(N);
        std::iota(v1.begin(), v1.end(), 0);
        Vec v2(100000);
        std::iota(v2.begin(), v2.end(), 0);
        timer t("Copy Assignment");
        v2 = v1;
    }
    
    //move contr
    {
        Vec v1(N);
        std::iota(v1.begin(), v1.end(), 0);
        timer t("Move Constructor");
        Vec v2(std::move(v1));
    }
    
    //move assignment
    {
        Vec v1(N);
        std::iota(v1.begin(), v1.end(), 0);
        Vec v2(100000);
        std::iota(v2.begin(), v2.end(), 0);
        timer t("Move Assignment");
        v2 = std::move(v1);
    }
}

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(nullptr);
    for(int i = 0; i<100; i++) {
        benchmark<Vector<int>>();
        t++;
        benchmark<std::vector<int>>();
    }
    for(const auto &it:benchv) {
        std::cout<<std::left<<std::setw(35)<<it.first;
        double time1 = (1.0*it.second[0])/1.0;
        double time2 = (1.0*it.second[1])/1.0;
        std::cout<<std::fixed<<std::setprecision(5)
                 <<std::setw(30)<<time1
                 <<std::setw(30)<<time2<<'\n';
    }
    return 0;
}