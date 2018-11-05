#include "ofApp.h"
#include "stdlib.h"

//  Raymond Hong - CS 134 SJSU - 11/4/2018
//  Main file that does all the heavy-lifting. Sets up all game components and takes care of game loop in update()
//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);

	//Load some assets
	destroySoundPlayer.load("sfx/destroy.wav");
	powerUpSoundPlayer.load("sfx/power_up.wav");
	damageSoundPlayer.load("sfx/damage.wav");
	arialFont.load("fonts/arial.ttf", 32);

	//Setup turret
	turretSprite.image.loadImage("images/sprite.png");
	turretSprite.speed = 500;
	turretSprite.image.resize(60, 60);
	turretSprite.width = turretSprite.image.getWidth();
	turretSprite.height = turretSprite.image.getHeight();
	turretSprite.trans.x = ofGetWidth() / 2 - turretSprite.width / 2;
	turretSprite.trans.y = ofGetHeight() / 2 - turretSprite.height / 2;
	turretSprite.damage = INT_MAX;
	turretSprite.health = TURRET_MAX_HEALTH;

	//Health bar
	healthBar.loadImage("images/healthBar.png");
	healthBar.resize(ofGetWidth(), healthBar.getHeight());

	//Initialize variables
	moveDir = MoveStop;
	idle = true;
	leftPressed = false;
	rightPressed = false;
	upPressed = false;
	downPressed = false;
	isGameOver = false;
	slidersActive = false;
	score = 0;
	level = 1;
	lastRotated = ofGetElapsedTimeMillis();
	currentAlienCurveIntensity = INITIAL_ALIEN_CURVE_INTENSITY;
	weaponLevel = 1;

	initMissileEmitters();

	setupSliderGui();
	initPowerUpEmitters();
	initEnemyEmitters();
}

// Initializes all of the enemy emitters and puts them into the enemyEmitters vector
//
void ofApp::initEnemyEmitters() {
	//Setup Alien Emitter
	alienEmitter = new Emitter();
	alienEmitter->sys = &alienEnemySystem;
	alienEmitter->loadSpriteImage("images/alien.png");
	alienEmitter->resizeImage(50, 50);
	alienEmitter->velocity = ofVec3f(0, 150, 0);
	alienEmitter->lifespan = 12000;
	alienEmitter->direction = 180;
	alienEmitter->rate = 900;
	alienEmitter->setPosition(ofVec2f(rand() % ofGetWidth(), 0));
	alienEmitter->sprite.damage = 20;
	alienEmitter->sprite.health = 10;

	//Setup Zombie Emitter
	zombieEmitter = new Emitter();
	zombieEmitter->sys = &zombieEnemySystem;
	zombieEmitter->loadSpriteImage("images/exploding_zombie_thing.png");
	zombieEmitter->resizeImage(45, 45);
	zombieEmitter->velocity = ofVec3f(0, INITIAL_ZOMBIE_Y_VELOCITY, 0);
	zombieEmitter->lifespan = 20000;
	zombieEmitter->direction = 180;
	zombieEmitter->rate = 600;
	zombieEmitter->setPosition(ofVec2f(rand() % ofGetWidth(), 0));
	zombieEmitter->sprite.damage = 20;
	zombieEmitter->sprite.health = 20;

	//Setup Blue Zombie Emitter;
	blueZombieEmitter = new Emitter();
	blueZombieEmitter->sys = &blueZombieEnemySystem;
	blueZombieEmitter->loadSpriteImage("images/random_zombie_thing.png");
	blueZombieEmitter->resizeImage(30, 30);
	blueZombieEmitter->velocity = ofVec3f(0, INITIAL_BLUE_ZOMBIE_Y_VELOCITY, 0);
	blueZombieEmitter->lifespan = 50000;
	blueZombieEmitter->direction = 180;
	blueZombieEmitter->rate = 700;
	blueZombieEmitter->setPosition(ofVec2f(rand() % ofGetWidth(), 0));
	blueZombieEmitter->sprite.damage = 30;
	blueZombieEmitter->sprite.health = 30;

	enemyEmitters.push_back(alienEmitter);
	enemyEmitters.push_back(zombieEmitter);
	enemyEmitters.push_back(blueZombieEmitter);

}

