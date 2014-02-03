//
//  SOMNode.h
//  Buddha001
//
//  Created by Bryan Chung on 4/8/13.
//
//

#ifndef __Buddha001__SOMNode__
#define __Buddha001__SOMNode__

#include <iostream>
#include "SOMVector.h"

class SOMNode
{
private:
	SOMVector weights;
	int pos;

public:
	static int num;

	SOMNode(int);
	int distanceTo(SOMNode);
	void setPos(int);
	int getPos();
	void setWeight(int, float);
	float getWeight(int);
	SOMVector getVector();
	void adjustWeights(SOMVector, float, float);
};

#endif /* defined(__Buddha001__SOMNode__) */
