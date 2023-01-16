/** @file demo1-reinterpret_cast.cpp
 *  @brief reinterpret_cast is a type of casting operator used in C++.
 * 
 *  It is used to convert a pointer of some data type into a pointer of another data type, even if the data types before
 *  and after conversion are different.
 *  It does not check if the pointer typeand data pointed by the pointer is same or not.
 *  Syntax :
 *  
 *  
 *  data_type * var_name =
 *  reinterpret_cast <data_type*>(pointer_variable);
 *  Return Type
 *  
 *  It doesn’t have any return type.It simply converts the pointer type.
 *
 *  @author Hooman Salamat
 *  @bug No known bugs.
 */

 // CPP program to demonstrate working of
 // reinterpret_cast
#include <iostream>
using namespace std;

int main()
{
    int* p = new int(65);
    char* ch = reinterpret_cast<char*>(p);
    cout << *p << endl;
    cout << *ch << endl;
    cout << p << endl;
    cout << ch << endl;
    return 0;
}