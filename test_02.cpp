#include <iostream>
#include "Allocator.h"

int main()
{
    Allocator<int, 15> alloc;
    int *p1 = nullptr;
    std::cout << "Created pointer p: " << p1 << "\n";

    p1 = alloc.allocate();
    std::cout << "Allocated to pointer p: " << p1 << "\n";

    alloc.construct(p1, 10);
    std::cout << "Constructed in pointer p1 (" << p1 << ") with value " << *p1 << "\n";

    int *p2 = alloc.allocate();
    alloc.construct(p2, 20);

    std::cout << "Created p2, allocated (" << p2 << ") and constructed with value " << *p2 << "\n";

    alloc.destroy(p1);
    alloc.deallocate(p1);
    std::cout << "Destroyed and deallocated pointer p1" << std::endl;

    alloc.destroy(p2);
    alloc.deallocate(p2);
    std::cout << "Destroyed and deallocated pointer p2" << std::endl;

}