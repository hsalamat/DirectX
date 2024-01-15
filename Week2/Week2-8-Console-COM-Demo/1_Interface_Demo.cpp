#include <iostream>
#include <string>

using namespace std;

//An interface describes the behavior or capabilities of a C++ class without committing to a particular implementation of that class.

//The C++ interfaces are implemented using abstract classes and these abstract classes should not be confused with data abstraction which is a concept of keeping implementation details separate from associated data.

//A class is made abstract by declaring at least one of its functions as pure virtual function.A pure virtual function is specified by placing "= 0" in its declaration as follows −

class IDrawable
{
public:
	virtual void Draw() = 0;
};

//Designing Strategy
//An object - oriented system might use an abstract base class to provide a common and standardized interface appropriate for all the external applications.
// Then, through inheritance from that abstract base class, derived classes are formed that operate similarly.

class Shape : public IDrawable
{
public:
	virtual void Draw() {
		cout << "Draw Shape: Rasterize a set of lines " << endl;
	}
	string name;
private:
	
};

Shape* CreateShape()
{
	Shape triangleShape;
	triangleShape.name = "MyShape";
	return &triangleShape;
}

class Bitmap : public IDrawable
{
public:
	virtual void Draw() {
		cout << "Draw Bitmap: Blit an array of pixels " << endl;
	}
	string name;
private:
};

Bitmap* CreateBitmap()
{
	Bitmap bitmap;
	bitmap.name = "MyPicture.bmp";
	return &bitmap;
}

//Becarefull using this function: IDrawble is just an interface!
// it is important to remember that interfaces are not objects. They are collections of methods that objects must implement.
void DrawSomeShapes(IDrawable** drawableArray, size_t count)
{
	for (int i = 0; i < count; i++)
	{
		drawableArray[i]->Draw();
	}
}
int main(void)
{
	//you can not create an instance of an IDrwable!
	//IDrawable draw;
	//draw.Draw();

	//A Program using this graphics library would manipulate Shape or Bitmap objects through IDrawable pointers, rather than Shape or Bitmap directly
	IDrawable* drawableArray[5];

	IDrawable* sDrawable = CreateShape();
	if (sDrawable)
	{
		sDrawable->Draw();
	}


	IDrawable* bDrawable = CreateBitmap();
	if (bDrawable)
	{
		bDrawable->Draw();
	}


	drawableArray[0] = CreateShape();
	drawableArray[1] = CreateShape();
	drawableArray[2] = CreateBitmap();
	DrawSomeShapes(drawableArray, 3);




	return 0;
}