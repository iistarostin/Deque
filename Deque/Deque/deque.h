#pragma once
#include <iterator>
#include <memory>
#include <iostream>
const unsigned int MIN_CAP = 4; //must be a power of 2

template <class T, class Alloc = std::allocator<T>>
class Deque
{
    T* raw;
    T* first;
    T* last;
    size_t count;
    size_t cap;
    Alloc alloc;
public:
    class EmptyDequeException { ; };
    class DataCorruptionException { ; };
    template<class Val_Type, class Ptr_Type>
    class IteratorBase : public std::iterator<std::random_access_iterator_tag, Val_Type>
    {
    public:
        T* current;
        IteratorBase(T* Current) :
            current(Current) {}
        IteratorBase(const IteratorBase& r) :
            current(r.current) {}
        bool operator ==(const IteratorBase& r)
        {
            return current == r.current;
        }
        bool operator !=(const IteratorBase& r)
        {
            return current != r.current;
        }
        IteratorBase& operator++()
        {
            ++current;
            return *this;
        }
        IteratorBase operator++(int)
        {
            IteratorBase temp(*this);
            ++current;
            return temp;
        }
        IteratorBase& operator--()
        {
            --current;
            return *this;
        }
        IteratorBase operator--(int)
        {
            IteratorBase temp(*this);
            --current;
            return temp;
        }
        IteratorBase& operator+=(int r)
        {
            current += r;
            return *this;
        }
        IteratorBase& operator-=(int r)
        {
            current -= r;
            return *this;
        }
        IteratorBase operator+(int r) const
        {
            return IteratorBase(*this) += r;
        }
        IteratorBase operator-(int r) const
        {
            return IteratorBase(*this) -= r;
        }
        int operator-(const IteratorBase& r) const 
        {
            return current - r.current;
        }
        Val_Type& operator *()
        {
            return *(this->current);
        }
        Ptr_Type operator->()
        {
            return this->current;
        }
        Val_Type& operator[] (int index)
        {
            return *(this->current + index);
        }
        ~IteratorBase()
        {}
    };
    /*
    class iterator : public IteratorBase
    {
    public:
        iterator(T* Current) :
            IteratorBase(Current, 0) {}
        iterator(const IteratorBase& r) :
            IteratorBase(r) {}
        T& operator *()
        {
            return *(this->current);
        }
        T* operator ->()
        {
            return (this->current);
        }
        
        T& operator[] (int index)
        {
            return *(this->current + index);
        }
        ~iterator()
        {
            ;
        }
    };
    class const_iterator : public IteratorBase
    {
    public:
        const_iterator(T* Current) :
            IteratorBase(Current, 0) {}

        const_iterator(const IteratorBase& r) :
            IteratorBase(r) {}

        const T& operator *()
        {
            return *(this->current);
        }
        const T* operator ->()
        {
            return this->current;
        }
        const T& operator[] (int index)
        {
            return *(this->current + index);
        }
        ~const_iterator()
        {
            
        }
    };
    class reverse_iterator : public IteratorBase
    {
    public:
        reverse_iterator(T* Current) :
            IteratorBase(Current, 1) {}
        reverse_iterator(const IteratorBase& r) :
            IteratorBase(r) {}

        T& operator *()
        {
            return *(this->current);
        }
        T* operator ->()
        {
            return this->current;
        }
        T& operator[] (int index)
        {
            return *(this->current - index);
        }
        ~reverse_iterator()
        {

        }
    };
    class const_reverse_iterator : public IteratorBase
    {
    public:
        const_reverse_iterator(T* Current) :
            IteratorBase(Current, 1) {}

        const_reverse_iterator(const IteratorBase& r) :
            IteratorBase(r) {}
        const T& operator *()
        {
            return *(this->current);
        }
        const T* operator ->()
        {
            return this->current;
        }
        const T& operator[] (int index)
        {
            return *(this->current - index);
        }
        ~const_reverse_iterator()
        {
            
        }
    };
    */
    typedef IteratorBase<T, T*> iterator;
    typedef IteratorBase<const T, const T*> const_iterator;
    typedef std::reverse_iterator<IteratorBase<T, T*>> reverse_iterator;
    typedef std::reverse_iterator<IteratorBase<const T, const T*>> const_reverse_iterator;
    Deque()
        :alloc()
    {
        count = 0;
        cap = MIN_CAP;
        raw = allocate(cap);
        first = raw + cap / 2 - 1;
        last = first;
    }
    Deque(const Deque& original):
        alloc(original.alloc)
    {
        count = original.count;
        cap = original.cap;
        T* temp_raw = allocate(cap);
        first = temp_raw + (original.first - original.raw);
        last = temp_raw + (original.last - original.raw);
        for (unsigned int i = 0; i < count; ++i)
        {
            construct(first + i, *(original.first + i));
        }
        raw = temp_raw;
    }

