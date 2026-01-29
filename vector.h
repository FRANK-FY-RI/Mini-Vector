#ifndef VECTOR_H
#define VECTOR_H

#include <cstdlib>
#include <cassert>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <cstring>
#include <type_traits>
#include <memory>
#include <scoped_allocator>
#include <initializer_list>



template <typename T, typename Alloc = std::allocator<T>>
class Vector {
    private:
        size_t currsize, cap;
        Alloc alloc;
        T *mainarr;
        void grow();

        using traits = std::allocator_traits<Alloc>;

        T* allocate(size_t n) {
            return n?traits::allocate(alloc, n) : nullptr;
        }

        void deallocate(T* ptr, size_t n) {
            if(ptr != nullptr) traits::deallocate(alloc, ptr, n);
        }

        template <typename... Args>
        void construct(T* ptr, Args&&... args) {
            traits::construct(alloc, ptr, std::forward<Args>(args)...);
        }

        void destroy(T* ptr) {
            traits::destroy(alloc, ptr);
        }

        void destroy_all(T* ptr, size_t n) {
            for(size_t i = 0; i<n; i++) {
                traits::destroy(alloc, ptr+i);
            }
        }

    public:
        //Default Constructor
        Vector() : currsize(0), cap(1), mainarr(allocate(cap)) {}

        //Constructor with given size only
        Vector(size_t givensize) 
        : currsize(givensize), cap(std::max<size_t>(1, 2*givensize)), mainarr(allocate(cap)) {
            for(size_t i = 0; i<givensize; i++) {
                construct(mainarr+i);
            }
        }

        //Constructor with given size and value
        Vector(size_t givensize, const T& value)
        : currsize(givensize), cap(std::max<size_t>(1, 2*givensize)) , mainarr(allocate(cap)){
            for(size_t i = 0; i<givensize; i++) {
                construct(mainarr+i, value);
            }
        }
        
        //Constructor using Initializer_List
        Vector(std::initializer_list<T> l) {
            currsize = l.size();
            cap = std::max<size_t>((size_t)1, 2*currsize);
            mainarr = allocate(cap);
            size_t i = 0;
            for(const auto x:l) {
                construct(mainarr+i, x);
                i++;
            }
        }

        //Copy Constructor (deep copy)
        Vector(const Vector& rhs) 
        : currsize(rhs.currsize), cap(rhs.cap) , mainarr(allocate(cap)) {
            if constexpr (std::is_trivially_copyable_v<T>) {
                std::memcpy(mainarr, rhs.mainarr, rhs.currsize*sizeof(T));
            }
            else {
                for(size_t i = 0; i<rhs.currsize; i++) {
                    construct(mainarr+i, rhs.mainarr[i]);
                }
            }
        }

        //Copy Assignment Operator
        Vector& operator=(const Vector& rhs) {
            if(this == &rhs) return *this;
            T* newarr = allocate(rhs.cap);
            if constexpr (std::is_trivially_copyable_v<T>) {
                std::memcpy(newarr, rhs.mainarr, rhs.currsize*sizeof(T));
            }
            else {
                for(size_t i = 0; i<rhs.currsize; i++) {
                    construct(newarr+i, rhs.mainarr[i]);
                }
            }
            if constexpr (!std::is_trivially_destructible_v<T>) {
                destroy_all(mainarr, currsize);
            }
            deallocate(mainarr, cap);
            mainarr = newarr;
            currsize = rhs.currsize;
            cap = rhs.cap;
            return *this;
        }

        //Move Constructor
        Vector(Vector&& rhs) noexcept
        : currsize(rhs.currsize), cap(rhs.cap), mainarr(rhs.mainarr)
        {
            rhs.cap = rhs.currsize = 0;
            rhs.mainarr = nullptr;
        }

        //Move Assignment Operator
        Vector& operator=(Vector&& rhs) noexcept {
            if(this == &rhs) return *this;
            if constexpr (!std::is_trivially_destructible_v<T>) {
                destroy_all(mainarr, currsize);
            }
            deallocate(mainarr, cap);
            currsize = rhs.currsize;
            cap = rhs.cap;
            mainarr = rhs.mainarr;
            rhs.currsize = rhs.cap = 0;
            rhs.mainarr = nullptr;
            return *this;
        }

        //Destructor
        ~Vector() {
            destroy_all(mainarr, currsize);
            deallocate(mainarr, cap);
        }

        //Operator[]
        T& operator[](int index) {
            assert(index>=0 && index<(int)currsize);
            return mainarr[index];
        }

        //Operator[] for const
        const T& operator[](int index) const {
            assert(index>=0 && index<(int)currsize);
            return mainarr[index];
        }
        
