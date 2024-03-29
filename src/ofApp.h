#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Particle.h"
#include "ParticleEmitter.h"

//  Raymond Hong - CS 134 SJSU - 11/4/2018
//  Header file for ofApp


typedef enum {MoveStop, MoveLeft, MoveRight, MoveUp, MoveDown } MoveDir;

class BaseObject {
	public:
		ofVec2f trans, scale;
		float rot;
		bool bSelected;
		void setPosition(ofVec3f p) { trans = p;  }
};

class Sprite : public BaseObject {
public:
	Sprite();
	void draw();
	float age();
	float speed; // in pixels/sec
	ofVec2f velocity; // in pixels/sec
	ofImage image;
	float birthtime; //elapsed time in ms
	float lifespan; //time in ms
	string name;
	bool haveImage;
	float width, height;
	int health;
	float damage;
};

class SpriteSystem {
public:
	void add(Sprite);
	void remove(int);
	void update();
	void draw();
	vector<Sprite> sprites;
};

class Emitter : public BaseObject {
public:
	Emitter();
	void start();
	void stop();
	void emit();
	void rotate(ofVec2f*);
	void loadEmitSound(string);
	void loadSpriteImage(string);
	void resizeImage(float, float);
	SpriteSystem *sys;
	float rate;
	int rateCounter;
	ofVec2f velocity;
	float lifespan;
	bool started;
	float direction;
	ofSoundPlayer soundPlayer;
	Sprite sprite;
	static const int MAX_RATE = 1000;
};


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void updateSprite();
		void moveSprite(MoveDir);
		void stopSprite();
		void checkTurretBounds();
		void checkCollisions();
		void checkLevel();
		void scaleEnemies();
		void randomizeMovement(SpriteSystem*);
		void curveVelocity(SpriteSystem*, float);
		bool collide(Sprite*, Sprite*);
		bool collide(Sprite*, Particle*);
		void checkGameOver();
		void initMissileEmitters();
		void initPowerUpEmitters();
		void setupSliderGui();
		void initEnemyEmitters();

		bool slidersActive;
		bool isGameOver;
		bool idle;
		int score;
		int level;
		float currentAlienCurveIntensity;
		float lastRotated;
		float weaponLevel;

		static const int LEVEL_TWO_REQUIREMENT = 10;
		static const int LEVEL_THREE_REQUIREMENT = 50;

		static const int INITIAL_ALIEN_CURVE_INTENSITY = 70;
		static const int MAX_ALIEN_CURVE_INTENSITY = 500;
		static const int INITIAL_ZOMBIE_Y_VELOCITY = 60; 
		static const int MAX_ALIEN_RATE = 985;
		static const int MAX_ZOMBIE_RATE = 985;
		static const int MAX_BUG_RATE = 985;
		static const int INITIAL_BUG_Y_VELOCITY = 130;

		static const int RATE_UP_CHANCE = 20; //20%
		static const int RATE_UP_BONUS = 3;
		static const int MAX_MISSILE_FIRE_RATE = 985;
		static const int DAMAGE_UP_CHANCE = 5; //5%
		static const int DAMAGE_UP_BONUS = 5;
		static const int WEAPON_UP_CHANCE = 20; //20%
		static const int WEAPON_UP_BONUS = 1;
		static const int FIRST_AID_CHANCE = 10; //10%
		static const int FIRST_AID_BONUS = 50;
		static const int TURRET_MAX_HEALTH = 100;

		bool leftPressed;
		bool rightPressed;
		bool upPressed;
		bool downPressed;
		ofVec3f mouseLast;

		Sprite turretSprite;
		MoveDir moveDir;

		SpriteSystem missileSystem;
		vector<Emitter*> missileEmitters;

		//Enemies
		vector<Emitter*> enemyEmitters;
		Emitter* alienEmitter;
		Emitter* zombieEmitter;
		Emitter* bugEmitter;
		SpriteSystem alienEnemySystem;
		SpriteSystem zombieEnemySystem;
		SpriteSystem bugEnemySystem;

		vector<ParticleEmitter*> collisionfulEffectEmitters; //'Dangerous' bullets that can hurt the player
		vector<ParticleEmitter*> collisionLessEffectEmitters; //Just for visuals. Does not collide with anything

		//Power-ups
		Emitter rateUpEmitter;
		SpriteSystem rateUpSystem;
		Emitter damageUpEmitter;
		SpriteSystem damageUpSystem;
		Emitter weaponUpEmitter;
		SpriteSystem weaponUpSystem;
		Emitter firstAidEmitter;
		SpriteSystem firstAidSystem;

		//Slider GUI
		ofxPanel panel;
		ofxFloatSlider rateSlider;
		ofxFloatSlider directionSlider;
		ofxFloatSlider enemyRateSlider;
		ofxVec3Slider enemyVelocitySlider;
		ofxFloatSlider enemyLifespanSlider;

		ofTrueTypeFont arialFont;

		ofImage healthBar;
		ofImage background;

		ofSoundPlayer powerUpSoundPlayer;
		ofSoundPlayer destroySoundPlayer;
		ofSoundPlayer damageSoundPlayer;
};