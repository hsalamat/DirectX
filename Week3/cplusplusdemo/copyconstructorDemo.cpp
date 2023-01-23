/** @file copycontructorDemo.cpp
 *  @brief Copy Constructor
 *
 *  A copy constructor is a member function that initializes an object using another object of the same class.
 *  A copy constructor has the following general function prototype :
 *  ClassName(const ClassName& old_obj);
 *
 *  @author Hooman Salamat
 */



#include<iostream> 
using namespace std;

class Point
{
private:
    int x, y;
public:
    Point(int x1, int y1) { x = x1; y = y1; }

    // Copy constructor 
    Point(const Point& p2) { x = p2.x; y = p2.y; }

    int getX() { return x; }
    int getY() { return y; }
};

class Game
{
private:
    int x, y;
public:
    Game(int x1, int y1) { x = x1; y = y1; }

    // Copy constructor not allowed
    Game(const Game& p2) = delete;

    int getX() { return x; }
    int getY() { return y; }
};

int main()
{
    Point p1(10, 15); // Normal constructor is called here 
    Point p2 = p1; // Copy constructor is called here 

    // Let us access values assigned by constructors 
    cout << "p1.x = " << p1.getX() << ", p1.y = " << p1.getY();
    cout << "\np2.x = " << p2.getX() << ", p2.y = " << p2.getY();

    Game g1(10, 15);
    //Game g2 = g1; copy constructor not allowed

    //If we don’t define our own copy constructor, the C++ compiler creates a default copy constructor for each class which does a member - wise copy between objects.
    //The compiler created copy constructor works fine in general.We need to define our own copy constructor only if an object has pointers or any runtime allocation 
    //of the resource like file handle, a network connection..etc.

    return 0;
}