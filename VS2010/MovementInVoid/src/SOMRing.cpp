//
//  SOMRing.cpp
//  Buddha001
//
//  Created by Bryan Chung on 4/8/13.
//
//

#include "SOMRing.h"

SOMRing::SOMRing():node(NULL)
{
	featureSize = 8;
	ringSize = 8;
	iterations = 10000;
	learningRate = 0.01f;
	t = 0;
	vec.resize(featureSize);
}

void SOMRing::setup()
{
	for (int i=0; i<ringSize; i++)
	{
		SOMNode myNode(featureSize);
		myNode.setPos(i);
		ring.push_back(myNode);
	}
	lambda = (float)iterations / log((float)ringSize/2.0);
	node = ring.at(0);
}

SOMNode SOMRing::getNode(int _p)
{
	return ring.at(_p);
}

int SOMRing::getSize()
{
	return ringSize;
}

SOMNode SOMRing::getBestMatch(SOMVector _v)
{
	SOMNode bmu = ring.at(0);
	float bestDist = _v.dist(bmu.getVector());
	float curDist;

	for (int i=0; i<ringSize; i++)
	{
		curDist = _v.dist(ring.at(i).getVector());
		if (curDist < bestDist)
		{
			bmu = ring.at(i);
			bestDist = curDist;
		}
	}
	return bmu;
}

void SOMRing::updateRing()
{
	if (t >= iterations)
	{
		t = 0;
		return;
	}
	float radius = (ringSize/2) * exp(-t / lambda);
	float rate = learningRate * exp(-t / lambda);

	node = getBestMatch(vec);
	int v1 = node.getPos();

	for (int i=0; i<ringSize; i++)
	{
		float posDiff = abs(ring[i].getPos() - v1);
		if (posDiff > ringSize/2)
		{
			posDiff = ringSize - posDiff;
		}
		if (posDiff < radius)
		{
			float phi = exp(-pow(posDiff,2) / (2*pow(radius,2)));
			ring[i].adjustWeights(vec, rate, phi);
		}
	}
	t++;
}

void SOMRing::train(SOMVector _v)
{
	vec = _v;
}

