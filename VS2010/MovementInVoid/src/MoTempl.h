//
//  MoTempl.h
//  Motion003
//
//  Created by Bryan Chung on 4/8/13.
//
//

#ifndef __Motion003__MoTempl__
#define __Motion003__MoTempl__

#include <opencv2/opencv.hpp>
#include <iostream>
#include "ofMain.h"
#include "MoResult.h"

using namespace std;

const static double MHI_DURATION = 1;
const static double MAX_TIME_DELTA = 0.5;
const static double MIN_TIME_DELTA = 0.05;
const static int N = 4;

class MoTempl
{
private:
	int last;
	int fWidth;
	int fHeight;

	vector<cv::Mat> buf;

	cv::Mat mhi;
	cv::Mat orient;
	cv::Mat mask;
	cv::Mat segmask;
	cv::Mat silh;
	double timestamp;

public:
	vector<cv::Rect> targets;

	MoTempl();
	void setup(int, int);
	cv::Mat update(ofVideoGrabber);
	vector<MoResult> getResult();
};

#endif /* defined(__Motion003__MoTempl__) */
