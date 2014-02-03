//
//  SOMRing.h
//  Buddha001
//
//  Created by Bryan Chung on 4/8/13.
//
//

#ifndef __Buddha001__SOMRing__
#define __Buddha001__SOMRing__

#include <iostream>
#include "SOMNode.h"

class SOMRing
{
private:
	int ringSize;
	vector<SOMNode> ring;
	int featureSize;
	int iterations;
	int t;
	float learningRate;
	float lambda;
	SOMNode node;
	SOMVector vec;

public:
	SOMRing();
	void setup();
	SOMNode getNode(int);
	int getSize();
	SOMNode getBestMatch(SOMVector);
	void updateRing();
	void train(SOMVector);
};


#endif /* defined(__Buddha001__SOMRing__) */