// Sets up slider GUI for testing values. Can be toggled on-off with the 'Ctrl' key
//
void ofApp::setupSliderGui() {
	panel.setup();
	panel.add(rateSlider.setup("Missile Rate", 970, 0, 1000));
	panel.add(directionSlider.setup("Missile Direction", 180, 0, 360));
	panel.add(enemyRateSlider.setup("Enemy Rate", 990, 0, 1000));
	panel.add(enemyLifespanSlider.setup("Enemy Lifespan", 7000, 0, 20000));
	panel.add(enemyVelocitySlider.setup("Enemy Velocity", ofVec3f(0, 200, 0), ofVec3f(0, 0, 0), ofVec3f(0, 2000, 0)));
}

// Initializes all of missile emitters and puts them in the missileEmitters vector
//
void ofApp::initMissileEmitters() {
	int i = 0;
	float directions[5] = { 180, 170, 190, 160, 200 };
	ofVec2f missileEmitterPosition;
	missileEmitterPosition.x = turretSprite.trans.x + turretSprite.width / 2;
	missileEmitterPosition.y = turretSprite.trans.y;


	missileEmitters.push_back(new Emitter());
	missileEmitters.push_back(new Emitter());
	missileEmitters.push_back(new Emitter());
	missileEmitters.push_back(new Emitter());
	missileEmitters.push_back(new Emitter());

	for (auto it = begin(missileEmitters); it != end(missileEmitters); it++) {
		(*it)->sys = &missileSystem;
		(*it)->loadSpriteImage("images/missile.png");
		(*it)->resizeImage(15, 15);
		(*it)->velocity = ofVec2f(0, -600);
		(*it)->lifespan = 3000;
		(*it)->setPosition(missileEmitterPosition);
		(*it)->direction = directions[i];
		(*it)->rate = 970;
		(*it)->sprite.health = 1;
		(*it)->sprite.damage = 10;
		if (i == 0) {
			(*it)->loadEmitSound("sfx/laser.wav");
		}
		i++;
	}
}

// Initializes all of the power-up emitters
//
void ofApp::initPowerUpEmitters() {
	rateUpEmitter.sys = &rateUpSystem;
	rateUpEmitter.loadSpriteImage("images/rateUpPowerUp.png");
	rateUpEmitter.resizeImage(25, 25);
	rateUpEmitter.velocity = ofVec3f(0, 200, 0);
	rateUpEmitter.lifespan = 20000;
	rateUpEmitter.direction = 180;
	rateUpEmitter.rate = Emitter::MAX_RATE; //Fire every time emit() is called
	rateUpEmitter.start();

	damageUpEmitter.sys = &damageUpSystem;
	damageUpEmitter.loadSpriteImage("images/damageUpPowerUp.png");
	damageUpEmitter.resizeImage(25, 25);
	damageUpEmitter.velocity = ofVec3f(0, 200, 0);
	damageUpEmitter.lifespan = 20000;
	damageUpEmitter.direction = 180;
	damageUpEmitter.rate = Emitter::MAX_RATE; //Fire every time emit() is called
	damageUpEmitter.start();

	weaponUpEmitter.sys = &weaponUpSystem;
	weaponUpEmitter.loadSpriteImage("images/weaponUpPowerUp.png");
	weaponUpEmitter.resizeImage(25, 25);
	weaponUpEmitter.velocity = ofVec3f(0, 200, 0);
	weaponUpEmitter.lifespan = 20000;
	weaponUpEmitter.direction = 180;
	weaponUpEmitter.rate = Emitter::MAX_RATE; //Fire every time emit() is called
	weaponUpEmitter.start();

	firstAidEmitter.sys = &firstAidSystem;
	firstAidEmitter.loadSpriteImage("images/firstAid.png");
	firstAidEmitter.resizeImage(25, 25);
	firstAidEmitter.velocity = ofVec3f(0, 200, 0);
	firstAidEmitter.lifespan = 20000;
	firstAidEmitter.direction = 180;
	firstAidEmitter.rate = Emitter::MAX_RATE; //Fire every time emit() is called
	firstAidEmitter.start();
}

