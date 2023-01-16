/** @file demo2-reinterpret_cast.cpp
 *  @brief Purpose for using reinterpret_cast
 *  
 *  reinterpret_cast is a very special and dangerous type of casting operator. And is suggested to use it using proper data type i.e., (pointer data type should be same as original data type).
 *  It can typecast any pointer to any other data type.
 *  It is used when we want to work with bits.
 *  If we use this type of cast then it becomes a non-portable product. So, it is suggested not to use this concept unless required.
 *  It is only used to typecast any pointer to its original type.
 *  Boolean value will be converted into integer value i.e., 0 for false and 1 for true.
 *
 *  @author Hooman Salamat
 *  @bug No known bugs.
 */

 // CPP code to illustrate using structure
#include <iostream>
using namespace std;

// creating structure mystruct
struct mystruct {
	int x;
	int y;
	char c;
	bool b;
};

class A {
public:
	void fun_a()
	{
		cout << " In class A\n";
	}
};

class B {
public:
	void fun_b()
	{
		cout << " In class B\n";
	}
};


int main()
{
	mystruct s;

	// Assigning values
	s.x = 5;
	s.y = 10;
	s.c = 'a';
	s.b = true;

	// data type must be same during casting
	// as that of original

	// converting the pointer of 's' to,
	// pointer of int type in 'p'.
	int* p = reinterpret_cast<int*>(&s);

	cout << sizeof(s) << endl;

	// printing the value currently pointed by *p
	cout << *p << endl;

	// incrementing the pointer by 1
	p++;

	// printing the next integer value
	cout << *p << endl;

	p++;

	// we are casting back char * pointed
	// by p using char *ch.
	char* ch = reinterpret_cast<char*>(p);

	// printing the character value
	// pointed by (*ch)
	cout << *ch << endl;

	ch++;

	/* since, (*ch) now points to boolean value,
	so it is required to access the value using
	same type conversion.so, we have used
	data type of *n to be bool. */

	bool* n = reinterpret_cast<bool*>(ch);
	cout << *n << endl;

	// we can also use this line of code to
	// print the value pointed by (*ch).
	cout << *(reinterpret_cast<bool*>(ch));

	// creating object of class B
	B* x = new B();

	// converting the pointer to object
	// referenced of class B to class A
	A* new_a = reinterpret_cast<A*>(x);

	// accessing the function of class A
	new_a->fun_a();

	return 0;
}
