#include "ofApp.h"
#include "stdlib.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);

	arialFont.load("fonts/arial.ttf", 32);
	turretSprite.image.loadImage("images/sprite.png");
	turretSprite.speed = 500;
	turretSprite.width = turretSprite.image.getWidth();
	turretSprite.height = turretSprite.image.getHeight();
	turretSprite.trans.x = ofGetWidth() / 2 - turretSprite.width / 2;
	turretSprite.trans.y = ofGetHeight() / 2 - turretSprite.height / 2;
	turretSprite.damage = INT_MAX;
	turretSprite.health = 100;
	moveDir = MoveStop;
	idle = true;
	leftPressed = false;
	rightPressed = false;
	upPressed = false;
	downPressed = false;
	
	missileEmitter.sys = &missileSystem;
	missileEmitter.loadSpriteImage("images/missile.png");
	missileEmitter.resizeImage(15, 15);
	missileEmitter.velocity = ofVec2f(0, -600);
	missileEmitter.lifespan = 3000;
	ofVec2f missileEmitterPosition;
	missileEmitterPosition.x = turretSprite.trans.x + turretSprite.width / 2;
	missileEmitterPosition.y = turretSprite.trans.y;
	missileEmitter.setPosition(missileEmitterPosition);

	panel.setup();
	panel.add(rateSlider.setup("rate", 970, 0, 1000));
	panel.add(directionSlider.setup("direction", 180, 0, 360));
	panel.add(enemyRateSlider.setup("enemy rate", 20, 0, 1000));
	panel.add(enemyLifespanSlider.setup("enemy lifespan", 7000, 0, 20000));
	panel.add(enemyVelocitySlider.setup("enemy velocity", ofVec3f(0, 200, 0), ofVec3f(0, 0, 0), ofVec3f(0, 2000, 0)));
	
	missileEmitter.direction = directionSlider;
	missileEmitter.rate = rateSlider;
	missileEmitter.sprite.health = 1;
	missileEmitter.sprite.damage = 10;

	missileEmitter.loadEmitSound("sfx/laser.wav");

	//Setup Alien Emitter
	alienEmitter = new Emitter();
	alienEmitter->sys = &alienEnemySystem;
	alienEmitter->loadSpriteImage("images/alien.png");
	alienEmitter->resizeImage(60, 60);
	alienEmitter->velocity = ofVec3f(0, 150, 0);
	alienEmitter->lifespan = 12000;
	alienEmitter->direction = 180;
	alienEmitter->rate = 800;
	ofVec2f* enemyEmitterPosition = new ofVec2f();
	enemyEmitterPosition->x = rand() % ofGetWidth();
	enemyEmitterPosition->y = 0;
	alienEmitter->setPosition(*enemyEmitterPosition);
	alienEmitter->sprite.damage = 10;
	alienEmitter->sprite.health = 10;

	//Setup Zombie Emitter
	zombieEmitter = new Emitter();
	zombieEmitter->sys = &zombieEnemySystem;
	zombieEmitter->loadSpriteImage("images/exploding_zombie_thing.png");
	zombieEmitter->resizeImage(50, 50);
	zombieEmitter->velocity = ofVec3f(0, INITIAL_ZOMBIE_Y_VELOCITY, 0);
	zombieEmitter->lifespan = 20000;
	zombieEmitter->direction = 180;
	zombieEmitter->rate = 700;
	ofVec2f* enemyEmitterPosition2 = new ofVec2f();
	enemyEmitterPosition2->x = rand() % ofGetWidth();
	enemyEmitterPosition2->y = 0;
	zombieEmitter->setPosition(*enemyEmitterPosition2);
	zombieEmitter->sprite.damage = 20;
	zombieEmitter->sprite.health = 20;



	enemyEmitters.push_back(alienEmitter);
	enemyEmitters.push_back(zombieEmitter);


	score = 0;
	level = 1;
	currentAlienCurveIntensity = INITIAL_ALIEN_CURVE_INTENSITY;
}

