#ifndef STACK_H
#define STACK_H

#include <memory>
#include <exception>
#include <algorithm>
#include <iostream>

template <class T, class Alloc = std::allocator<T>>class Stack {
public:
    class forward_iterator;
    class StackNode;

    Stack()
    {
        container = nullptr;
        _size = 0;
    }

    ~Stack() { erase(); }

    size_t size(void) const
    {
        return _size;
    }

    bool empty(void) const
    {
        return size() == 0;
    }


    forward_iterator begin(void) const
    {
        return forward_iterator(container);
    }

    forward_iterator end(void) const
    {
        return forward_iterator();
    }

    Stack& push(T value)
    {
        insert(_size, value);
        std::cout << "Stack push - SUCCESS!\n";
        return *this;
    }

    Stack& pop(void)
    {
        if (!empty()){
            erase(_size - 1);
            std::cout << "Stack pop - SUCCESS!\n";
        }

        else throw std::logic_error("Stack is empty");
        return *this;
    }

    T top(void)
    {
        if (!empty())
            return container.get()->top();
        else throw std::logic_error("Stack is empty");
    }

    void insert(size_t index, T val)
    {
        if (index == 0)
        {
            container = fabricateNode(val, container);
        }
        else if (index > 0 && index <= _size)
        {
            --index;
            forward_iterator begin = Stack<T, Alloc>::begin();
            for (forward_iterator end = Stack<T, Alloc>::end(); index != 0; ++begin)
            {
                --index;
            }

            std::shared_ptr<StackNode> allocated = fabricateNode(val, begin.value.lock().get()->next);
            begin.value.lock().get()->insert(std::move(allocated));
        }
        else throw std::logic_error("Outside the bounds");
        
        ++_size;
    }

    void erase(size_t index)
    {
        if (container == nullptr) throw std::logic_error("Stack is empty");
        if (index == 0)
        {
            container = container.get()->next;
        }
        else if (index > 0 && index < _size)
        {
            --index;
            forward_iterator begin = Stack<T, Alloc>::begin();
            for (forward_iterator end = Stack<T, Alloc>::end(); index != 0; ++begin)
            {
                --index;
            }
			begin.value.lock().get()->erase();
        }
        else throw std::logic_error("Outside the bounds");

        --_size;
    }

    void erase()
    {
        container = nullptr;
        _size = 0;
    }

private:
    std::shared_ptr<StackNode> container;
    size_t _size;

    std::shared_ptr<StackNode> fabricateNode (T val, const std::shared_ptr<StackNode> &oneAfterNext)
    {
        StackNode *raw = allocator.allocate(1);
        allocator.construct(raw, val, oneAfterNext);

        std::shared_ptr<StackNode> result (
            raw,
            [this](StackNode *p) {
                p->next = nullptr;
                allocator.deallocate(p, 1);
            }
        );
        return result;
    }

    using AllocNode = typename Alloc::template rebind<StackNode>::other;
	AllocNode allocator;
};

template <class T, class Alloc>
class Stack<T, Alloc>::forward_iterator {
public:
//The next using's actually make our pointer compatible with pointer templates
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type = T ;
    using pointer    = T*;
    using reference  = T&;

    forward_iterator() { value = std::shared_ptr<StackNode> (nullptr); }
    forward_iterator(const std::shared_ptr<StackNode> &arg) { value = arg; }
    ~forward_iterator() { };

    forward_iterator& operator = (const std::shared_ptr<StackNode> &arg)
    {
        value = arg;
        return *this;
    }

    forward_iterator& operator ++ ()
    {
        if (value.expired()) return *this;
        value = value.lock().get()->next;
        return *this;
    }

    forward_iterator operator ++ (int)
    {
        if (value.expired()) return *this;
        forward_iterator prev(value.lock());
        value = value.lock().get()->next;
        return prev;
    }

    T& operator *()
    {
        if (value.expired()) throw std::logic_error("Invalid iterator");
        return value.lock().get()->data;
    }

    bool operator == (const forward_iterator& other) const
    {
        return value.lock() == other.value.lock();
    }

    bool operator != (const forward_iterator& other) const
    {
        return !(*this == other);
    }

    std::weak_ptr<StackNode> value;
};

template <class T, class Alloc>
class Stack<T, Alloc>::StackNode {
    friend Stack<T, Alloc>;
public:
    StackNode(T val, const std::shared_ptr<StackNode> &oneAfterNext) // If we add in the middle, oneAfterNext will be element after out new one
    {
        data = val;
        if (oneAfterNext.get() != nullptr)
            next = oneAfterNext;
    }

    T top(void)
    {
        if (next.use_count() != 0) {
            return next.get()->top();
        }
        else return data;
    }
    
    void insert(std::shared_ptr<StackNode> Allocated)
    {
        next = Allocated;
    }

    void erase()
    {
        next = next.get()->next;
    }
    
private:
    T data;
    std::shared_ptr<StackNode> next;
};

#endif
