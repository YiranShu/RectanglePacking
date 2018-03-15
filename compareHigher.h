#ifndef _CompareHigher_H_
#define _CompareHigher_H_
#include "texturePacking.h"
class CompHigher //descending order
{
public:
	bool operator ()(const Rect& item1, const Rect& item2)
	{
		return (item1.width > item2.width); //The more weight, the higher rank
	}
};
#endif