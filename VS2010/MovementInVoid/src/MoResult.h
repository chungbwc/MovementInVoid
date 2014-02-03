//
//  MoResult.h
//  Motion003
//
//  Created by Bryan Chung on 4/8/13.
//
//

#ifndef __Motion003__MoResult__
#define __Motion003__MoResult__

#include <iostream>

class MoResult
{
public:
	int x;
	int y;
	int w;
	int h;
	float angle;

	MoResult(int, int, int, int, float);
};

#endif /* defined(__Motion003__MoResult__) */
