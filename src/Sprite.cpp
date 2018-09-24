#include "ofApp.h"

Sprite::Sprite() {
	trans.x = 0;
	trans.y = 0;
	scale.x = 1.0;
	scale.y = 1.0;
	rot = 0;
	speed = 0;
	bSelected = false;
}

void Sprite::draw() {
	image.draw(trans.x, trans.y);
}

float Sprite::age() {
	return ofGetElapsedTimeMillis() - birthtime;
}