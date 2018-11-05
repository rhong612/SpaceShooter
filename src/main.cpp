#include "ofMain.h"
#include "ofApp.h"

//  Raymond Hong - CS 134 SJSU - 11/4/2018
//  Entry-point of the application

//========================================================================
int main( ){
	ofSetupOpenGL(600,1024,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
