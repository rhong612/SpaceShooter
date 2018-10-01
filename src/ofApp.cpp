#include "ofApp.h"
#include "stdlib.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	turretSprite.image.loadImage("images/sprite.png");
	turretSprite.speed = 600;
	turretSprite.width = turretSprite.image.getWidth();
	turretSprite.height = turretSprite.image.getHeight();
	turretSprite.trans.x = ofGetWidth() / 2 - turretSprite.width / 2;
	turretSprite.trans.y = ofGetHeight() / 2 - turretSprite.height / 2;
	moveDir = MoveStop;
	idle = true;
	leftPressed = false;
	rightPressed = false;
	upPressed = false;
	downPressed = false;
	
	missileEmitter.sys = &missileSystem;
	missileEmitter.loadSpriteImage("images/missile.png");
	missileEmitter.velocity = ofVec2f(0, -500);
	missileEmitter.lifespan = 3000;
	ofVec2f missileEmitterPosition;
	missileEmitterPosition.x = turretSprite.trans.x + turretSprite.width / 2;
	missileEmitterPosition.y = turretSprite.trans.y;
	missileEmitter.setPosition(missileEmitterPosition);

	for (int i = 0; i < 3; i++) {
		Emitter* enemyEmitter = new Emitter();
		enemyEmitter->sys = &enemySystem;
		enemyEmitter->loadSpriteImage("images/enemy.png");
		enemyEmitter->velocity = ofVec2f(0, 500);
		enemyEmitter->lifespan = 3000;
		enemyEmitter->direction = 180;
		enemyEmitter->rate = 10.0;
		ofVec2f* enemyEmitterPosition = new ofVec2f();
		enemyEmitterPosition->x = ofGetWidth() * ((rand() % 100) / 100.0);
		enemyEmitterPosition->y = 0;
		enemyEmitter->setPosition(*enemyEmitterPosition);
		enemyEmitters.push_back(*enemyEmitter);
	}

	panel.setup();
	panel.add(rateSlider.setup("rate", 10, 0, 70));
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
	checkTurretBounds();
	missileSystem.update();
	ofVec2f missileEmitterPosition;
	missileEmitterPosition.x = turretSprite.trans.x + turretSprite.width / 2;
	missileEmitterPosition.y = turretSprite.trans.y;
	missileEmitter.setPosition(missileEmitterPosition);
	missileEmitter.emit();


	enemySystem.update();
	for (auto it = begin(enemyEmitters); it != end(enemyEmitters); it++) {
		it->emit();
	}

	checkCollisions();
}

void ofApp::checkCollisions() {
	for (vector<Sprite>::iterator it = enemySystem.sprites.begin(); it != enemySystem.sprites.end(); it++) {
		for (vector<Sprite>::iterator missileIter = missileSystem.sprites.begin(); missileIter != missileSystem.sprites.end(); missileIter++) {
			float hDistance = abs(it->trans.x - missileIter->trans.x);
			float vDistance = abs(it->trans.y - missileIter->trans.y);
			float hContactDistance = it->width / 2 + missileIter->width / 2;
			float vContactDistance = it->height / 2 + missileIter->height / 2;
			if (hDistance < hContactDistance && vDistance < vContactDistance) {
				it->lifespan = 0;
				missileIter->lifespan = 0;
			}
		}
	}
}

void ofApp::updateSprite() {
	float dist = turretSprite.speed * 1 / ofGetFrameRate();
	ofVec3f dir;

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

	turretSprite.trans += dir;
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
		turretSprite.draw();
		missileSystem.draw();
		enemySystem.draw();
	}
}

void ofApp::checkTurretBounds() {
	if (turretSprite.trans.x < 0) {
		turretSprite.trans.x = 0;
	}
	else if (turretSprite.trans.x > ofGetWidth() - turretSprite.image.getWidth()) {
		turretSprite.trans.x = ofGetWidth() - turretSprite.image.getWidth();
	}
	if (turretSprite.trans.y < 0) {
		turretSprite.trans.y = 0;
	}
	else if (turretSprite.trans.y > ofGetHeight() - turretSprite.image.getHeight()) {
		turretSprite.trans.y = ofGetHeight() - turretSprite.image.getHeight();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (!idle) {
		switch (key) {
			case ' ':
				missileEmitter.start();
				for (auto it = begin(enemyEmitters); it != end(enemyEmitters); it++) {
					it->start();
				}
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
		turretSprite.trans += delta;
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
