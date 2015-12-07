#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxNetwork.h"
#include "ofxControlPanel.h"
#include "ofxFACTLiverpoolScreen.h"
#include "ofxSunrise.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		bool isTimeToOpenScreen();
		void renderPixelsReceive();
		void renderVideo();
		void renderImage();

		ofxFACTLiverpoolScreen	myScreen;
		ofxSunrise				mySunrise;
		ofxControlPanel			gui;
		bool					activeGUI;
		simpleFileLister		listerImg;
		simpleFileLister		listerMov;
		ofVideoPlayer*			myVideo;
		ofImage					myVideoFrame;
		ofImage					myImage;
		string					movieSelectedPath;
		string					imageSelectedPath;

		int widthTile;
	    int heightTile;

		map<string,int>			moviesMap;
		int						idMovieSelected;
};

#endif

