#include <iostream>
#include <cstdlib>
#include <cassert>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <cstring>
#include <initializer_list>

/*
The following vector has a design limitation in several areas causing wastage of memory
until the vector is destroyed, like pop_back, clear and erase.
The elemets to be erased is not destroyed because deleting it may cause duplicate
delete afterwards during reallocation of memory when vector becomes is full or
when the destructor is called.
*/

template <typename T>
class Vector {
    private:
        size_t currsize, cap;
        T *mainarr;
        void grow();
        T* mv();
        T* cpy();
    public:
        //Default Constructor
        Vector() : currsize(0), cap(1), mainarr(new T[cap]) {}

        //Constructor with given size only
        Vector(size_t givensize) 
        : currsize(givensize), cap(std::max<size_t>(1, 2*givensize)), mainarr(new T[cap]()) {}

        //Constructor with given size and value
        Vector(size_t givensize, const T& value)
        : currsize(givensize), cap(std::max<size_t>(1, 2*givensize)) {
            mainarr = new T[cap];
            for(size_t i = 0; i<givensize; i++) {
                mainarr[i] = value;
            }
        }
        
        //Constructor using Initializer_List
        Vector(std::initializer_list<T> l) {
            currsize = l.size();
            cap = std::max<size_t>((size_t)1, 2*currsize);
            mainarr = new T[cap];
            size_t i = 0;
            for(const auto x:l) {
                mainarr[i++] = x;
            }
        }

        //Copy Constructor (deep copy)
        Vector(const Vector& rhs) 
        : currsize(rhs.currsize), cap(rhs.cap), mainarr(new T[cap]) {
            for(size_t i = 0; i<currsize; i++) {
                mainarr[i] = rhs.mainarr[i];
            }
        }

        //Copy Assignment Operator
        Vector& operator=(const Vector& rhs) {
            if(this == &rhs) return *this;
            currsize = rhs.currsize;
            cap = rhs.cap;
            T* newarr = new T[cap];
            for(size_t i = 0; i<currsize; i++) {
                newarr[i] = rhs.mainarr[i];
            }
            delete[] mainarr;
            mainarr = newarr;
            return *this;
        }

        //Destructor
        ~Vector() {
            delete[] mainarr;
            mainarr = nullptr;
        }

        //Operator[]
        T& operator[](int index) {
            assert(index>=0 && index<currsize);
            return mainarr[index];
        }

        //Operator[] for const
        const T& operator[](int index) const {
            assert(index>=0 && index<currsize);
            return mainarr[index];
        }
        
        //Function at()
        T& at(int index) {
            if(index<0 || index>=currsize) {
                throw std::out_of_range("Index out of bounds");
            }
            return mainarr[index];
        }

        //Function to check current size
        size_t size() const {
            return currsize;
        }

        //Function to check current capacity
        size_t capacity() const {
            return cap;
        }

        //Function to check if empty
        bool empty() const {
            return currsize==0;
        }

        //Function to check if full
        bool full() const {
            return currsize==cap;
        }

        //Function push_back with lvalue
        void push_back(const T& val) {
            if(full()) grow();
            mainarr[currsize++] = val;
        }

        //Function push_back with rvalue
        void push_back(T&& val) {
            if(full()) grow();
            mainarr[currsize++] = std::move(val);
        }

        //Function pop_back
        void pop_back() {
            assert(currsize>0);
            currsize--;
        }

        
        using iterator = T*;
        using const_iterator = const T*;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = const std::reverse_iterator<const_iterator>;
        

        //Iterator begin
        iterator begin() {
            return mainarr;
        }

        const_iterator begin() const {
            return mainarr;
        }

        //Iterator end
        iterator end() {
            return (mainarr+currsize);
        }

        const_iterator end() const {
            return (mainarr+currsize);
        }

        //Iterator cbegin
        const_iterator cbegin() const{
            return mainarr;
        }

