#include "ofApp.h"

//  Raymond Hong - CS 134 SJSU - 11/4/2018
//  Slightly modified version of class code SpriteSystem. Added a check in update() to delete a sprite if it is out-of-bounds on the Y-axis.

//  Add a Sprite to the Sprite System
//
void SpriteSystem::add(Sprite s) {
	sprites.push_back(s);
}

// Remove a sprite from the sprite system. Note that this function is not currently
// used. The typical case is that sprites automatically get removed when the reach
// their lifespan.
//
void SpriteSystem::remove(int i) {
	sprites.erase(sprites.begin() + i);
}


//  Update the SpriteSystem by checking which sprites have exceeded their
//  lifespan (and deleting).  Also the sprite is moved to it's next
//  location based on velocity and direction.
//  Modified to delete sprite if out of bounds.
//
void SpriteSystem::update() {

	if (sprites.size() == 0) return;
	vector<Sprite>::iterator s = sprites.begin();
	vector<Sprite>::iterator tmp;

	// check which sprites have exceed their lifespan and delete
	// from list.  When deleting multiple objects from a vector while
	// traversing at the same time, use an iterator.
	//
	while (s != sprites.end()) {
		if (s->lifespan != -1 && s->age() > s->lifespan) {
			//          cout << "deleting sprite: " << s->name << endl;
			tmp = sprites.erase(s);
			s = tmp;
		}
		//If sprite is out of bounds on the y-axis, delete it
		else if (s->trans.y > ofGetHeight() + s->height || s->trans.y < -1.0 * s->height) {
			tmp = sprites.erase(s);
			s = tmp;
		}
		else s++;
	}

	//  Move sprite
	//
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].trans += sprites[i].velocity / ofGetFrameRate();
	}
}

//  Render all the sprites
//
void SpriteSystem::draw() {
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].draw();
	}
}