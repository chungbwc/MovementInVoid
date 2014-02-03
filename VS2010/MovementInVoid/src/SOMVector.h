//
//  SOMVector.h
//  Buddha001
//
//  Created by Bryan Chung on 4/8/13.
//
//

#ifndef __Buddha001__SOMVector__
#define __Buddha001__SOMVector__

#include <iostream>
#include <vector>

using namespace std;

class SOMVector : public vector<float>
{
public:
	SOMVector();
	float dist(SOMVector);
};

#endif /* defined(__Buddha001__SOMVector__) */
