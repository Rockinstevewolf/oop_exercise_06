#include <map>
#include <list>
#include <iostream>
#include "Allocator.h"

template <int N> int factorial() {
	return N * factorial<N - 1>();
}
template <> int factorial<0>() {
	return 1;
}

using AllocList =
std::list<int, Allocator<int, 10>>; 

using AllocMap =
std::map<int, int, std::less<int>, Allocator<std::pair<const int, int>, 10>>;

template <int N> void listFill(AllocList& list) {
	list.push_front(factorial<N>());
	listFill<N - 1>(list);
}

template <> void listFill<0>(AllocList& list) {
	list.push_front(factorial<0>());
}


template <int N> void mapFill(AllocMap& map) {
	map.insert(std::pair<int, int>(N, factorial<N>()));
	mapFill<N - 1>(map);
}

template <> void mapFill<0>(AllocMap& map) {
	map.insert(std::pair<int, int>(0, factorial<0>()));
}

int main()
{
	AllocMap allocatedMap;
	AllocList allocatedList;

	mapFill <9>(allocatedMap);
	listFill<9>(allocatedList);

	std::cout << "Allocated map:" << std::endl;
	for (auto p : allocatedMap) std::cout << "\t" << p.first << " " << p.second << "\n";
	std::cout << "Allocated list:" << std::endl;
	for (auto p : allocatedList) std::cout << "\t" << p << "\n";
	std::cout << std::flush;

	return 0;
}