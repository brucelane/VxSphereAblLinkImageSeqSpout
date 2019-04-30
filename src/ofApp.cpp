#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	link.setup(120);

	for (int i = 1; i <= 3; i++) {
		std::ostringstream sout;
		sout << "eye" << std::setfill('0') << std::setw(2) << i << ".jpg";
		ofImage image;
		image.load(sout.str());
		images.push_back(image);
	}
	ofBackground(0, 0, 0);
	ofDisableArbTex();
	ofEnableAlphaBlending();
	ofSetFrameRate(30);
	glEnable(GL_DEPTH_TEST);
	audioThreshold = 0.9;
	audioPeakDecay = 0.915;
	audioMaxDecay = 0.995;
	audioMirror = false;
	colormap.loadImage("eye03.jpg");
	bumpmap.loadImage("eye02.jpg");

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	gluQuadricNormals(quadric, GLU_SMOOTH);

	shader.load("shaders/displace.vert", "shaders/displace.frag");

	fftFile.setMirrorData(false);
	fftFile.setup();

	soundPlayer.loadSound("Eye to eye20180604.wav");
	soundPlayer.play();
	factor = 1.0f;
}

//--------------------------------------------------------------
void ofApp::update() {
	if (soundPlayer.getIsPlaying() == true) {
		fftFile.setThreshold(audioThreshold);
		fftFile.setPeakDecay(audioPeakDecay);
		fftFile.setMaxDecay(audioMaxDecay);
		fftFile.setMirrorData(audioMirror);
		fftFile.update();
		int numOfVerts = 512;

		float * audioData = new float[numOfVerts];
		fftFile.getFftPeakData(audioData, numOfVerts);
		audioValue = audioData[8];  // yg1: 220
	}
	//if (audioValue < 0.01) {
	//	//if (factor == 0.0f) factor = 0.0f;
	//	factor -= 0.1;
	//}
	//else {
	//	factor = 0.0f;
	//}
	//factor = ofMap(mouseY, 0.0f, ofGetHeight(), -1.0f, 1.0f);
	ofSetWindowTitle(ofToString(audioValue, 2) + " - " + ofToString(ofGetFrameNum(), 2));

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofxAbletonLink::Status status = link.update();

	// visualize the current status
	int quantum = (int)ceil(link.quantum());
	if (quantum >= 1) {
		int nbeat = (int)floor(status.beat) % quantum;
		ofSetColor(255);
		for (int i = 0; i < quantum; i++) {
			ofImage& image = images[nbeat % images.size()];
			//image.draw((ofGetWidth() - image.getWidth()) / 2, (ofGetHeight() - image.getHeight()) / 2);
		}
	}
	ofSetColor(255);

	maxHeight = audioValue * 300 + ofGetFrameNum() / 200;
	ofPushMatrix();
	shader.begin();
	shader.setUniformTexture("colormap", colormap, 1);
	shader.setUniformTexture("bumpmap", bumpmap, 2);
	shader.setUniform1f("maxHeight", maxHeight);
	//shader.setUniform1f("factor", factor);

	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	ofRotateY(360 * sinf(float(ofGetFrameNum()) / 500.0f));

	ofRotate(-90, 1, 0, 0);
	//gluSphere(quadric, 150, 400, 400);
	gluSphere(quadric, 100 + ofGetFrameNum() / 200, 400, 400);

	shader.end();
	ofPopMatrix();
	//ofSetColor(255);
	ofSetColor(0);
	ofDrawBitmapString("Tempo: " + ofToString(link.tempo()) + " Beats: " + ofToString(status.beat) + " Phase: " + ofToString(status.phase), 20, 20);
	ofDrawBitmapString("Number of peers: " + ofToString(link.numPeers()), 20, 40);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == OF_KEY_RIGHT) {
		link.setQuantum(link.quantum() + 1);
	}
	else if (key == OF_KEY_LEFT) {
		link.setQuantum(link.quantum() - 1);
	}
	else if (key == OF_KEY_UP) {
		link.setTempo(link.tempo() + 1);
	}
	else if (key == OF_KEY_DOWN) {
		link.setTempo(link.tempo() - 1);
	}
	if (key == ' ') soundPlayer.play();
	if (key == 'f')	 ofSetFullscreen(true);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

