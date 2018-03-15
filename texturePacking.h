#ifndef _TEXTUREPACKING_H_
#define _TEXTUREPACKING_H_
#include <vector>
using namespace std;

class Hole
{
public:
	int height; //the height of the hole after inserting a rectangle
	int width; //the width of the hole after inserting a rectangle
	int x; //the x-coordinate of the left bottom vertex of a hole
	int y; //the y-coordinate of the left bottom vertex of a hole
};

struct Rect
{
	int height; //the height of every rectangle
	int width; //the width of every rectangle
	int x; //the x-coordinate of the left bottom vertex of a rectangle
	int y; //the y-coordinate of the left bottom vertex of a rectangle
};

class TexturePacking
{
public:
	int totalWidth; //user inputs the total width of the rectangle which contains others
	int totalHeight; //the program calculates the total width of the rectangle which contains others
	vector<Rect> rectangles; //all rectangles
	vector<Hole> holes; //all available holes

public:
	TexturePacking() : totalHeight(0) {} //initial
	bool Input(); //to accept input
	void sortRect();//to sort input rectangles by their width
	void execPacking();//to insert small rectangles into the large one
	int getTotalHeight(); //get the variable totalHeight for output
	void clearObject(); //clear the information in the instance for next input
};
#endif