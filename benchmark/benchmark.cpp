#include "vector.h"
#include <vector>
#include <chrono>

using ll = long long;

const size_t N = 5e6;
const size_t M = 5e4;

u_int16_t rng_state = 123456789;
inline u_int16_t fast_rand() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

inline void iota(auto st, auto end, int st_val) {
    while(st != end) {
        *st = st_val;
        st_val++;
        st++;
    }
}


template <typename Vec>
void benchmark(const std::string& s) {
    std::cout<<"Benchmarking "<<s<<"\n";
    std::cout<<"------------------------------------------------"<<"\n";

    //push_back
    {
        Vec v;
        auto start = std::chrono::high_resolution_clock::now();
        for(size_t i = 0; i<N; i++) {
            v.push_back(i);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::duration<double, std::milli>> (end-start);
        std::cout<<"push_back                :"<<duration.count()<<"ms\n";
    }
    
    //emplace_back
    {
        Vec v;
        auto start = std::chrono::high_resolution_clock::now();
        for(size_t i = 0; i<N; i++) {
            v.emplace_back(i);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::duration<double, std::milli>> (end-start);
        std::cout<<"emplace_back             :"<<duration.count()<<"ms\n";
    }
    
    //reserve+push_back
    {
        Vec v;
        auto start = std::chrono::high_resolution_clock::now();
        v.reserve(N);
        auto end = std::chrono::high_resolution_clock::now();
        for(size_t i = 0; i<N; i++) {
            v.push_back(i);
        }
        auto duration = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end-start);
        std::cout<<"reserve+push_back        :"<<duration.count()<<"ms\n";
    }
    
    //sequential read
    {
        Vec v(N);
        iota(v.begin(), v.end(), 0);
    
        auto start = std::chrono::high_resolution_clock::now();
        ll sum = 0;
        for(size_t i = 0; i<N; i++) {
            sum += v[i];
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end-start);
        std::cout<<"sequential_read          :"<<duration.count()<<"ms\n";
    }
    
    //random read
    {
        Vec v(N);
        iota(v.begin(), v.end(), 0);
    
        auto start = std::chrono::high_resolution_clock::now();
        ll sum = 0;
        for(size_t i = 0; i<N; i++) {
            int ind = fast_rand() % N;
            sum += v[ind];
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end-start);
        std::cout<<"random_read              :"<<duration.count()<<"ms\n";
    }
    
    //sequential write
    {
        Vec v(N);
        auto start = std::chrono::high_resolution_clock::now();
        for(size_t i = 0; i<N; i++) {
            v[i] = i;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end-start);
        std::cout<<"sequential write         :"<<duration.count()<<"ms\n";
    }
    
    //random write
    {
        Vec v(N);
        auto start = std::chrono::high_resolution_clock::now();
        for(size_t i = 0; i<N; i++) {
            int ind = fast_rand();
            v[ind] = i;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end-start);
        std::cout<<"random write             :"<<duration.count()<<"ms\n";
    }
    
    //range-for iteration
    {
        Vec v(N);
        iota(v.begin(), v.end(), 0);
        ll sum = 0;
        auto start = std::chrono::high_resolution_clock::now();
        for(auto it:v) {
            sum += it;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end-start);
        std::cout<<"range-for iteration      :"<<duration.count()<<"ms\n";
    }
    
    //front erase stress test
    {
        Vec v(M);
        iota(v.begin(), v.end(), 0);
        auto start = std::chrono::high_resolution_clock::now();
        for(size_t i = 0; i<M; i++) {
            v.erase(v.begin());
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end-start);
        std::cout<<"front erase stress test  :"<<duration.count()<<"ms\n";
    }
    
    //front insert stress test
    {
        Vec v;
        auto start = std::chrono::high_resolution_clock::now();
        for(size_t i = 0; i<M; i++) {
            v.insert(v.begin(), i);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end-start);
        std::cout<<"front insert stress test :"<<duration.count()<<"ms\n";
    }
    
    //pop_back
    {
        Vec v(N);
        iota(v.begin(), v.end(), 0);
        auto start = std::chrono::high_resolution_clock::now();
        for(size_t i = 0; i<N; i++) {
            v.pop_back();
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end-start);
        std::cout<<"pop_back                 :"<<duration.count()<<"ms\n";
    }
    
    //clear+refill
    {
        Vec v(N);
        iota(v.begin(), v.end(), 0);
        auto start = std::chrono::high_resolution_clock::now();
        v.clear();
        for(size_t i = 0; i<N; i++) {
            v.emplace_back(i);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end-start);
        std::cout<<"clear+refill             :"<<duration.count()<<"ms\n";
    }
    
    //copy contr
    {
        Vec v1(N);
        iota(v1.begin(), v1.end(), 0);
        auto start = std::chrono::high_resolution_clock::now();
        Vec v2(v1);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end-start);
        std::cout<<"Copy Constructor         :"<<duration.count()<<"ms\n";
    }
    
    //copy assignment
    {
        Vec v1(N);
        iota(v1.begin(), v1.end(), 0);
        Vec v2(100000);
        iota(v2.begin(), v2.end(), 0);
        auto start = std::chrono::high_resolution_clock::now();
        v2 = v1;
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end-start);
        std::cout<<"Copy Assignment          :"<<duration.count()<<"ms\n";
    }
    
    //move contr
    {
        Vec v1(N);
        iota(v1.begin(), v1.end(), 0);
        auto start = std::chrono::high_resolution_clock::now();
        Vec v2(move(v1));
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end-start);
        std::cout<<"Move Constructor         :"<<duration.count()<<"ms\n";
    }
    
    //move assignment
    {
        Vec v1(N);
        iota(v1.begin(), v1.end(), 0);
        Vec v2(100000);
        iota(v2.begin(), v2.end(), 0);
        auto start = std::chrono::high_resolution_clock::now();
        v2 = move(v1);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end-start);
        std::cout<<"Move Assignment          :"<<duration.count()<<"ms\n";
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