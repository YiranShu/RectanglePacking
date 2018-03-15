#include <vector>
#include <glut.h>
#include <iostream>
#include <algorithm>
#include "compareHigher.h"
#include "texturePacking.h"
using namespace std;

#define screenHeight 480
#define screenWidth 640
TexturePacking instance;

template<typename T>
void Swap(T &x, T &y)
{
	T temp = x;
	x = y;
	y = temp;
}

//to accept input
bool TexturePacking::Input()
{
	int num; //the number of rectangles
	Rect temp; //temp for input the shape of rectangles
	bool flag = true;

	cout << "Please input the width of the largest rectangle: ";
	cin >> totalWidth; //input the width of the largest rectangle
	cout << "How many rectangles? ";
	cin >> num;
	for (int i = 1; i <= num; i++)
	{
		cout << "Please input the width and height of Rect" << i << ": ";
		cin >> temp.width >> temp.height;
		if (temp.width > totalWidth && temp.height > totalWidth)
		{
			flag = false; //the given rect is so big that the large one cannot contain it 
		}
		else if (temp.width > totalWidth && totalWidth >= temp.height)
		{
			Swap(temp.height, temp.width);//width > total >= height, swap. 
		}
		else if (totalWidth >= temp.height && temp.height > temp.width)
		{
			Swap(temp.height, temp.width);//total >= height > width, swap.
		}
		rectangles.push_back(temp); //it's a valid rectangle
	}//Now for every rectangle, total >= width >= height or height > total >= width
	return flag; //all given rects can be contained by the large one
}

//sort input rectangles by their width
void TexturePacking::sortRect()
{ //The wider a rectangle is, the higher it ranks.
	sort(rectangles.begin(), rectangles.end(), CompHigher());
}

//insert small rectangles into the large one
void TexturePacking::execPacking()
{
	Hole hole; //for temp use
	vector<Hole>::iterator hit1, hit2;
	vector<Rect>::iterator rit;//point to the rectangle to insert
	int holesHeight;//the height of all holes
	bool set;//if the new rectangle is set, 'set' is true. Otherwise, 'set' is false
	int m, n; //m is for x-coordinate of the left bottom vertex of the new hole
			 //n is to restore hit1 if hit1 is influenced while inserting a new hole

	for (rit = rectangles.begin(); rit != rectangles.end(); rit++)
	{
		set = false;
		if (holes.empty())//if there is no holes
		{                 //add a new rectangle and a new hole
			rit->x = 0; //complete the property of the new rectangle
			rit->y = totalHeight; //complete the property of the new rectangle
			hole.x = rit->width; //set the property of the new hole
			hole.y = totalHeight; //set the property of the new hole
			hole.height = rit->height; //set the property of the new hole
			hole.width = totalWidth - rit->width; //set the property of the new hole
			holes.push_back(hole); //put the new hole in
			totalHeight += rit->height;//update totalHeight
		}
		else
		{
			for (hit1 = holes.begin(); hit1 != holes.end(); hit1++)
			{
				holesHeight = totalHeight - hit1->y; //compute the total height of all holes
				if (holesHeight >= rit->height && hit1->width >= rit->width)//put it in
				{
					set = true;
					rit->x = hit1->x;
					rit->y = hit1->y;
					for (hit2 = holes.begin(); hit2 != hit1; hit2++)
					{
						if (hit2->width > hit1->width - rit->width)
						{//keep the width of the hole below no more than that of the hole above
							hit2->x += hit2->width - (hit1->width - rit->width);
							hit2->width = hit1->width - rit->width;
						}
					}
					m = totalWidth - hit1->width + rit->width;
					if (hit1->height >= rit->height)
					{//create a new hole
						hole.x = m;
						hole.width = totalWidth - hole.x;
						hole.y = hit1->y;
						hole.height = rit->height;
						//hit1's height and y-coordinate of the left bottom vertex is influenced
						hit1->height -= rit->height;
						hit1->y += rit->height;
						hit1 = holes.insert(hit1, hole);
					}
					else
					{
						for (hit2 = hit1 + 1; hit2 != holes.end(); hit2++)
						{//if the original holes are covered, delete the holes
							//cout << "!";
							if (hit2->y + hit2->height < hit1->y + rit->height)
							{//if the original holes are covered, delete the holes
								hit2->x = m;
								hit2->width = totalWidth - hit2->x;
							}
							else break;
						}
						//create a new hole
						hole.height = hit2->height - (hit2->y + hit2->height - (hit1->y + rit->height));
						hole.x = m;
						hole.width = totalWidth - hole.x;
						hole.y = hit2->y;
						n = hit2 - hit1;//next operation will influence hit1, so n is to help restore hit1 by hit2
						hit2 = holes.insert(hit2, hole) + 1; //hit1 is influenced
						hit1 = hit2 - n - 1; //restore hit1
						hit2->y = hole.y + hole.height;//update hit2's y-coordinate of the left bottom vertex
						hit2->height -= hole.height;//update hit2's height
						hit1->x = m;
						hit1->width = totalWidth - hit1->x;
						if (hit2->height == 0)
							hit2 = holes.erase(hit2);//delete the hole whose height is 0
					}
					break;
				}
				else if (holesHeight < rit->height || holesHeight == rit->height && hit1->width < rit->width)
				{//It is impossible to insert the new rectangle into holes
				 //add a new rectangle and a new hole
					set = true;
					rit->x = 0; //complete the property of the new rectangle
					rit->y = totalHeight; //complete the property of the new rectangle
					hole.x = rit->width; //set the property of the new hole
					hole.y = totalHeight; //set the property of the new hole
					hole.height = rit->height; //set the property of the new hole
					hole.width = totalWidth - hole.x; //set the property of the new hole
					holes.push_back(hole); //put the new hole in
					totalHeight += rit->height; //update totalHeight
					break;
				}
			}
			if (!set) //if the new rectangle is not set yet
			{
				rit->x = 0; //complete the property of the new rectangle
				rit->y = totalHeight; //complete the property of the new rectangle
				hole.x = rit->width; //set the property of the new hole
				hole.y = totalHeight; //set the property of the new hole
				hole.height = rit->height; //set the property of the new hole
				hole.width = totalWidth - rit->width; //set the property of the new hole
				holes.push_back(hole); //put the new hole in
				totalHeight += rit->height; //update totalHeight
			}
		}
	}
}

