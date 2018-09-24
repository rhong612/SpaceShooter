#define _USE_MATH_DEFINES
#include "ofApp.h"
#include <cmath>

Emitter::Emitter() {
	started = false;
}

void Emitter::start() {
	started = true;
}

void Emitter::stop() {
	started = false;
}

void Emitter::loadEmitSound(string path) {
	soundPlayer.load(path);
}

void Emitter::emit(float x, float y) {
	if (started && ofGetElapsedTimeMillis() - lastSpawned >= rate) {
		Sprite sprite;
		sprite.image.loadImage(path);
		sprite.width = sprite.image.getWidth();
		sprite.height = sprite.image.getHeight();
		sprite.trans.x = x - sprite.width / 2;
		sprite.trans.y = y;
		sprite.velocity = velocity;
		rotate(&sprite.velocity);
		sprite.birthtime = ofGetElapsedTimeMillis();
		sprite.lifespan = lifespan;
		sys->add(sprite);
		soundPlayer.play();
		lastSpawned = ofGetElapsedTimeMillis();
	}
}


void Emitter::rotate(ofVec2f * vector) {
	float radDirection = direction * M_PI / 180.0; //Convert to radians
	float newX = vector->x * cos(radDirection) - vector->y * sin(radDirection);
	float newY = vector->x * sin(radDirection) - vector->y * cos(radDirection);
	vector->x = newX;
	vector->y = newY;
}
