#include "ofApp.h"


//  Raymond Hong - CS 134 SJSU - 11/4/2018
//  Implementation for a Sprite class.

Sprite::Sprite() {
	trans.x = 0;
	trans.y = 0;
	scale.x = 1.0;
	scale.y = 1.0;
	rot = 0;
	speed = 0;
	bSelected = false;
	health = 100;
	damage = 100; //Damage if Sprite touches another sprite/particle
}

//Draws the sprite
void Sprite::draw() {
	image.draw(trans.x, trans.y);
}

//Calculates the age of the sprite
float Sprite::age() {
	return ofGetElapsedTimeMillis() - birthtime;
}