//
//  MoTempl.cpp
//  Motion003
//
//  Created by Bryan Chung on 4/8/13.
//
//

#include "MoTempl.h"

MoTempl::MoTempl()
{
}

void MoTempl::setup(int _w, int _h)
{
	fWidth = _w;
	fHeight = _h;
	last = 0;
	buf.resize(N);
	for (int i=0; i<N; i++)
	{
		buf[i] = cv::Mat::zeros(fHeight, fWidth, CV_8UC1);
	}
	mhi = cv::Mat::zeros(fHeight, fWidth, CV_32FC1);
	orient = cv::Mat::zeros(fHeight, fWidth, CV_32FC1);
	segmask = cv::Mat::zeros(fHeight, fWidth, CV_32FC1);
	mask = cv::Mat::zeros(fHeight, fWidth, CV_8UC1);
}

cv::Mat MoTempl::update(ofVideoGrabber _v)
{
	cv::Mat frame(_v.getHeight(), _v.getWidth(), CV_8UC3, _v.getPixels());

	int idx2;
	int idx1 = last;
	timestamp = ofGetElapsedTimef();
	cv::Rect comp;

	cv::cvtColor(frame, buf[last], CV_BGR2GRAY);
	idx2 = (last+1) % N;
	last = idx2;

	silh = buf[idx2];
	cv::absdiff(buf[idx1], buf[idx2], silh);
	cv::threshold(silh, silh, 30, 1, CV_THRESH_BINARY);

	cv::updateMotionHistory(silh, mhi, timestamp, MHI_DURATION);
	mhi.convertTo(mask, CV_8UC1, 255.0/MHI_DURATION, (MHI_DURATION - timestamp)*255.0/MHI_DURATION);

	cv::Mat dst = cv::Mat::zeros(fHeight, fWidth, CV_8UC3);
	cv::Mat emp = cv::Mat::zeros(fHeight, fWidth, CV_8UC1);
	cv::Mat temp [] = {emp, emp, mask};
	cv::merge(temp, 3, dst);
	cv::calcMotionGradient(mhi, mask, orient, MAX_TIME_DELTA, MIN_TIME_DELTA, 3);
	cv::segmentMotion(mhi, segmask, targets, timestamp, MAX_TIME_DELTA);

	return dst;
}

vector<MoResult> MoTempl::getResult()
{
	vector<MoResult> result;

	for (vector<cv::Rect>::iterator it = targets.begin(); it!=targets.end(); ++it)
	{
		int x = (*it).x;
		int y = (*it).y;
		int w = (*it).width;
		int h = (*it).height;

		if ((w+h) < 150)
			continue;

		cv::Mat silhroi(silh, cv::Rect(x, y, w, h));
		cv::Mat mhiroi(mhi, cv::Rect(x, y, w, h));
		cv::Mat orientroi(orient, cv::Rect(x, y, w, h));
		cv::Mat maskroi(mask, cv::Rect(x, y, w, h));

		double angle = cv::calcGlobalOrientation(orientroi, maskroi, mhiroi, timestamp, MHI_DURATION);
		angle = 360.0 - angle;

		float count = cv::norm(silhroi, CV_L1);
		if (count < (w*h*0.05))
			continue;

		MoResult res(x, y, w, h, (float)angle);
		result.push_back(res);
	}
	return result;
}

