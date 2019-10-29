#pragma once

#include "ofMain.h"
#include "ofxAbletonLink.h"
#include "ofxFFTFile.h"
#include "ofxSpout2Sender.h"
#include "ofxXmlSettings.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void exit();
	
private:
	ofxAbletonLink link;
	//vector<ofImage> images;
	void loadImage();

	ofImage colormap, bumpmap;
	int imgIndex;
	GLUquadricObj *quadric;
	// spout
	ofxSpout2::Sender spout;
	ofFbo 						fbo;
	ofShader 					shader;
	int 						targetWidth, targetHeight;
	ofxXmlSettings 				settings;

	ofSoundPlayer soundPlayer;
	float audioThreshold;
	float audioPeakDecay;
	float audioMaxDecay;
	bool audioMirror;
	ofxFFTFile fftFile;
	float audioValue;
	float factor;
	float maxHeight;
	ofVec3f  twod;
	float angleX;
	float angleY;
	float angleZ;
	float startTime;
	float currentTime;
	bool isPlaying;
};