        //Iterator cend
        const_iterator cend() const {
            return (mainarr+currsize);
        }
        
        //Iterator rbegin
        reverse_iterator rbegin() {
            return reverse_iterator(end());
        }

        //Iterator rend
        reverse_iterator rend() {
            return reverse_iterator(begin());
        }

        //Iterator crbegin
        const_reverse_iterator crbegin() {
            return const_reverse_iterator(cend());
        }

        //Iterator crend
        const_reverse_iterator crend () {
            return const_reverse_iterator(cbegin());
        }
        
        //Function reserve
        void reserve(size_t n) {
            if(cap>=n) return;
            cap = n;
            T* newarr = mv();
            delete[] mainarr;
            mainarr = newarr;
        }

        //Function resize
        void resize(size_t size) {
            if(size<=currsize) currsize = size;
            else {
                for(size_t i = currsize; i<size; i++) {
                    this->push_back(T());
                }
            }
        }

        //Function resize with val
        void resize(size_t size, const T& val) {
            if(size<=currsize) currsize = size;
            else {
                for(size_t i = currsize; i<size; i++) {
                    this->push_back(val);
                }
            }
        }

        //Function clear
        //clear only resets currsize; objects remain alive until vector destruction
        void clear() {
            currsize = 0;
        }

        //Function erase
        void erase(iterator ind) {
            assert(currsize>0);
            assert(ind>=(this->begin()) && ind<(this->end()));
            size_t i = ind - this->begin();
            for(; i<currsize-1; i++) {
                mainarr[i] = mainarr[i+1];
            }
            currsize--;
        }

        //Function erase range
        void erase(iterator begin, iterator end) {
            assert(begin <= end);
            assert(begin >= this->begin());
            assert(end <= this->end());
            size_t size = currsize - (end - begin);
            cap = std::max<size_t>(1, 2*size);
            T* newarr = new T[cap];
            iterator it = this->begin();
            size_t i = 0;
            while(it != begin) {
                newarr[i++] = *(it++);
            }
            it = end;
            while(it != this->end()) {
                newarr[i++] = *(it++);
            }
            delete[] mainarr;
            mainarr = newarr;
            currsize = size;
        }

        //Function insert
        void insert(iterator ind, const T& val) {
            assert(ind>=(this->begin()) && ind<=(this->end()));
            size_t pos = ind - this->begin();
            if(currsize+1 > cap) {
                cap *= 2;
                T* newarr = new T[cap];
                for(size_t i = 0; i<pos; i++) {
                    newarr[i] = mainarr[i];
                }
                newarr[pos] = val;
                for(size_t i = currsize; i>pos; i--) {
                    newarr[i] = mainarr[i-1];
                }
                delete[] mainarr;
                mainarr = newarr;
                currsize++;
                return;
            }
            for(size_t i = currsize; i>pos; i--) {
                mainarr[i] = mainarr[i-1];
            }
            mainarr[pos] = val;
            currsize++;
        }
};

//Function mv
template <typename T>
T* Vector<T>::mv() {
    T* newarr = new T[cap];
    if constexpr (std::is_trivially_copyable_v<T>) {
        std::memcpy(newarr, mainarr, currsize*sizeof(T));
    }
    else {
        for(size_t i = 0; i<currsize; i++) {
            newarr[i] = std::move(mainarr[i]);
        }
    }
    return newarr;
}

//Function mv
template <typename T>
T* Vector<T>::cpy() {
    T* newarr = new T[cap];
    if constexpr (std::is_trivially_copyable_v<T>) {
        std::memcpy(newarr, mainarr, currsize*sizeof(T));
    }
    else {
        for(size_t i = 0; i<currsize; i++) {
            newarr[i] = mainarr[i];
        }
    }
    return newarr;
}

//Function grow
template <typename T>
void Vector<T>::grow() {
    cap *= 2;
    T* newarr = mv();
    delete[] mainarr;
    mainarr = newarr;
}