//--------------------------------------------------------------
void ofApp::update(){
	for (auto it = begin(particleEmitters); it != end(particleEmitters); it++) {
		(*it)->update();
	}

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

	curveVelocity(&alienEnemySystem, currentAlienCurveIntensity);
	alienEnemySystem.update();
	zombieEnemySystem.update();
	for (auto it = begin(enemyEmitters); it != end(enemyEmitters); it++) {
		/*
		(*it)->rate = enemyRateSlider;
		(*it)->velocity = toGlm(enemyVelocitySlider);
		(*it)->lifespan = enemyLifespanSlider;
		*/
		(*it)->trans.x = rand() % ofGetWidth(); //Randomize emitter position
		(*it)->emit();
	}

	checkCollisions();
	checkLevel();
	scaleEnemies();
}

void ofApp::checkLevel() {
	if (level == 3) {
		level = score / 30 + 3; //Increase level every 30 points after level 3
	}
	else if (level == 2 && score >= LEVEL_THREE_REQUIREMENT) {
		level = 3;
	}
	else if (level == 1 && score >= LEVEL_TWO_REQUIREMENT) {
		level = 2;
		zombieEmitter->start();
	}
}

void ofApp::scaleEnemies() {
	alienEmitter->rate = level * 10 + 800;
	zombieEmitter->rate = level * 5 + 700;

	currentAlienCurveIntensity = level * INITIAL_ALIEN_CURVE_INTENSITY;
	zombieEmitter->velocity = ofVec3f(0, level * 5.0 + INITIAL_ZOMBIE_Y_VELOCITY, 0);
}

void ofApp::curveVelocity(SpriteSystem *sys, float curveIntensity) {
	ofVec2f newEnemyVelocity;
	for (auto it = begin(alienEnemySystem.sprites); it != end(alienEnemySystem.sprites); it++) {
		newEnemyVelocity.x = -cos(it->trans.y / 100) * curveIntensity;
		newEnemyVelocity.y = it->velocity.y;
		it->velocity = newEnemyVelocity;
	}
}

