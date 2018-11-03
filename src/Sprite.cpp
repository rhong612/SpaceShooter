#include "ofApp.h"

Sprite::Sprite() {
	trans.x = 0;
	trans.y = 0;
	scale.x = 1.0;
	scale.y = 1.0;
	rot = 0;
	speed = 0;
	bSelected = false;
	health = 100;
	damage = 100;
}

void Sprite::draw() {
	image.draw(trans.x, trans.y);
}

float Sprite::age() {
	return ofGetElapsedTimeMillis() - birthtime;
}

void Sprite::attack(Sprite* other) {
	health -= other->damage;
	other->health -= damage;

	if (health <= 0) {
		lifespan = 0;
	}
	if (other->health <= 0) {
		other->lifespan = 0;
	}
}