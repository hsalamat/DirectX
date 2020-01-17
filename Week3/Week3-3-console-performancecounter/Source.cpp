#include<Windows.h>
#include<iostream>
#include "GameTimer.h"
using namespace std;
int main()
{

	//long int			4bytes  -2,147,483,648 to 2,147,483,647
	//unsigned long int	4bytes 	0			   to 4, 294, 967, 295
	//long long int		8bytes  -(2^63)		   to (2^63) - 1
	//LARGE_INTEGER is a union of a 64-bit integer and a pair of 32-bit integers. If you want to perform 64-bit arithmetic on one you need to select the 64-bit int from inside the union.

	LARGE_INTEGER StartingTime, EndingTime, DifferenceOfTime,deltaTime;
	LARGE_INTEGER frequency; //default frequency that is supported by the hardware


	//The performance timer measures time in units called counts.
	QueryPerformanceCounter(&StartingTime);
	cout << "How long does it take to execute this line?" << endl;
	QueryPerformanceCounter(&EndingTime);

	cout << "Starting Time: " << StartingTime.QuadPart << endl;
	cout << "Ending Time: " << EndingTime.QuadPart << endl;

	//To get the frequency(counts per second) of the performance timer
	//the number of seconds (or fractions of a second) per count is just the reciprocal of the counts per second : mSecondsPerCount = 1.0 / (double)countsPerSec;
	QueryPerformanceFrequency(&frequency);

	//QuadPart represents a 64-bit signed integer (for example: 670644208300)
	//Every LargeInteger is a union of High Part (long:156 ) and Low Part (unsigned long: 629310124)


	DifferenceOfTime.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;

	cout << "Difference Of Time: " << DifferenceOfTime.QuadPart << endl;
	cout << "Frequency: " << frequency.QuadPart << endl;

	//how about calculating this directly without quadpart
	//deltaTime = EndingTime - StartingTime; //no operator matches this operands . Because a large integer is a union!

	DifferenceOfTime.QuadPart *= 1000000; //it's in microseconds

	//To convert a time reading valueInCounts to microseconds, we just divide it by frequency
	DifferenceOfTime.QuadPart /= frequency.QuadPart; //For Tick per seconds

	DifferenceOfTime.QuadPart /= 1000; //the value is in milliseconds now

	cout << "Execution Time in milliseconds is : " << DifferenceOfTime.QuadPart << endl;

	//How can we use GameTimer class
	cout << "Using GameTimer : " << endl;

	GameTimer mTimer;

	mTimer.Reset(); //the first frame of animation, there is no previous frame. This initializes mPrevTime to the current time

	int message = 10;

	while (message != 0)
	{
			mTimer.Start();  //Call when unpaused.
			mTimer.Tick();   //Call every frame.

			cout << "the delta time now is : " << mTimer.DeltaTime() * 1000 << endl; //in milliseconds
			
			// Returns the total time elapsed since Reset() was called, NOT counting any  time when the clock is stopped.
			cout << "the total time now is : " << mTimer.TotalTime() * 1000 << endl; //in milliseconds
			mTimer.Stop(); //Call when paused.

			Sleep(1000);	
			message--;
		
	}


	system("pause");
	return 0;
}