        //Function at()
        T& at(int index) {
            if(index<0 || index>=(int)currsize) {
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
            construct(mainarr+currsize, val);
            currsize++;
        }

        //Function push_back with rvalue
        void push_back(T&& val) {
            if(full()) grow();
            construct(mainarr+currsize, std::move(val));
            currsize++;
        }

        //Function pop_back
        void pop_back() {
            assert(currsize>0);
            destroy(mainarr+currsize-1);
            currsize--;
        }

        //Function emplace_back
        template <typename... Args>
        void emplace_back(Args&&... args) {
            if(full()) grow();
            construct(mainarr+currsize, std::forward<Args>(args)...);
            currsize++;
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
            T* newarr = allocate(n);
            if constexpr (std::is_trivially_copyable_v<T>) {
                memcpy(newarr, mainarr, currsize*sizeof(T));
            }
            else {
                for(size_t i = 0; i<currsize; i++) {
                    construct(newarr+i, std::move_if_noexcept(mainarr[i]));
                }
            }
            if constexpr (!std::is_trivially_destructible_v<T>) {
                destroy_all(mainarr, currsize);
            }
            deallocate(mainarr, cap);
            cap = n;
            mainarr = newarr;
        }

        //Function resize
        void resize(size_t size) {
            if(size<=currsize) {
                for(size_t i = size; i<currsize; i++) {
                    destroy(mainarr+i);
                }
                currsize = size;
            }
            else {
                for(size_t i = currsize; i<size; i++) {
                    this->emplace_back(T());
                }
            }
        }
        
        //Function resize with val
        void resize(size_t size, const T& val) {
            if(size<=currsize) {
                for(size_t i = size; i<currsize; i++) {
                    destroy(mainarr+i);
                }
                currsize = size;
            }
            else {
                for(size_t i = currsize; i<size; i++) {
                    this->emplace_back(val);
                }
            }
        }

        //Function clear
        void clear() {
            if constexpr (!std::is_trivially_destructible_v<T>) {
                destroy_all(mainarr, currsize);
            }
            currsize = 0;
        }

        //Function erase
        void erase(iterator ind) {
            assert(currsize>0);
            assert(ind>=(this->begin()) && ind<(this->end()));
            size_t i = ind - this->begin();
            for(; i<currsize-1; i++) {
                mainarr[i] = std::move_if_noexcept(mainarr[i+1]);
            }
            if constexpr (!std::is_trivially_destructible_v<T>) {
                destroy(mainarr+currsize-1);
            }
            currsize--;
        }
        
        //Function erase range
        void erase(iterator begin, iterator end) {
            assert(begin < end);
            assert(begin >= this->begin());
            assert(end <= this->end());
            size_t r = end - this->begin();
            size_t l = begin - this->begin();
            while(r<currsize) {
                mainarr[l] = std::move_if_noexcept(mainarr[r]);
                r++;
                l++;
            }
            if constexpr (!std::is_trivially_destructible_v<T>) {
                for(size_t i = l; i<currsize; i++) {
                    destroy(mainarr+i);
                }
            }
            currsize = currsize - (end-begin);
        }

        //Function insert with lvalue
        void insert(iterator ind, const T& val) {
            assert(ind>=(this->begin()) && ind<=(this->end()));
            size_t pos = ind - this->begin();
            if(pos == currsize) {
                this->emplace_back(val);
                return;
            }
            if(currsize+1 > cap) {
                T* newarr = allocate(2*cap);
                for(size_t i = 0; i<pos; i++) {
                    construct(newarr+i, std::move_if_noexcept(mainarr[i]));
                }
                for(size_t i = currsize; i>pos; i--) {
                    construct(newarr+i, std::move_if_noexcept(mainarr[i-1]));
                    
                }
                construct(newarr+pos, val);
                if constexpr (!std::is_trivially_destructible_v<T>) {
                    destroy_all(mainarr, currsize);
                }
                deallocate(mainarr, cap);
                cap *= 2;
                mainarr = newarr;
                currsize++;
                return;
            }
            construct(mainarr+currsize, std::move_if_noexcept(mainarr[currsize-1]));
            for(size_t i = currsize-1; i>pos; i--) {
                mainarr[i] = std::move_if_noexcept(mainarr[i-1]);
            }
            mainarr[pos] = val;
            currsize++;
        }

        //Function insert with rvalue
        void insert(iterator ind, T&& val) {
            assert(ind>=(this->begin()) && ind<=(this->end()));
            size_t pos = ind - this->begin();
            if(pos == currsize) {
                this->emplace_back(std::move_if_noexcept(val));
                return;
            }
            if(currsize+1 > cap) {
                T* newarr = allocate(2*cap);
                for(size_t i = 0; i<pos; i++) {
                    construct(newarr+i, std::move_if_noexcept(mainarr[i]));
                }
                for(size_t i = currsize; i>pos; i--) {
                    construct(newarr+i, std::move_if_noexcept(mainarr[i-1]));
                    
                }
                construct(newarr+pos, std::move_if_noexcept(val));
                if constexpr (!std::is_trivially_destructible_v<T>) {
                    destroy_all(mainarr, currsize);
                }
                deallocate(mainarr, cap);
                cap *= 2;
                mainarr = newarr;
                currsize++;
                return;
            }
            construct(mainarr+currsize, std::move_if_noexcept(mainarr[currsize-1]));
            for(size_t i = currsize-1; i>pos; i--) {
                mainarr[i] = std::move_if_noexcept(mainarr[i-1]);
            }
            mainarr[pos] = std::move_if_noexcept(val);
            currsize++;
        }

};

//Function grow
template <typename T, typename Alloc>
void Vector<T, Alloc>::grow() {
    T* newarr = allocate(2*cap);
    if constexpr (std::is_trivially_copyable_v<T>) {
        std::memcpy(newarr, mainarr, sizeof(T)*currsize);
    }
    else {
        for(size_t i = 0; i<currsize; i++) {
            construct(newarr+i, std::move_if_noexcept(mainarr[i]));
        }
    }
    if constexpr (!std::is_trivially_destructible_v<T>) {
        destroy_all(mainarr, currsize);
    }
    deallocate(mainarr, cap);
    cap *= 2;
    mainarr = newarr;
}

#endif