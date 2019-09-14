#include "ofApp.h"
// idees passer du logo 2d à 3D
//--------------------------------------------------------------
void ofApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
	// load settings.xml
	if (settings.loadFile("vxsphere.xml") == false) {
		ofLog() << "vxsphere.xml not found";
	}
	settings.pushTag("settings");
	targetWidth = settings.getValue("targetWidth", 1280);
	targetHeight = settings.getValue("targetHeight", 720);
	ofLogNotice("targetWidth: " + ofToString(targetWidth) + " targetHeight: " + ofToString(targetHeight));
	fbo.allocate(targetWidth, targetHeight);
	fbo.begin();
	ofClear(0, 0, 0, 0);
	fbo.end();

	link.setup(120);

	/*for (int i = 1; i <= 8; i++) {
		std::ostringstream sout;
		sout << "p00" << std::setfill('0') << std::setw(2) << i << ".jpg";
		ofImage image;
		image.load(sout.str());
		images.push_back(image);
	} */
	imgIndex = 1;
	ofBackground(0, 0, 0);
	ofDisableArbTex();
	ofEnableAlphaBlending();
	ofSetFrameRate(30);
	glEnable(GL_DEPTH_TEST);
	audioThreshold = 0.9;
	audioPeakDecay = 0.915;
	audioMaxDecay = 0.995;
	audioMirror = false;
	colormap.loadImage("p-(1).jpg");
	bumpmap.loadImage("p-(1).jpg");

	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	gluQuadricNormals(quadric, GLU_SMOOTH);

	shader.load("shaders/displace.vert", "shaders/displace.frag");

	fftFile.setMirrorData(false);
	fftFile.setup();

	soundPlayer.loadSound("massactivextrait.wav");
	//soundPlayer.play();
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
	ofSetWindowTitle("angle: " + ofToString(angleX, 2) + "img: " + ofToString(imgIndex, 2) + " mult: " + ofToString(factor, 2) + " - " + ofToString(audioValue, 2) + " - " + ofToString(ofGetFrameNum(), 2));

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofxAbletonLink::Status status = link.update();

	// visualize the current status
	/*int quantum = (int)ceil(link.quantum());
	if (quantum >= 1) {
		int nbeat = (int)floor(status.beat) % quantum;
		ofSetColor(255);
		for (int i = 0; i < quantum; i++) {
			ofImage& image = images[nbeat % images.size()];
			//image.draw((ofGetWidth() - image.getWidth()) / 2, (ofGetHeight() - image.getHeight()) / 2);
		}
	}
	ofImage& image = images[ofGetFrameNum() / 200 % images.size()];
	*/
	ofSetColor(255);

	maxHeight = audioValue * 50 * factor + ofGetFrameNum() / 200;
	fbo.begin();
	ofClear(0, 0, 0, 0);
	ofPushMatrix();
	twod.set(0.30, 0.59, 0.11);
	//twod.set(0.0, 0.0, 0.0);
	shader.begin();
	shader.setUniformTexture("colormap", colormap, 1);
	shader.setUniformTexture("bumpmap", bumpmap, 2);
	//shader.setUniformTexture("colormap", image, 1);
	//shader.setUniformTexture("bumpmap", image, 2);
	shader.setUniform1f("maxHeight", maxHeight);
	shader.setUniform3f("twod", twod);
	shader.setUniform1f("time", ofGetElapsedTimef());
	//shader.setUniform1f("factor", factor);
	//ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	ofTranslate(targetWidth / 2, targetHeight / 2);
	angleX = 360 + (mouseX / 1.0 + 0.01) * sinf(float(ofGetFrameNum()) / 500.0f);
	angleY = 360 + (mouseY / 1.0 + 0.01) * sinf(float(ofGetFrameNum()) / 500.0f);
	ofRotateY(angleX);
	ofRotateX(angleY);
	ofRotate(-90, 1, 0, 0);
	//gluSphere(quadric, 150, 400, 400);
	gluSphere(quadric, 300 + ofGetFrameNum() / 200, 400, 400); //300 taille sphere
	shader.end();
	fbo.end();

	ofPopMatrix();


	spout.sendTexture(fbo.getTexture(), "vxSphere");
	fbo.draw(0, 0, targetWidth, targetHeight);
	//image.draw(200, 100);	
	//ofSetColor(0);
	//ofDrawBitmapString("Tempo: " + ofToString(link.tempo()) + " Beats: " + ofToString(status.beat) + " Phase: " + ofToString(status.phase), 20, 20);
	//ofDrawBitmapString("Number of peers: " + ofToString(link.numPeers()), 20, 40);
}
void ofApp::loadImage() {
	bool bFileThere = false;
	string fileName = "p-(" + ofToString(imgIndex) + ").jpg";
	/*if (imgIndex < 10) {
		fileName = "p000" + ofToString(imgIndex) + ".jpg";
	}
	else {
		fileName = "p00" + ofToString(imgIndex) + ".jpg";
	} */

	fstream fin;
	string fileNameInOF = ofToDataPath(fileName); // since OF files are in the data directory, we need to do this  
	fin.open(fileNameInOF.c_str(), ios::in);
	if (fin.is_open()) {
		cout << " file exists" << endl;
		bFileThere = true;
	}
	fin.close();
	if (bFileThere) {
		colormap.loadImage(fileName);
		bumpmap.loadImage(fileName);
	}

}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == OF_KEY_RIGHT) {
		//link.setQuantum(link.quantum() + 1);
		imgIndex++;
		loadImage();
	}
	else if (key == OF_KEY_LEFT) {
		//link.setQuantum(link.quantum() - 1);
		imgIndex--;
		if (imgIndex < 1) imgIndex = 1;
		loadImage();
	}
	else if (key == OF_KEY_UP) {
		//link.setTempo(link.tempo() + 1);
		factor++;
	}
	else if (key == OF_KEY_DOWN) {
		//link.setTempo(link.tempo() - 1);
		factor--;
		if (factor < 1) factor = 1;
	}
	if (key == ' ') soundPlayer.play();
	//if (key == 'f')	 ofSetFullscreen(true);


}
void ofApp::exit() {
	spout.exit();
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

