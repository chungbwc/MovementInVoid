#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0, 0, 0);
	ofSetBackgroundAuto(false);
	ofSetVerticalSync(true);
	ofSetCircleResolution(72);
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	ofSetFrameRate(30);
	font.loadFont("font/DroidSansMono.ttf", 12);

	capWidth = 640;
	capHeight = 480;

	//	vidGrabber.setVerbose(true);
	vidGrabber.setDeviceID(0);
	vidGrabber.initGrabber(capWidth, capHeight);
	vidTexture.allocate(capWidth, capHeight, GL_RGB);

	ring.setup();
	mot.setup(capWidth, capHeight);

	port.listDevices();

//	arduino1.connect("\\\\.\\COM10");
//	readyArduino1 = false;
	port.setup("COM9", 9600);
	fan.setup("\\\\.\\COM10", 9600);

	tgc.setup();
	att = 0.0;
	med = 0.0;
	signal = 200;
	blink = 0;
	mBlink = 0;

	ofAddListener(tgc.waveChangeEvent, this, &testApp::waveListener);
	ofAddListener(tgc.blinkEvent, this, &testApp::blinkListener);

	masterAngle = -1;
	angle = 0;
	angleStep = -90;

	lastTime1 = ofGetElapsedTimef();
	lastTime2 = ofGetElapsedTimef();

	fan.writeByte('<');
	for (int i=0; i<8; i++) 
	{
		fan.writeByte((unsigned char) 0);
	}
	fan.writeByte('>');

	tgc.startThread(true, false);
}

