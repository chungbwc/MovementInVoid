//
//  SOMNode.cpp
//  Buddha001
//
//  Created by Bryan Chung on 4/8/13.
//
//

#include "SOMNode.h"

SOMNode::SOMNode(int _w)
{
	weights.resize(_w);
	for (int i=0; i<_w; i++)
	{
//		weights[i] = (float)rand() / (float)RAND_MAX;
		weights[i] = 0.0;
	}
}

void SOMNode::setPos(int _p)
{
	pos = _p;
}

int SOMNode::getPos()
{
	return pos;
}

int SOMNode::distanceTo(SOMNode _n)
{
	int temp = getPos() - _n.getPos();
	return abs(temp);
}

void SOMNode::setWeight(int _w, float _v)
{
	if (_w >= weights.size())
		return;
	weights[_w] = _v;
}

float SOMNode::getWeight(int _w)
{
	if (_w >= weights.size())
		return 0;
	return (float) weights.at(_w);
}

SOMVector SOMNode::getVector()
{
	return weights;
}

void SOMNode::adjustWeights(SOMVector _i, float _l, float _d)
{
	float wt, vw;
	for (int w=0; w<weights.size(); w++)
	{
		wt = (float) weights.at(w);
		vw = (float) _i.at(w);
		wt += _d * _l * (vw - wt);
		weights[w] = wt;
	}
}