//--------------------------------------------------------------
void ofApp::update(){
	checkGameOver();
	if (isGameOver) {
		return;
	}

	if (slidersActive) {
		for (auto it = begin(missileEmitters); it != end(missileEmitters); it++) {
			(*it)->rate = rateSlider;
			(*it)->direction = directionSlider;
		}
	}

	//Update turret and missiles
	updateSprite();
	checkTurretBounds();
	missileSystem.update();

	//Update power-up systems
	rateUpSystem.update();
	damageUpSystem.update();
	weaponUpSystem.update();
	firstAidSystem.update();

	//Custom movement for specific types of enemies
	curveVelocity(&alienEnemySystem, currentAlienCurveIntensity);
	randomizeMovement(&blueZombieEnemySystem);

	//Update enemy systems
	alienEnemySystem.update();
	zombieEnemySystem.update();
	blueZombieEnemySystem.update();

	//Update particle emitters
	for (auto it = begin(collisionfulEffectEmitters); it != end(collisionfulEffectEmitters); it++) {
		(*it)->update();
	}
	for (auto it = begin(collisionLessEffectEmitters); it != end(collisionLessEffectEmitters); it++) {
		(*it)->update();
	}

	//Emit missiles
	ofVec2f missileEmitterPosition;
	missileEmitterPosition.x = turretSprite.trans.x + turretSprite.width / 2;
	missileEmitterPosition.y = turretSprite.trans.y;
	for (auto it = begin(missileEmitters); it != end(missileEmitters); it++) {
		(*it)->setPosition(missileEmitterPosition);
		(*it)->emit();
	}

	//Emit enemies
	for (auto it = begin(enemyEmitters); it != end(enemyEmitters); it++) {
		if (slidersActive) {
			(*it)->rate = enemyRateSlider;
			(*it)->velocity = toGlm(enemyVelocitySlider);
			(*it)->lifespan = enemyLifespanSlider;
		}
		(*it)->trans.x = rand() % ofGetWidth(); //Randomize emitter position
		(*it)->emit();
	}

	checkCollisions();
	checkLevel();
	scaleEnemies();
	healthBar.resize(ofGetWidth() * ((float) turretSprite.health / (float) TURRET_MAX_HEALTH), healthBar.getHeight());
}


// Sets a boolean that determines if the player lost the game
//
void ofApp::checkGameOver() {
	if (turretSprite.health <= 0) {
		isGameOver = true;
	}
}

// Randomizes movement by randomly rotating the velocity of the sprites given. Y-value will always be positive to ensure constant downwards movement.
//
void ofApp::randomizeMovement(SpriteSystem* sys) {
	//Randomize every 2 seconds
	if (ofGetElapsedTimeMillis() - lastRotated >= 2000) {
		for (auto it = begin(sys->sprites); it != end(sys->sprites); it++) {
			float radDirection = (rand() % 360) * PI / 180.0; //Convert to radians
			float oldX = it->velocity.x;
			float oldY = it->velocity.y;
			float newX = oldX * cos(radDirection) - oldY * sin(radDirection);
			float newY = oldX * sin(radDirection) - oldY * cos(radDirection);
			if (newY <= 0) {
				newY *= -1.0; //Make sure the sprite is moving vertically downwards at all times.
			}
			it->velocity = ofVec2f(newX, newY);
		}
		lastRotated = ofGetElapsedTimeMillis();
	}
}

// Modifies the current level based on the player's score
//
void ofApp::checkLevel() {
	if (level >= 3) {
		level = (score / 30) + 3; //Increase level every 30 points after level 3
	}
	else if (level == 2 && score >= LEVEL_THREE_REQUIREMENT) {
		level = 3;
		blueZombieEmitter->start(); //Blue zombies start appearing at level 3
	}
	else if (level == 1 && score >= LEVEL_TWO_REQUIREMENT) {
		level = 2;
		zombieEmitter->start(); //Normal zombies start appearing at level 2
	}
}

// Scales enemies according to the current level
//
void ofApp::scaleEnemies() {
	//Increase enemy spawn rate based on level
	alienEmitter->rate = level * 5 + 900 > MAX_ALIEN_RATE ? MAX_ALIEN_RATE : level * 5 + 900;
	zombieEmitter->rate = level * 5 + 600 > MAX_ZOMBIE_RATE ? MAX_ZOMBIE_RATE : level * 5 + 600;
	blueZombieEmitter->rate = level * 5 + 500 > MAX_BLUE_ZOMBIE_RATE ? MAX_BLUE_ZOMBIE_RATE : level * 5 + 500;

	//Alien: Increase curve intensity
	//Zombie: Increase velocity
	//Blue zombie: Increase velocity
	currentAlienCurveIntensity = currentAlienCurveIntensity < MAX_ALIEN_CURVE_INTENSITY ? level * INITIAL_ALIEN_CURVE_INTENSITY : MAX_ALIEN_CURVE_INTENSITY;
	zombieEmitter->velocity = ofVec3f(0, level * 10.0 + INITIAL_ZOMBIE_Y_VELOCITY, 0);
	blueZombieEmitter->velocity = ofVec3f(0, level * 7.0 + INITIAL_BLUE_ZOMBIE_Y_VELOCITY, 0);
}

