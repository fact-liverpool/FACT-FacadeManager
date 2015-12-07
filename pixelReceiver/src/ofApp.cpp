#include "ofApp.h"

#define RECONNECT_TIME 400

//--------------------------------------------------------------
void ofApp::setup(){
	// we don't want to be running to fast
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	int secondsSleep = 300;
	myScreen.setupReceive();
	//ofHideCursor();
	//Liverpool - latitude and longitude and timezone
	mySunrise.setData( 53.4076, -2.9809, 1);

	//GUI
	gui.setBackgroundColor(30, 30, 60, 200);
	gui.setTextColor(240, 50, 50, 255);

	gui.loadFont("arial.ttf", 10);
	gui.setup("FACT facade control - Code by Mar Canet and Varvara Guljajeva 2011", 10, 10, 800, 800);
	gui.addPanel("General control", 4, false);

	gui.setBackgroundColor(30, 30, 30, 200);

	//--------- PANEL 1
	gui.setWhichPanel(0);

	gui.setWhichColumn(0);
	gui.addSlider("timeSwitchOff hour", 0, 0, 24);
	gui.addSlider("timeSwitchOff minute", 0, 0, 60);
	vector <string> timeSwitchOnType;
	timeSwitchOnType.push_back("Automatic on sunset");
	timeSwitchOnType.push_back("Manual setup");
	timeSwitchOnType.push_back("Allways on");
	gui.addMultiToggle("Choose Switch On type",0, timeSwitchOnType);
	gui.addSlider("timeSwitchOn hour", 0, 0, 24);
	gui.addSlider("timeSwitchOn minute",  0, 0, 60);

	gui.setWhichColumn(1);
	listerImg.listDir("data/images/");
	gui.addFileLister("Image lister", &listerImg, 200, 300);

	listerMov.listDir("data/movies/");
	gui.addFileLister("Movies lister", &listerMov, 200, 300);

	gui.setWhichColumn(2);
	vector <string> contentType;
	contentType.push_back("Video");
	contentType.push_back("Image");
	gui.addMultiToggle("Choose content type to show",0, contentType);
	gui.addLabel("Press ESC to close");
	gui.addLabel("Press SPACE to open/close menu");
	//SETTINGS AND EVENTS

	//load from xml!
	gui.loadSettings("controlPanelSettings.xml");

	//if you want to use events call this after you have added all your gui elements
	gui.setupEvents();
	gui.enableEvents();
	activeGUI = true;
	movieSelectedPath = "";
	imageSelectedPath = "";

	widthTile = ofGetScreenWidth()/50;//1600/50;
	heightTile = ofGetScreenHeight()/50;//900/50;

	myVideoFrame.allocate(50,50,OF_IMAGE_COLOR);
	myVideo = NULL;

	myVideo = new ofVideoPlayer[listerMov.getNumEntries()];
	for(int i=0;i<listerMov.getNumEntries();i++){
		moviesMap[listerMov.getPath(i)] = i;
		myVideo[i].load(listerMov.getPath(i));
        cout << listerMov.getPath(i) << endl;
	}
	myScreen.setIpServer("192.168.0.200");
}

//--------------------------------------------------------------
void ofApp::update(){
	//myScreen.receive();
	gui.update();

	if(movieSelectedPath != listerMov.getSelectedPath())
	{
		movieSelectedPath = listerMov.getSelectedPath();
		cout << "Movies[" <<movieSelectedPath<<"]" << endl;
		idMovieSelected = moviesMap[listerMov.getSelectedPath()];
		myVideo[idMovieSelected].play();

	}
	if(imageSelectedPath!= listerImg.getSelectedPath())
	{
		imageSelectedPath = listerImg.getSelectedPath();
		myImage.load(imageSelectedPath);
	}

	if(gui.getValueI("CONTENT_TYPE")==0){
		myVideo[idMovieSelected].update();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofBackground(0, 0, 0);
	ofFill();
	
	if(true)
	{
		if( myScreen.isReceivingPixels() ){
			renderPixelsReceive();
		}else if(gui.getValueI("CONTENT_TYPE")==0){
			renderVideo();
		}else if(gui.getValueI("CONTENT_TYPE")==1){
			renderImage();
		}
	}
	
	if(activeGUI)
	{
		ofShowCursor();
		gui.draw();
    }else{
        ofHideCursor();
    }
}
//--------------------------------------------------------------
bool ofApp::isTimeToOpenScreen()
{
	float currentHour = ofGetHours() + (ofGetMinutes()/60.0);
	float timeOpen;
	int switchMode = gui.getValueI("TIME_SWITCHON_TYPE");
	if(switchMode==1){
		timeOpen = gui.getValueF("TIME_SWITCHON_HOUR") + (gui.getValueF("TIME_SWITCHON_MINUTE")/60.0);
	}else if(switchMode==0){
		timeOpen = mySunrise.getSunset()-0.5; // open half an hour before sunset
	}else if(switchMode==2){
		timeOpen = true;
	}
	float timeClose = gui.getValueF("TIME_SWITCHOFF_HOUR")+(gui.getValueF("TIME_SWITCHOFF_MINUTE")/60.0);
	return currentHour>timeOpen && currentHour<timeClose;
}
//--------------------------------------------------------------
void ofApp::renderPixelsReceive()
{
	ofPushMatrix();
	ofTranslate(0,0,0);
	for(int i=0;i<50;i++){
		for(int j=0;j<50;j++){
			int id = (i+j*50);
			ofSetColor( myScreen.getColor(id).r, myScreen.getColor(id).g, myScreen.getColor(id).b );
			ofDrawRectangle(i*widthTile,j*heightTile,widthTile,heightTile);
		}	
	}
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::renderVideo()
{
	ofPixels pixels = myVideo[idMovieSelected].getPixels();
	ofPushMatrix();
	ofTranslate(0,0,0);
	for(int i=0;i<50;i++){
		for(int j=0;j<50;j++){
			int id = (i+j*50)*3;
			ofSetColor( pixels[id], pixels[id+1], pixels[id+2] );
			ofDrawRectangle(i*widthTile,j*heightTile,widthTile,heightTile);
		}	
	}
	ofPopMatrix();
}
//--------------------------------------------------------------
void ofApp::renderImage()
{
	ofPixels pixels = myImage.getPixels();
	ofPushMatrix();
	ofTranslate(0,0,0);
	for(int i=0;i<50;i++){
		for(int j=0;j<50;j++){
			int id = (i+j*50)*3;
			ofSetColor( pixels[id], pixels[id+1], pixels[id+2] );
			ofDrawRectangle(i*widthTile,j*heightTile,widthTile,heightTile);
		}	
	}
	ofPopMatrix();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
	if( key==' '){
		activeGUI =!activeGUI;
		cout << "press" << endl;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	for(int i=0;i<50;i++){
		for(int j=0;j<50;j++){
			int id = (i+j*50);
			myScreen.setColor(id,255,0,0);
		}	
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
}
