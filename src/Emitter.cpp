#define _USE_MATH_DEFINES
#include "ofApp.h"
#include <cmath>

//  Raymond Hong - CS 134 SJSU - 11/4/2018
//  Implementation for an Emitter class.

Emitter::Emitter() {
	started = false;
	rateCounter = 0;
}

//Starts the Emitter
void Emitter::start() {
	started = true;
}

//Stops the Emitter
void Emitter::stop() {
	started = false;
}

//Loads a sound that the Emitter may play upon calling emit()
void Emitter::loadEmitSound(string path) {
	soundPlayer.load(path);
}

//Loads the image for the Emitter to emit
void Emitter::loadSpriteImage(string path) {
	sprite.image.loadImage(path);
}

//Resizes the image for the Emitter
void Emitter::resizeImage(float width, float height) {
	sprite.image.resize(width, height);
}

//Emits a sprite
void Emitter::emit() {
	rateCounter++;
	if (started && rateCounter > MAX_RATE - rate) {
		sprite.width = sprite.image.getWidth();
		sprite.height = sprite.image.getHeight();
		sprite.trans.x = trans.x - sprite.width / 2;
		sprite.trans.y = trans.y;
		sprite.velocity = velocity;
		rotate(&sprite.velocity);
		sprite.birthtime = ofGetElapsedTimeMillis();
		sprite.lifespan = lifespan;
		sys->add(sprite);
		if (soundPlayer.isLoaded()) {
			soundPlayer.play();
		}
		rateCounter = 0;
	}
}

//Rotates the velocity of the sprite to be emitted based on the Emitter's direction field
void Emitter::rotate(ofVec2f * vector) {
	float radDirection = direction * M_PI / 180.0; //Convert to radians
	float newX = vector->x * cos(radDirection) - vector->y * sin(radDirection);
	float newY = vector->x * sin(radDirection) - vector->y * cos(radDirection);
	vector->x = newX;
	vector->y = newY;
}
