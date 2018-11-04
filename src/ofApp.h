#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Particle.h"
#include "ParticleEmitter.h"


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

		bool isGameOver;

		int score;
		int level;
		float currentAlienCurveIntensity;
		float lastRotated;
		static const int LEVEL_TWO_REQUIREMENT = 5;
		static const int LEVEL_THREE_REQUIREMENT = 10;

		static const int INITIAL_ALIEN_CURVE_INTENSITY = 70;
		static const int MAX_ALIEN_CURVE_INTENSITY = 500;
		static const int INITIAL_ZOMBIE_Y_VELOCITY = 50; 
		static const int INITIAL_BLUE_ZOMBIE_Y_VELOCITY = 100;

		static const int RATE_UP_CHANCE = 15; //15%
		static const int RATE_UP_BONUS = 3;
		static const int MAX_RATE = 985;

		static const int DAMAGE_UP_CHANCE = 5; //5%
		static const int DAMAGE_UP_BONUS = 5;

		static const int WEAPON_UP_CHANCE = 10; //10%
		static const int WEAPON_UP_BONUS = 1;

		static const int FIRST_AID_CHANCE = 15; //15%
		static const int FIRST_AID_BONUS = 50;

		static const int TURRET_MAX_HEALTH = 100;

		bool idle;
		bool leftPressed;
		bool rightPressed;
		bool upPressed;
		bool downPressed;
		ofVec3f mouseLast;

		Sprite turretSprite;
		MoveDir moveDir;

		SpriteSystem missileSystem;
		vector<Emitter*> missileEmitters;
		Emitter mainMissileEmitter;

		Emitter leftSideMissileEmitter;
		Emitter rightSideMissileEmitter;
		Emitter farLeftSideMissileEmitter;
		Emitter farRightSideMissileEmitter;

		float weaponLevel;

		vector<Emitter*> enemyEmitters;
		Emitter* alienEmitter;
		Emitter* zombieEmitter;
		Emitter* blueZombieEmitter;
		SpriteSystem alienEnemySystem;
		SpriteSystem zombieEnemySystem;
		SpriteSystem blueZombieEnemySystem;

		vector<ParticleEmitter*> particleEmitters;

		vector<ParticleEmitter*> explosionEffectEmitters;

		Emitter rateUpEmitter;
		SpriteSystem rateUpSystem;


		Emitter damageUpEmitter;
		SpriteSystem damageUpSystem;


		Emitter weaponUpEmitter;
		SpriteSystem weaponUpSystem;

		Emitter firstAidEmitter;
		SpriteSystem firstAidSystem;

		
		ofxFloatSlider rateSlider;
		ofxFloatSlider directionSlider;

		ofxFloatSlider enemyRateSlider;
		ofxVec3Slider enemyVelocitySlider;
		ofxFloatSlider enemyLifespanSlider;

		ofxPanel panel;

		ofTrueTypeFont arialFont;

		ofImage healthBar;


		ofSoundPlayer powerUpSoundPlayer;
		ofSoundPlayer destroySoundPlayer;
		ofSoundPlayer damageSoundPlayer;
};