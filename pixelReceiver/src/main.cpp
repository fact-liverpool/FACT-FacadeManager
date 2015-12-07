#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"
#include <assert.h>

int main(){
    ofAppGlutWindow window;
    window.setWindowTitle("factScreen");

    ofSetupOpenGL(&window, 1600,900, OF_WINDOW);			// <-------- setup the GL context

    ofRunApp(new ofApp());
}
