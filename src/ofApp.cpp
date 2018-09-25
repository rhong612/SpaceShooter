#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	sprite.image.loadImage("images/sprite.png");
	sprite.speed = 500;
	sprite.width = sprite.image.getWidth();
	sprite.height = sprite.image.getHeight();
	sprite.trans.x = ofGetWidth() / 2 - sprite.width / 2;
	sprite.trans.y = ofGetHeight() / 2 - sprite.height / 2;
	moveDir = MoveStop;
	idle = true;
	leftPressed = false;
	rightPressed = false;
	upPressed = false;
	downPressed = false;
	
	missileEmitter.sys = &missileSystem;
	missileEmitter.path = "images/missile.png";
	missileEmitter.velocity = ofVec2f(0, -500);
	missileEmitter.lifespan = 10000;

	panel.setup();
	panel.add(rateSlider.setup("rate", 70, 0, 500));
	panel.add(directionSlider.setup("direction", 180, 0, 360));
	missileEmitter.direction = directionSlider;
	missileEmitter.rate = rateSlider;

	missileEmitter.loadEmitSound("sfx/laser.wav");
}

//--------------------------------------------------------------
void ofApp::update(){
	missileEmitter.rate = rateSlider;
	missileEmitter.direction = directionSlider;
	updateSprite();
	missileSystem.update();
	missileEmitter.emit(sprite.trans.x + sprite.width / 2, sprite.trans.y);
}

void ofApp::updateSprite() {
	float dist = sprite.speed * 1 / ofGetFrameRate();
	ofVec3f dir;
	ofRectangle r = ofGetWindowRect();
	
	switch (moveDir)
	{
		case MoveUp:
			dir = ofVec3f(0, -dist, 0);
			break;
		case MoveDown:
			dir = ofVec3f(0, dist, 0);
			break;
		case MoveLeft:
			dir = ofVec3f(-dist, 0, 0);
			break;
		case MoveRight:
			dir = ofVec3f(dist, 0, 0);
			break;
	}

	sprite.trans += dir;
}

void ofApp::moveSprite(MoveDir dir) {
	moveDir = dir;
}

void ofApp::stopSprite() {
	moveDir = MoveStop;
}

//--------------------------------------------------------------
void ofApp::draw(){
	if (idle) {
		ofDrawBitmapString("Press spacebar to start!", ofGetWidth() / 2, ofGetHeight() / 2);
	}
	else {
		panel.draw();
		checkTurretBounds();
		sprite.draw();
		missileSystem.draw();
	}
}

void ofApp::checkTurretBounds() {
	if (sprite.trans.x < 0) {
		sprite.trans.x = 0;
	}
	else if (sprite.trans.x > ofGetWidth() - sprite.image.getWidth()) {
		sprite.trans.x = ofGetWidth() - sprite.image.getWidth();
	}
	if (sprite.trans.y < 0) {
		sprite.trans.y = 0;
	}
	else if (sprite.trans.y > ofGetHeight() - sprite.image.getHeight()) {
		sprite.trans.y = ofGetHeight() - sprite.image.getHeight();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (!idle) {
		switch (key) {
			case ' ':
				missileEmitter.start();
				break;
			case OF_KEY_RIGHT:
				rightPressed = true;
				moveSprite(MoveRight);
				break;
			case OF_KEY_LEFT:
				leftPressed = true;
				moveSprite(MoveLeft);
				break;
			case OF_KEY_UP:
				upPressed = true;
				moveSprite(MoveUp);
				break;
			case OF_KEY_DOWN:
				downPressed = true;
				moveSprite(MoveDown);
				break;
		}
	}
	else if (key == ' ') {
		idle = false;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch (key) {
		case ' ':
			missileEmitter.stop();
			break;
		case OF_KEY_LEFT:
			leftPressed = false;
			if (!leftPressed && !rightPressed && !upPressed && !downPressed) {
				stopSprite();
			}
			break;
		case OF_KEY_RIGHT:
			rightPressed = false;
			if (!leftPressed && !rightPressed && !upPressed && !downPressed) {
				stopSprite();
			}
			break;
		case OF_KEY_UP:
			upPressed = false;
			if (!leftPressed && !rightPressed && !upPressed && !downPressed) {
				stopSprite();
			}
			break;
		case OF_KEY_DOWN:
			downPressed = false;
			if (!leftPressed && !rightPressed && !upPressed && !downPressed) {
				stopSprite();
			}
			break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (!idle) {
		ofPoint mouseCurrent = ofPoint(x, y);
		ofVec3f delta = mouseCurrent - mouseLast;
		sprite.trans += delta;
		mouseLast = mouseCurrent;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (!idle) {
		mouseLast = ofPoint(x, y);
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}


void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