void ofApp::checkCollisions() {
	//Alien collisions
	for (vector<Sprite>::iterator it = alienEnemySystem.sprites.begin(); it != alienEnemySystem.sprites.end(); it++) {
		//Contact with player missile
		for (vector<Sprite>::iterator missileIter = missileSystem.sprites.begin(); missileIter != missileSystem.sprites.end(); missileIter++) {			
			float hDistance = abs( (it->trans.x + it->width / 2) - (missileIter->trans.x + missileIter->width / 2) );
			float vDistance = abs( (it->trans.y + it->height / 2) - (missileIter->trans.y + missileIter->height / 2) );
			float hContactDistance = it->width / 2 + missileIter->width / 2;
			float vContactDistance = it->height / 2 + missileIter->height / 2;
			if (hDistance <= hContactDistance && vDistance <= vContactDistance) {
				bool alienDied = collide(&*it, &*missileIter);
				if (alienDied) {
					score++;
				}
			}
		}
		//Contact with turret
		float hDistance = abs((it->trans.x + it->width / 2) - (turretSprite.trans.x + turretSprite.width / 2));
		float vDistance = abs((it->trans.y + it->height / 2) - (turretSprite.trans.y + turretSprite.height / 2));
		float hContactDistance = it->width / 2 + turretSprite.width / 2;
		float vContactDistance = it->height / 2 + turretSprite.height / 2;
		if (hDistance <= hContactDistance && vDistance <= vContactDistance) {
			collide(&turretSprite, &*it);
		}
	}
	//Zombie Collisions
	for (vector<Sprite>::iterator it = zombieEnemySystem.sprites.begin(); it != zombieEnemySystem.sprites.end(); it++) {
		//Contact with player missile
		for (vector<Sprite>::iterator missileIter = missileSystem.sprites.begin(); missileIter != missileSystem.sprites.end(); missileIter++) {
			float hDistance = abs((it->trans.x + it->width / 2) - (missileIter->trans.x + missileIter->width / 2));
			float vDistance = abs((it->trans.y + it->height / 2) - (missileIter->trans.y + missileIter->height / 2));
			float hContactDistance = it->width / 2 + missileIter->width / 2;
			float vContactDistance = it->height / 2 + missileIter->height / 2;
			if (hDistance <= hContactDistance && vDistance <= vContactDistance) {
				bool zombieDied = collide(&*it, &*missileIter);

				if (zombieDied) {
					score += 5;
					//Create explosion effect
					ParticleEmitter* particleEmitter = new ParticleEmitter();
					particleEmitter->setLifespan(3);
					particleEmitter->setParticleRadius(7.0);
					particleEmitter->setVelocity(ofVec3f(0, 0, 0));
					particleEmitter->sys->addForce(new ImpulseRadialForce(40000.0));
					particleEmitter->position = it->trans;
					particleEmitter->groupSize = 10;
					particleEmitter->oneShot = true;
					particleEmitter->start();
					particleEmitters.push_back(particleEmitter);
				}
			}
		}
		//Contact with turret
		float hDistance = abs((it->trans.x + it->width / 2) - (turretSprite.trans.x + turretSprite.width / 2));
		float vDistance = abs((it->trans.y + it->height / 2) - (turretSprite.trans.y + turretSprite.height / 2));
		float hContactDistance = it->width / 2 + turretSprite.width / 2;
		float vContactDistance = it->height / 2 + turretSprite.height / 2;
		if (hDistance <= hContactDistance && vDistance <= vContactDistance) {
			collide(&turretSprite, &*it);
		}
	}


	for (auto it = begin(particleEmitters); it != end(particleEmitters); it++) {
		for (auto part = begin((*it)->sys->particles); part != end((*it)->sys->particles); part++) {
			//Enemy explosion particle contact with player turret
			float hDistance = abs((part->position.x + part->radius) - (turretSprite.trans.x + turretSprite.width / 2));
			float vDistance = abs((part->position.y + part->radius) - (turretSprite.trans.y + turretSprite.height / 2));
			float hContactDistance = part->radius + turretSprite.width / 2;
			float vContactDistance = part->radius / 2 + turretSprite.height / 2;
			if (hDistance <= hContactDistance && vDistance <= vContactDistance) {
				collide(&turretSprite, &*part);
			}
		}
	}
}


bool ofApp::collide(Sprite* main, Sprite* other) {
	main->health -= other->damage;
	other->health -= main->damage;

	if (main->health <= 0) {
		main->lifespan = 0;
	}
	if (other->health <= 0) {
		other->lifespan = 0;
	}
	
	return main->health <= 0;
}


bool ofApp::collide(Sprite* main, Particle* other) {
	main->health -= other->damage;

	if (main->health <= 0) {
		main->lifespan = 0;
	}
	other->lifespan = 0;

	return main->health <= 0;
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
		arialFont.drawString("Press spacebar to start!", ofGetWidth() / 2 - 200, ofGetHeight() / 2);
	}
	else {
		panel.draw();
		ofSetColor(255, 255, 255); //When a particle is drawn, it calls ofSetColor(red);
		turretSprite.draw();
		missileSystem.draw();
		alienEnemySystem.draw();
		zombieEnemySystem.draw();
		for (auto it = begin(particleEmitters); it != end(particleEmitters); it++) {
			(*it)->draw();
		}
		ofSetColor(255, 255, 255); //ParticleEmitter draw calls ofSetColor(red). Need to reverse the effect.
		arialFont.drawString("Score:" + to_string(score), ofGetWidth() / 5, ofGetHeight() / 10);
		arialFont.drawString("Level:" + to_string(level), ofGetWidth() * 3/4, ofGetHeight() / 10);
		arialFont.drawString("Health:" + to_string(turretSprite.health), ofGetWidth() / 2, ofGetHeight() - 50);
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
				alienEmitter->start();
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