// Simulates a wave-like motion with the given sprites
void ofApp::curveVelocity(SpriteSystem *sys, float curveIntensity) {
	ofVec2f newEnemyVelocity;
	for (auto it = begin(sys->sprites); it != end(sys->sprites); it++) {
		newEnemyVelocity.x = -cos(it->trans.y / 100) * curveIntensity;
		newEnemyVelocity.y = it->velocity.y;
		it->velocity = newEnemyVelocity;
	}
}

// Checks for collisions and handles them accordingly.
//
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
					destroySoundPlayer.play();

					//Create explosion effect. No collision detection needed.
					ParticleEmitter* explosionEffectEmitter = new ParticleEmitter();
					explosionEffectEmitter->setLifespan(0.5);
					explosionEffectEmitter->setParticleRadius(1.0);
					explosionEffectEmitter->setVelocity(ofVec3f(0, 0, 0));
					explosionEffectEmitter->sys->addForce(new ImpulseRadialForce(3000.0));
					explosionEffectEmitter->position = ofVec3f(it->trans.x + it->width / 2.0, it->trans.y + it->height / 2.0, 0);
					explosionEffectEmitter->groupSize = 100;
					explosionEffectEmitter->oneShot = true;
					explosionEffectEmitter->color = ofColor::blue;
					explosionEffectEmitter->start();
					collisionLessEffectEmitters.push_back(explosionEffectEmitter);

					int randomNum = rand() % 100;
					if (randomNum <= DAMAGE_UP_CHANCE) {
						//Create damage-up power-up
						damageUpEmitter.setPosition(ofVec3f(it->trans.x, it->trans.y, 0));
						damageUpEmitter.emit();
					}
					else {
						randomNum = rand() % 100; //reroll for first aid item
						if (randomNum <= FIRST_AID_CHANCE) {
							firstAidEmitter.setPosition(ofVec3f(it->trans.x, it->trans.y, 0));
							firstAidEmitter.emit();
						}
					}

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
			damageSoundPlayer.play();
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
					destroySoundPlayer.play();
					score += 5;

					//Create death explosion effect
					ParticleEmitter* particleEmitter = new ParticleEmitter();
					particleEmitter->setLifespan(3);
					particleEmitter->setParticleRadius(5.0);
					particleEmitter->setVelocity(ofVec3f(0, 0, 0));
					particleEmitter->sys->addForce(new ImpulseRadialForce(30000.0));
					particleEmitter->position = ofVec3f(it->trans.x + it->width / 2.0, it->trans.y + it->height / 2.0, 0);
					particleEmitter->groupSize = 10;
					particleEmitter->oneShot = true;
					particleEmitter->start();
					collisionfulEffectEmitters.push_back(particleEmitter);



					int randomNum = rand() % 100;
					if (randomNum <= RATE_UP_CHANCE) {
						//Create rate-up power-up
						rateUpEmitter.setPosition(ofVec3f(it->trans.x, it->trans.y, 0));
						rateUpEmitter.emit();
					}
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
			damageSoundPlayer.play();
		}
	}

	//Blue Zombie Collisions
	for (vector<Sprite>::iterator it = blueZombieEnemySystem.sprites.begin(); it != blueZombieEnemySystem.sprites.end(); it++) {
		//Contact with player missile
		for (vector<Sprite>::iterator missileIter = missileSystem.sprites.begin(); missileIter != missileSystem.sprites.end(); missileIter++) {
			float hDistance = abs((it->trans.x + it->width / 2) - (missileIter->trans.x + missileIter->width / 2));
			float vDistance = abs((it->trans.y + it->height / 2) - (missileIter->trans.y + missileIter->height / 2));
			float hContactDistance = it->width / 2 + missileIter->width / 2;
			float vContactDistance = it->height / 2 + missileIter->height / 2;
			if (hDistance <= hContactDistance && vDistance <= vContactDistance) {
				bool zombieDied = collide(&*it, &*missileIter);
				if (zombieDied) {
					destroySoundPlayer.play();
					score += 20;
					//Create death explosion effect
					ParticleEmitter* particleEmitter = new ParticleEmitter();
					particleEmitter->setLifespan(3);
					particleEmitter->setParticleRadius(5.0);
					particleEmitter->setVelocity(ofVec3f(0, 0, 0));
					particleEmitter->sys->addForce(new ImpulseRadialForce(30000.0));
					particleEmitter->position = ofVec3f(it->trans.x + it->width / 2.0, it->trans.y + it->height / 2.0, 0);
					particleEmitter->groupSize = 30;
					particleEmitter->oneShot = true;
					particleEmitter->start();
					collisionfulEffectEmitters.push_back(particleEmitter);


					int randomNum = rand() % 100;
					if (randomNum <= WEAPON_UP_CHANCE) {
						//Create rate-up power-up
						weaponUpEmitter.setPosition(ofVec3f(it->trans.x, it->trans.y, 0));
						weaponUpEmitter.emit();
					}
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
			damageSoundPlayer.play();
		}
	}

	//Check for contact between explosion particles and player turret
	for (auto it = begin(collisionfulEffectEmitters); it != end(collisionfulEffectEmitters); it++) {
		for (auto part = begin((*it)->sys->particles); part != end((*it)->sys->particles); part++) {
			float hDistance = abs((part->position.x + part->radius) - (turretSprite.trans.x + turretSprite.width / 2));
			float vDistance = abs((part->position.y + part->radius) - (turretSprite.trans.y + turretSprite.height / 2));
			float hContactDistance = part->radius + turretSprite.width / 2;
			float vContactDistance = part->radius / 2 + turretSprite.height / 2;
			if (hDistance <= hContactDistance && vDistance <= vContactDistance) {
				collide(&turretSprite, &*part);
				damageSoundPlayer.play();
			}
		}
	}

	//Rate Up Power-up collisions
	for (vector<Sprite>::iterator it = rateUpSystem.sprites.begin(); it != rateUpSystem.sprites.end(); it++) {
		//Contact with turret
		float hDistance = abs((it->trans.x + it->width / 2) - (turretSprite.trans.x + turretSprite.width / 2));
		float vDistance = abs((it->trans.y + it->height / 2) - (turretSprite.trans.y + turretSprite.height / 2));
		float hContactDistance = it->width / 2 + turretSprite.width / 2;
		float vContactDistance = it->height / 2 + turretSprite.height / 2;
		if (hDistance <= hContactDistance && vDistance <= vContactDistance) {
			//Power-up received
			powerUpSoundPlayer.play();
			for (auto it = begin(missileEmitters); it != end(missileEmitters); it++) {
				if ((*it)->rate + RATE_UP_BONUS <= MAX_MISSILE_FIRE_RATE) {
					(*it)->rate += RATE_UP_BONUS;
				}
			}
			it->lifespan = 0;
		}
	}

	//Damage Up Power-up collisions
	for (vector<Sprite>::iterator it = damageUpSystem.sprites.begin(); it != damageUpSystem.sprites.end(); it++) {
		//Contact with turret
		float hDistance = abs((it->trans.x + it->width / 2) - (turretSprite.trans.x + turretSprite.width / 2));
		float vDistance = abs((it->trans.y + it->height / 2) - (turretSprite.trans.y + turretSprite.height / 2));
		float hContactDistance = it->width / 2 + turretSprite.width / 2;
		float vContactDistance = it->height / 2 + turretSprite.height / 2;
		if (hDistance <= hContactDistance && vDistance <= vContactDistance) {
			//Power-up received
			powerUpSoundPlayer.play();
			for (auto it = begin(missileEmitters); it != end(missileEmitters); it++) {
				(*it)->sprite.damage += DAMAGE_UP_BONUS;
			}
			it->lifespan = 0;
		}
	}

	//Weapon Up Power-up collisions
	for (vector<Sprite>::iterator it = weaponUpSystem.sprites.begin(); it != weaponUpSystem.sprites.end(); it++) {
		//Contact with turret
		float hDistance = abs((it->trans.x + it->width / 2) - (turretSprite.trans.x + turretSprite.width / 2));
		float vDistance = abs((it->trans.y + it->height / 2) - (turretSprite.trans.y + turretSprite.height / 2));
		float hContactDistance = it->width / 2 + turretSprite.width / 2;
		float vContactDistance = it->height / 2 + turretSprite.height / 2;
		if (hDistance <= hContactDistance && vDistance <= vContactDistance) {
			//Power-up received
			powerUpSoundPlayer.play();
			if (weaponLevel < missileEmitters.size()) {
				weaponLevel += WEAPON_UP_BONUS;
			}
			it->lifespan = 0;
		}
	}

	//First-aid collisions
	for (vector<Sprite>::iterator it = firstAidSystem.sprites.begin(); it != firstAidSystem.sprites.end(); it++) {
		//Contact with turret
		float hDistance = abs((it->trans.x + it->width / 2) - (turretSprite.trans.x + turretSprite.width / 2));
		float vDistance = abs((it->trans.y + it->height / 2) - (turretSprite.trans.y + turretSprite.height / 2));
		float hContactDistance = it->width / 2 + turretSprite.width / 2;
		float vContactDistance = it->height / 2 + turretSprite.height / 2;
		if (hDistance <= hContactDistance && vDistance <= vContactDistance) {
			//Healing received
			powerUpSoundPlayer.play();
			turretSprite.health = turretSprite.health + 50 > 100 ? 100 : turretSprite.health + 50;
			it->lifespan = 0;
		}
	}
}

// Handles damage calculation when a collision occurs between 2 sprites. Returns true if the main sprite is destroyed, false otherwise
//
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

// Handles damage calculation when a collision occurs between a sprite and a particle. Returns true if the sprite is destroyed, false otherwise
//
bool ofApp::collide(Sprite* main, Particle* other) {
	main->health -= other->damage;

	if (main->health <= 0) {
		main->lifespan = 0;
	}
	other->lifespan = 0;

	return main->health <= 0;
}

// Update the turret sprite's location
//
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

// Sets moveDir to the arrow key pressed
//
void ofApp::moveSprite(MoveDir dir) {
	moveDir = dir;
}

// Stops the sprite
//
void ofApp::stopSprite() {
	moveDir = MoveStop;
}

//--------------------------------------------------------------
void ofApp::draw(){
	if (idle) {
		arialFont.drawString("Press spacebar to start!", ofGetWidth() / 2 - 200, ofGetHeight() / 2);
	}
	else if (!isGameOver) {
		if (slidersActive) {
			panel.draw();
		}
		turretSprite.draw();
		missileSystem.draw();
		alienEnemySystem.draw();
		zombieEnemySystem.draw();
		blueZombieEnemySystem.draw();
		rateUpSystem.draw();
		damageUpSystem.draw();
		weaponUpSystem.draw();
		firstAidSystem.draw();
		healthBar.draw(0, ofGetHeight() - healthBar.getHeight());
		for (auto it = begin(collisionfulEffectEmitters); it != end(collisionfulEffectEmitters); it++) {
			(*it)->draw();
		}
		for (auto it = begin(collisionLessEffectEmitters); it != end(collisionLessEffectEmitters); it++) {
			(*it)->draw();
		}
		ofSetColor(255, 255, 255); //Particle draw calls ofSetColor(...). Need to reverse the effect.
	}
	else {
		arialFont.drawString("Game Over!", ofGetWidth() * 1/4, ofGetHeight() / 2);
	}
	arialFont.drawString("Score:" + to_string(score), 0, ofGetHeight() / 15);
	arialFont.drawString("Level:" + to_string(level), 0, ofGetHeight() / 9);
	arialFont.drawString("Health:" + to_string(turretSprite.health), ofGetWidth() * 1 / 3, ofGetHeight() - 50);
}

// Prevents the turret from exiting out of bounds
//
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
	else if (turretSprite.trans.y > ofGetHeight() - turretSprite.image.getHeight() - healthBar.getHeight()) {
		turretSprite.trans.y = ofGetHeight() - turretSprite.image.getHeight() - healthBar.getHeight();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (!idle) {
		int i = 0;
		switch (key) {
			case ' ':
				for (auto it = begin(missileEmitters); it != end(missileEmitters) && i < weaponLevel; it++) {
					(*it)->start();
					i++;
				}
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
			case OF_KEY_LEFT_CONTROL:
				slidersActive = !slidersActive;
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
			for (auto it = begin(missileEmitters); it != end(missileEmitters); it++) {
				(*it)->stop();
			}
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
