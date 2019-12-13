#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <exception>
#include <memory>
#include <iostream>

template <class T, size_t BLOCK_COUNT> 
class Allocator {
public:
    using value_type = T;
    using pointer    = value_type*;
    using reference  = value_type&;
	using const_reference = const value_type&;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    size_type getFreeSize   (void) const { return freeCount; }
    char *    getAllocBlocks(void) const { return allocatedBlocks; }
    void **   getFreeBlocks (void) const { return freeBlockPoint; }


    Allocator()
    {
        freeCount = BLOCK_COUNT;
        allocatedBlocks = static_cast<char*  >(operator new(BLOCK_COUNT * sizeof(T)     ));
		freeBlockPoint  = static_cast<void** >(operator new(BLOCK_COUNT * sizeof(void*) ));
        for (size_type i = 0; i < BLOCK_COUNT; ++i)
        {
            freeBlockPoint[i] = allocatedBlocks + i * sizeof(T);
        }
        std::cout << "Allocate - success!\n";
    }

    template<typename U>
    Allocator(const Allocator<U, BLOCK_COUNT> &other)
    {
        freeCount   = other.getFreeSize();
        allocatedBlocks = other.getAllocBlocks();
        freeBlockPoint  = other.getFreeBlocks();
        std::cout << "Allocate - SUCCESS!\n";
    }
    ~Allocator()
    {
        operator delete(allocatedBlocks);
        operator delete(freeBlockPoint);
        std::cout << "Delete allocated - SUCCESS!\n";
    }

    template<class U>
    struct rebind {
        using other = Allocator<U, BLOCK_COUNT>;
    };

    pointer allocate(size_type amount = 1)
    {
		if (freeCount == 0) throw std::bad_alloc();
		return (pointer) freeBlockPoint[--freeCount];
    }
    
    void deallocate(pointer ptr, size_type amount = 1)
    {
		size_type insertIndex;
		for (insertIndex = 0; insertIndex < freeCount; ++insertIndex)
		{
			if (ptr < freeBlockPoint[insertIndex]) break;
		}
		for (size_type i = freeCount; i != insertIndex; --i)
		{
			freeBlockPoint[i] = freeBlockPoint[i - 1];
		}
		freeBlockPoint[insertIndex] = ptr;
		++freeCount;
    }

	template<typename U, typename... Args>
	void construct(U *ptr, Args &&...args)
	{
		new (ptr) U (std::forward<Args>(args)...);
	}

	void destroy(pointer ptr)
	{
		ptr->~T();
	}

private:
    size_type freeCount;
	void **freeBlockPoint;
	char *allocatedBlocks;
};

template<typename T1, size_t SZ1, typename T2, size_t SZ2>
inline bool operator == (const Allocator<T1, SZ1>&, const Allocator<T2, SZ2>&)
{ return true; }

template<typename T, size_t SZ>
inline bool operator == (const Allocator<T, SZ>&, const Allocator<T, SZ>&)
{ return true; }

template<typename T1, size_t SZ1, typename T2, size_t SZ2>
inline bool operator != (const Allocator<T1, SZ1>&, const Allocator<T2, SZ2>&)
{ return false; }

template<typename T, size_t SZ>
inline bool operator != (const Allocator<T, SZ>&, const Allocator<T, SZ>&)
{ return false; }

#endif