//get the variable totalHeight for output
int TexturePacking::getTotalHeight()
{
	return totalHeight;
}

//clear the information in the instance for next input
void TexturePacking::clearObject()
{
	totalHeight = 0; //restore totalHeight
	rectangles.clear();
	holes.clear();
}

void myInit()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0, 0.5, 0.0);
	glLineWidth(1.0);
}

void setWindow(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left, right, bottom, top);
}

void setViewPort(GLint left, GLint right, GLint bottom, GLint top)
{
	glViewport(left, bottom, right - left, top - bottom);
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void myMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{//next case
		glBegin(GL_LINE_LOOP);
		glVertex2i(0, 0);
		glVertex2i(instance.totalWidth, 0);
		glVertex2i(instance.totalWidth, instance.totalHeight);
		glVertex2i(0, instance.totalHeight);
		glEnd();
		glFlush();
		for (unsigned int i = 0; i < instance.rectangles.size(); i++)
		{
			glBegin(GL_LINE_LOOP);
			glVertex2i(instance.rectangles[i].x, instance.rectangles[i].y);
			glVertex2i(instance.rectangles[i].x + instance.rectangles[i].width, instance.rectangles[i].y);
			glVertex2i(instance.rectangles[i].x + instance.rectangles[i].width, instance.rectangles[i].y + instance.rectangles[i].height);
			glVertex2i(instance.rectangles[i].x, instance.rectangles[i].y + instance.rectangles[i].height);
			glEnd();
			glFlush();
		}
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(150, 100);
	glutInitWindowSize(screenWidth, screenHeight);
	glutCreateWindow("Demo");
	myInit();
	glutDisplayFunc(&myDisplay);
	if (instance.Input())
	{
		instance.sortRect();//sort input rectangles by their width
		instance.execPacking(); //insert small rectangles into the large one
		setWindow(0, instance.totalWidth, 0, instance.totalHeight);
		cout << "The height of the large rectangle is " << instance.getTotalHeight() << endl;
		glutMouseFunc(&myMouse);
	}
	else
		cout << "The large rectangle cannot contain these rectangles!" << endl;
	setViewPort(0, screenWidth, 0, screenHeight);
	glutMainLoop();
	
	return 0;
}