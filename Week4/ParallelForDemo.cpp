// parallel-for-structure.cpp
// compile with: /EHsc => The exception-handling model that catches synchronous (C++) exceptions
#include <ppl.h>
#include <array>
#include <sstream>
#include <iostream>

using namespace concurrency;
using namespace std;

int wmain()
{
	// Because the parallel_for algorithm acts on each item in parallel, the order in which the values are printed to the console will vary.
	parallel_for(1, 100, [](int value) {
		wstringstream ss;
		ss << value << L' ';
		wcout << ss.str();
		});
}