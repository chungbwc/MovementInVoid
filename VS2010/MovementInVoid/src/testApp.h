#pragma once

#include <opencv2/opencv.hpp>

#include "ofMain.h"
#include "ofxNetwork.h"
#include "TGConnect.h"
#include "TGEventData.h"
#include "MoTempl.h"
#include "SOMRing.h"

#include "Poco/RegularExpression.h"

using Poco::RegularExpression;

const static int pins[8] = {2, 3, 9, 4, 5, 6, 7, 8};

class testApp : public ofBaseApp
{
private:
	ofVideoGrabber vidGrabber;
	ofTexture vidTexture;
	MoTempl mot;

	int capWidth;
	int capHeight;

	SOMRing ring;

//	ofArduino arduino1;
//	bool readyArduino1;
	ofSerial fan;
	float lastTime1;

	ofSerial port;
	float lastTime2;

	int angle;
	int angleStep;

	TGConnect tgc;
	TGEventData tgData;
	int blink;
	int att;
	int med;
	int mBlink;
	int signal;

	ofTrueTypeFont font;
	int masterAngle;

	void waveListener(TGEventData&);
	void blinkListener(int&);

public:
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
};
