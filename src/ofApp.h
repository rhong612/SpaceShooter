#pragma once

#include "ofMain.h"
#include "ofxGui.h"


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
	SpriteSystem *sys;
	float rate;
	float rateCounter;
	ofVec2f velocity;
	float lifespan;
	bool started;
	float direction;
	ofSoundPlayer soundPlayer;
	Sprite sprite;
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
		void curveVelocity(SpriteSystem*, float);

		int score;

		bool idle;
		bool leftPressed;
		bool rightPressed;
		bool upPressed;
		bool downPressed;
		ofVec3f mouseLast;

		Sprite turretSprite;
		MoveDir moveDir;

		SpriteSystem missileSystem;
		Emitter missileEmitter;

		vector<Emitter> enemyEmitters;
		SpriteSystem enemySystem;

		
		ofxFloatSlider rateSlider;
		ofxFloatSlider directionSlider;
		ofxPanel panel;
};