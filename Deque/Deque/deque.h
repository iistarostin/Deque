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
    class IteratorBase : public std::iterator<std::random_access_iterator_tag, T>
    {
    public:
        T* current;
        bool isReverse;
        IteratorBase(T* Current, bool IsReverse) :
            current(Current), isReverse(IsReverse) {}
        IteratorBase(const IteratorBase& r) :
            current(r.current), isReverse(r.isReverse) {}
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
            if (isReverse)
            {
                --current;
            }
            else
            {
                ++current;
            }
            return *this;
        }
        IteratorBase operator++(int)
        {
            IteratorBase temp(*this);
            if (isReverse)
            {
                --current;
            }
            else
            {
                ++current;
            }
            return temp;
        }
        IteratorBase& operator--()
        {
            if (isReverse)
            {
                ++current;
            }
            else
            {
                --current;
            }
            return *this;
        }
        IteratorBase operator--(int)
        {
            IteratorBase temp(*this);
            if (isReverse)
            {
                ++current;
            }
            else
            {
                --current;
            }
            return temp;
        }
        IteratorBase& operator+=(int r)
        {
            if (isReverse)
            {
                current-=r;
            }
            else
            {
                current+=r;
            }
            return *this;
        }
        IteratorBase& operator-=(int r)
        {
            if (isReverse)
            {
                current+=r;
            }
            else
            {
                current-=r;
            }
            return *this;
        }
        IteratorBase operator+(int r)
        {
            return IteratorBase(*this) += r;
        }
        IteratorBase operator-(int r)
        {
            return IteratorBase(*this) -= r;
        }
        int operator-(const IteratorBase& r)
        {
            return current - r.current;
        }
        ~IteratorBase()
        {
            ;
        }
    };
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
    Deque()
        :alloc()
    {
        count = 0;
        cap = MIN_CAP;
        raw = allocate(cap);
        first = raw + cap / 2;
        last = raw + cap / 2 - 1;
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
        return *last;
    }
    T& back()
    {
        return *last;
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
        if (!count)
            throw EmptyDequeException();
        --count;
        T res = *(last);
        destroy(last--);
        shrink();
        return res;
    }
    T pop_front()
    {
        if (!count)
            throw EmptyDequeException();
        --count;
        T res = *(first);
        destroy(first++);
        shrink();
        return res;;
    }
    void push_back(const T& val)
    {
        if ((int)(last - raw) == (int)(cap - 1))
            stretch();
        construct(++last, val);
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
        return iterator(last+1);
    }
    const_iterator end() const
    {
        return const_iterator(last+1);
    }
    const_iterator cend() const
    {
        return const_iterator(last+1);
    }

    reverse_iterator rbegin()
    {
        return reverse_iterator(first);
    }
    const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(first);
    }
    const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(first);
    }
    reverse_iterator rend()
    {
        return reverse_iterator(last+1);
    }
    const_reverse_iterator rend() const
    {
        return const_reverse_iterator(last+1);
    }
    const_reverse_iterator crend() const
    {
        return const_reverse_iterator(last+1);
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

    void stretch()
    {
        size_t new_cap = cap * 2;
        T* new_raw = allocate(new_cap);
        T* new_first = new_raw + cap / 4;
        for (unsigned int i = 0; i < count; ++i)
        {
            construct(new_first + i, first[i]);
            destroy(first + i);
        }
        deallocate(raw, cap);
        raw = new_raw;
        cap = new_cap;
        first = new_first;
        last = first + count - 1;
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
        T* new_raw = allocate(new_cap);
        T* new_first = new_raw + new_cap / 4;
        for (unsigned int i = 0; i < count; ++i)
        {
            construct(new_first + i, first[i]);
            destroy(first + i);
        }
        deallocate(raw, cap);
        raw = new_raw;
        cap = new_cap;
        first = new_first;
        last = first + count - 1;
    }
};