    const T& back() const
    {
        return *(last - 1);
    }
    T& back()
    {
        return *(last - 1);
    }
    const T& front() const
    {
        return *first;
    }
    T& front()
    {
        return *first;
    }
    T operator[] (unsigned int index) const
    {
        return first[index];
    }
    T& operator[] (unsigned int index)
    {
        return first[index];
    }

    bool empty() const
    {
        return !count;
    }
    size_t size() const
    {
        return count;
    }

    T pop_back()
    {
        if (empty())
            throw EmptyDequeException();
        T res = *(--last);
        destroy(last);
        --count;
        shrink();
        return res;
    }
    T pop_front()
    {
        if (empty())
            throw EmptyDequeException();
        T res = *first;
        destroy(first++);
        --count;
        shrink();
        return res;;
    }
    void push_back(const T& val)
    {
        if (last - raw == (int)cap)
            stretch();
        construct(last++, val);
        ++count;
    }
    void push_front(const T& val)
    {
        if (!(first - raw))
            stretch();
        construct(--first, val);
        ++count;
    }

    iterator begin()
    {
        return iterator(first);
    }
    const_iterator begin() const
    {
        return const_iterator(first);
    }
    const_iterator cbegin() const
    {
        return const_iterator(first);
    }
    iterator end()
    {
        return iterator(last);
    }
    const_iterator end() const
    {
        return const_iterator(last);
    }
    const_iterator cend() const
    {
        return const_iterator(last);
    }

    reverse_iterator rbegin()
    {
        return reverse_iterator(last);
    }
    const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(last);
    }
    const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(last);
    }
    reverse_iterator rend()
    {
        return reverse_iterator(first);
    }
    const_reverse_iterator rend() const
    {
        return const_reverse_iterator(first);
    }
    const_reverse_iterator crend() const
    {
        return const_reverse_iterator(first);
    }

    ~Deque()
    {
        if (raw)
            deallocate(raw, cap);
    }

private:
    T* allocate(size_t number_of_elems)
    {
        return alloc.allocate(number_of_elems);
    }
    void construct(T* dest, const T& obj)
    {
        alloc.construct(dest, obj);
    }
    void destroy(T* dest)
    {
        alloc.destroy(dest);
    }
    void deallocate(T* &dest, size_t number_of_elems)
    {
        alloc.deallocate(dest, number_of_elems);
        dest = 0;
    }

    void resize(size_t new_cap)
    {
        T* new_raw = allocate(new_cap);
        T* new_first = new_raw + new_cap / 4 + 1;
        for (unsigned int i = 0; i < count; ++i)
        {
            construct(new_first + i, first[i]);
            destroy(first + i);
        }
        deallocate(raw, cap);
        raw = new_raw;
        cap = new_cap;
        first = new_first;
        last = first + count;
    }

    void stretch()
    {
        size_t new_cap = cap * 2;
        resize(new_cap);
    }
    void shrink()
    {
        if (cap < MIN_CAP)
        {
            throw DataCorruptionException();
        }
        if (count > cap / 4 || cap < 2 * MIN_CAP)
            return;
        size_t new_cap = cap / 2;
        resize(new_cap);
    }
};
