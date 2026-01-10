#include <bits/stdc++.h>
#include "vector.h"
using namespace std;
using ll = long long;
using Clock = chrono::steady_clock;

static volatile ll sink;

constexpr int N  = 5'000'000;
constexpr int M  = 1'000'000;
constexpr int MID_OPS = 200'000;

// =====================================================
//                    TIMER
// =====================================================
struct Timer {
    Clock::time_point start;
    Timer() { start = Clock::now(); }
    double ms() const {
        return chrono::duration<double, milli>(Clock::now() - start).count();
    }
};


// =====================================================
//                  RANDOM
// =====================================================
static uint32_t rng_state = 123456789;
inline uint32_t fast_rand() {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}



// =====================================================
//                   BENCH
// =====================================================
template<typename Vec>
void benchmark(const string& name) {
    cout << "\n====================================\n";
    cout << "Benchmarking: " << name << "\n";
    cout << "====================================\n";

    // ---------------------------------------------------
    // 1. push_back + SPACE
    {
        Vec v;

        Timer t;
        for(int i = 0; i < N; i++) v.push_back(i);

        cout << "push_back                 : " << t.ms() << " ms\n";
        sink = v.size();
    }

    // ---------------------------------------------------
    // 2. reserve + push_back
    {
        Vec v;
        if constexpr (requires(Vec x) { x.reserve(N); })
            v.reserve(N);

        Timer t;
        for(int i = 0; i < N; i++) v.push_back(i);

        cout << "reserve + push_back       : " << t.ms() << " ms\n";
        sink = v.size();
    }

    // ---------------------------------------------------
    // 3. random write
    {
        Vec v(N);
        Timer t;
        for(int i = 0; i < N; i++) {
            int idx = fast_rand() % N;
            v[idx] = i;
        }
        cout << "random write              : " << t.ms() << " ms\n";
        sink = v[0];
    }

    // ---------------------------------------------------
    // 4. sequential read
    {
        Vec v(N);
        iota(v.begin(), v.end(), 1);

        Timer t;
        ll sum = 0;
        for(int i = 0; i < N; i++) sum += v[i];
        cout << "sequential read           : " << t.ms() << " ms\n";
        sink = sum;
    }

    // ---------------------------------------------------
    // 5. random read
    {
        Vec v(N);
        iota(v.begin(), v.end(), 1);

        Timer t;
        ll sum = 0;
        for(int i = 0; i < N; i++) {
            int idx = fast_rand() % N;
            sum += v[idx];
        }
        cout << "random read               : " << t.ms() << " ms\n";
        sink = sum;
    }

    // ---------------------------------------------------
    // 6. range-for iteration
    {
        Vec v(N);
        iota(v.begin(), v.end(), 1);

        Timer t;
        ll sum = 0;
        for(auto &x : v) sum += x;
        cout << "range iteration           : " << t.ms() << " ms\n";
        sink = sum;
    }

    // ---------------------------------------------------
    // 7. front erase stress
    {
        Vec v;
        for(int i = 0; i < M; i++) v.push_back(i);

        Timer t;
        for(int i = 0; i < M / 2; i++) {
            v.erase(v.begin());
        }
        cout << "erase front stress        : " << t.ms() << " ms\n";
        sink = v.size();
    }

    // ---------------------------------------------------
    // 8. middle insert stress
    {
        Vec v;
        for(int i = 0; i < M; i++) v.push_back(i);

        Timer t;
        for(int i = 0; i < MID_OPS; i++) {
            int pos = v.size() / 2;
            v.insert(v.begin() + pos, i);
        }
        cout << "middle insert stress      : " << t.ms() << " ms\n";
        sink = v.size();
    }

    // ---------------------------------------------------
    // 9. pop_back
    {
        Vec v(N);
        Timer t;
        while(!v.empty()) v.pop_back();
        cout << "pop_back                  : " << t.ms() << " ms\n";
        sink = v.size();
    }

    // ---------------------------------------------------
    // 10. clear + refill
    {
        Vec v(N);
        Timer t;
        v.clear();
        for(int i = 0; i < N; i++) v.push_back(i);
        cout << "clear + refill            : " << t.ms() << " ms\n";
        sink = v.size();
    }

    // ---------------------------------------------------
    // 11. copy constructor
    {
        Vec v(N);
        iota(v.begin(), v.end(), 1);

        Timer t;
        Vec copy = v;
        cout << "copy constructor          : " << t.ms() << " ms\n";
        sink = copy.size();
    }
        // ---------------------------------------------------
    // 12. move constructor
    {
        Vec v(N);
        iota(v.begin(), v.end(), 1);

        Timer t;
        Vec moved = std::move(v);
        cout << "move constructor          : " << t.ms() << " ms\n";
        sink = moved.size();
    }

    // ---------------------------------------------------
    // 13. move assignment
    {
        Vec src(N);
        iota(src.begin(), src.end(), 1);

        Vec dst;   // empty destination

        Timer t;
        dst = std::move(src);
        cout << "move assignment           : " << t.ms() << " ms\n";
        sink = dst.size();
    }

    cout << endl;
}

// =====================================================
//                     MAIN
// =====================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    using StdVec = vector<int>;

    benchmark<StdVec>("std::vector<int>");
    benchmark<Vector<int>>("MyVector<int>");

    return 0;
}
