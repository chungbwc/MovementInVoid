//
//  SOMVector.cpp
//  Buddha001
//
//  Created by Bryan Chung on 4/8/13.
//
//

#include "SOMVector.h"

SOMVector::SOMVector()
{
}

float SOMVector::dist(SOMVector _v)
{
	if (_v.size() != size())
		return -999.0f;
	float sum = 0.0f;
	float temp;
	for (int i=0; i<size(); i++)
	{
		temp = at(i) - _v.at(i);
		temp *= temp;
		sum += temp;
	}
	return sum;
}
