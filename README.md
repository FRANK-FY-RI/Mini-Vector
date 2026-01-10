# 🚀 Mini Vector – Custom C++ Dynamic Array Implementation

A lightweight implementation of a `std::vector`-like container written in modern C++.
This project focuses on **manual memory management, copy/move semantics, performance benchmarking**, and understanding how STL containers work internally.

---

## 📁 Project Structure

```
.
├── vector.h
└── benchmark/
    ├── benchmark.cpp
    ├── benchmark.png
    └── results.csv
```

### Files

* **vector.h**
  Header-only implementation of a custom dynamic array (`MyVector<T>`).

* **benchmark/benchmark.cpp**
  Benchmark program comparing `MyVector<T>` against `std::vector<T>`.

* **benchmark/benchmark.png**
  Visualization of benchmark results.

* **benchmark/results.csv**
  Raw benchmark data used for plotting.

---

## ✨ Features

* ✅ Dynamic resizing
* ✅ Deep copy (copy constructor & copy assignment)
* ✅ Move semantics (move constructor & move assignment)
* ✅ Manual memory management (`new[]` / `delete[]`)
* ✅ Optimized copying for trivially copyable types
* ✅ Bounds-safe operations (with assertions)
* ✅ Performance benchmarking vs `std::vector`

---

## 🧠 Implemented Operations

* `push_back`
* `pop_back`
* `reserve`
* `clear`
* `resize`
* Copy constructor
* Copy assignment operator
* Move constructor
* Move assignment operator
* Initializer list constructor
* Indexing operator `operator[]`
* Iterators
* Size & capacity management

---

## ⚙️ Build & Run Benchmarks

### Compile

From the project root:

```bash
g++ -std=c++23 -O3 -march=native benchmark.cpp -o bench
```

> Compiler tested: **GCC**

### Run

```bash
./bench
```

This prints timing results for multiple operations such as:

* push_back
* reserve + push_back
* random write
* sequential read
* random read
* erase stress
* middle insert stress
* copy constructor
* move constructor
* move assignment

---

## 📊 Benchmark Results

Benchmark results are stored in:

* 📄 `benchmark/results.csv`
* 🖼️ `benchmark/benchmark.png`

Example comparison:

* `std::vector<int>`
* `MyVector<int>`

These benchmarks help analyze:

* Allocation strategies
* Copy vs move performance
* Cache friendliness
* Insert / erase overhead

> ⚠️ Results may vary depending on compiler flags, hardware, and OS.

---

## 🎯 Motivation

This project was built to:

* Understand how STL containers manage memory internally
* Practice rule-of-five implementations
* Learn performance measurement and benchmarking
* Improve low-level C++ skills relevant to systems programming

---

## 📌 Future Improvements

* Exception safety guarantees
* Small buffer optimization
* Custom allocator support
* More benchmark scenarios

---

## 📜 License

This project is for learning and experimentation purposes.

---