//--------------------------------------------------------------
void testApp::update(){
	vidGrabber.update();
	vidTexture.loadData((mot.update(vidGrabber)).data, capWidth, capHeight, GL_RGB);
	vidTexture.loadData(vidGrabber.getPixels(), capWidth, capHeight, GL_RGB);
/*
	if (arduino1.isArduinoReady() && !readyArduino1) 
	{
		readyArduino1 = true;
		ofLog() << "Arduino 1 is ready.";
		for (int i=0; i<ring.getSize(); i++)
		{
			arduino1.sendDigitalPinMode(pins[i], ARD_OUTPUT);
			//			arduino1.sendDigitalPinMode(pins[i], ARD_PWM);
		}
	}

	arduino1.update();
*/
	tgc.lock();
	if (att < tgData.attention)
	{
		att++;
	} else if (att > tgData.attention)
	{
		att--;
	}
	if (med < tgData.meditation)
	{
		med++;
	} else if (med > tgData.meditation)
	{
		med--;
	}
	if (mBlink > 0) 
	{
		mBlink--;
	}
	tgc.unlock();
	signal = tgc.getSignalQuality();
	if (signal == 0)
	{
		ring.updateRing();
	}

	bool updateServo = (ofGetElapsedTimef() - lastTime2) > 2.0;
	if (updateServo) 
	{
		float r = med/2.0;
//		float r = 50.0;
		int x = r*cos(ofDegToRad(angle)) + 50;
		int y = r*sin(ofDegToRad(angle)) + 50;
		angle += angleStep;
		angle %= 360;

		unsigned char att1 = x;
		unsigned char med1 = y;
		port.writeByte('<');
		port.writeByte(att1);
		port.writeByte(':');
		port.writeByte(med1);
		port.writeByte('>');
		lastTime2 = ofGetElapsedTimef();
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0, 0, 0);
	vidTexture.draw(0, 0);

	ofPushStyle();
	vector<MoResult> tRes = mot.getResult();
	float sumAll = 0, sumRad = 0;
	for (vector<MoResult>::iterator it = tRes.begin(); it!=tRes.end(); ++it)
	{
		int x = (*it).x;
		int y = (*it).y;
		int w = (*it).w;
		int h = (*it).h;
		float angle = (*it).angle;

		int cx = x + w/2;
		int cy = y + h/2;

		int r = min(w, h)/2;

		sumRad += r;
		sumAll += r*angle;
		ofSetColor(255, 255, 100, 100);
		ofCircle(cx, cy, r);
		ofSetColor(0, 0, 200);
		ofLine(cx, cy, cx + r*cos(ofDegToRad(angle)), cy + r*sin(ofDegToRad(angle)));
	}
	if (sumRad > 0) 
	{
		masterAngle = floor((sumAll/sumRad)/45);
	}
	ofPopStyle();

	int ox = 960;
	int oy = 240;
	float rad = 160.0;
	float del = 10.0;

	bool updatePwm = (ofGetElapsedTimef() - lastTime1) > 10.0;
	unsigned char val[8];
	ofPushStyle();
	for (int i=0; i<ring.getSize(); i++)
	{
		SOMVector v = ring.getNode(i).getVector();
		ofSetColor(ofFloatColor(v[1], v[2], v[3]));
		float angle = i*45.0;
		float tx = rad*cos(ofDegToRad(angle));
		float ty = rad*sin(ofDegToRad(angle));
		ofCircle(ox + tx, oy + ty, 30+del*v[0]);
		ofSetColor(ofFloatColor(v[5], v[6], v[7]));
		ofCircle(ox + tx, oy + ty, 20+del*v[4]);

		if (updatePwm) 
		{
			float sum = 0.0;
			for (int j=0; j<v.size(); j++)
			{
				sum += v[j];
			}
			sum = ofClamp(sum*0.75, 0.0, 1.0)*99;
			/*
			if (sum > 0.6) 
			{
				arduino1.sendDigital(pins[i], ARD_HIGH);
			} else 
			{
				arduino1.sendDigital(pins[i], ARD_LOW);
			}
			*/
//			ofLog() << i << ", value: " << sum;
			if (i == masterAngle) 
			{
				val[i] = (unsigned char) 99;
			} else 
			{
				val[i] = (unsigned char) sum;
			}
		}
	}
	ofPopStyle();
	if (updatePwm) 
	{
		fan.writeByte('<');
		for (int i=0; i<8; i++) 
		{			
//			ofLog() << "Index: " << i << ", value: " << (int) val[i];			
			fan.writeByte(val[i]);		
		}
		fan.writeByte('>');
		lastTime1 = ofGetElapsedTimef();
	}

	ofPushStyle();
	ofSetColor(ofColor::yellow);
	if (signal == 0)
	{
		font.drawString("Status: good", 60, 900);
	} else if (signal == 200) 
	{
		font.drawString("Status: disconnect", 60, 900);
	} else {
		font.drawString("Status: poor", 60, 900);
	}
	font.drawString("D   T   A1  A2  B1  B2  G1  G2  BK", 65, 850);
	//	font.drawString("Attention:  ", 50, 800);
	//	font.drawString("Meditation: ", 50, 830);
	//	font.drawString("Blink:      ", 50, 860);
	//	ofRect(180, 790, att, 15);
	//	ofRect(180, 820, med, 15);
	//	ofRect(180, 850, mBlink, 15);
	float temp;
	temp = tgData.delta*200;
	ofRect(60, 800-temp, 30, temp);
	temp = tgData.theta*200;
	ofRect(100, 800-temp, 30, temp);
	temp = tgData.alpha1*200;
	ofRect(140, 800-temp, 30, temp);
	temp = tgData.alpha2*200;
	ofRect(180, 800-temp, 30, temp);
	temp = tgData.beta1*200;
	ofRect(220, 800-temp, 30, temp);
	temp = tgData.beta2*200;
	ofRect(260, 800-temp, 30, temp);
	temp = tgData.gamma1*200;
	ofRect(300, 800-temp, 30, temp);
	temp = tgData.gamma2*200;
	ofRect(340, 800-temp, 30, temp);
	temp = mBlink*200/255.0;
	ofRect(380, 800-temp, 30, temp);
	ofPopStyle();

	ofPushStyle();
	ofSetColor(ofColor::aqua);
	ofLine(760, 520, 1160, 520);
	ofLine(1160, 520, 1160, 920);
	ofLine(1160, 920, 760, 920);
	ofLine(760, 920, 760, 520);
	int x = med*400/100;
	int y = att*400/100;
	ofSetColor(ofColor::orange);
	ofLine(760+x, 520, 760+x, 920);
	ofLine(760, 920-y, 1160, 920-y);
	font.drawString("attention", 765, 915-y);
	font.drawString("meditation", 765+x, 915);
	ofPopStyle();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key = 'x') 
	{
		unsigned char x = 50;
		unsigned char y = 50;
		port.writeByte('<');
		port.writeByte(x);
		port.writeByte(':');
		port.writeByte(y);
		port.writeByte('>');

		fan.writeByte('<');
		for (int i=0; i<8; i++) 
		{
			fan.writeByte((unsigned char) 0);
		}
		fan.writeByte('>');
	}
	exit();
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	ofLog() << ofGetFrameRate();
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

void testApp::waveListener(TGEventData &param)
{
	tgData = param;
	SOMVector v;
	v.resize(8);
	v[0] = tgData.alpha1;
	v[1] = tgData.alpha2;
	v[2] = tgData.beta1;
	v[3] = tgData.beta2;
	v[4] = tgData.gamma1;
	v[5] = tgData.gamma2;
	v[6] = tgData.delta;
	v[7] = tgData.theta;
	ring.train(v);
}

void testApp::blinkListener(int &param)
{
	blink = param;
	mBlink = blink;
/*
	if (blink > 80)
	{
		for (int i=0; i<8; i++)
		{
			arduino1.sendDigital(pins[i], ARD_HIGH);
		}
	}
	*/
}

void testApp::exit()
{
//	arduino1.disconnect();
	port.close();
	tgc.stopThread();
}