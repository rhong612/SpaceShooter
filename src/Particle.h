#pragma once

#include "ofMain.h"


//  Raymond Hong - CS 134 SJSU - 11/4/2018
//  Header file for Particle

class ParticleForceField;

class Particle {
public:
	Particle();

	ofVec3f position;
	ofVec3f velocity;
	ofVec3f acceleration;
	ofVec3f forces;
	float	damping;
	float   mass;
	float   lifespan;
	float   radius;
	float   birthtime;
	void    integrate();
	void    draw();
	float   age();        // sec
	ofColor color;
	float damage;
};


