#include "ofApp.h"



AlienEmitter::AlienEmitter(SpriteSystem* enemySystem, float initialX, float initialY) {
	loadSpriteImage("images/enemy.png");
	sys = enemySystem;
	loadSpriteImage("images/enemy.png");
	resizeImage(80, 80);
	/* Slider values
	velocity = toGlm(enemyVelocitySlider);
	lifespan = enemyLifespanSlider;
	direction = 180;
	rate = enemyRateSlider;
	*/
	velocity = ofVec3f(0, 200, 0);
	lifespan = 7000;
	direction = 180;
	rate = 20;
	setPosition(ofVec2f(initialX, initialY));
}
