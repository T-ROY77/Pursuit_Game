


#include "ofMain.h"
#include "Emitter.h"



//  Create a new Emitter - needs a SpriteSystem
//
Emitter::Emitter(SpriteSystem* spriteSys) {
	sys = spriteSys;
	lifespan = 3000;    // milliseconds
	started = false;

	lastSpawned = 0;
	rate = 1;    // sprites/sec
	haveChildImage = false;
	haveImage = false;
	velocity = ofVec3f(0, 0, 0);
	drawable = false;
	width = 50;
	height = 50;
	childWidth = 10;
	childHeight = 10;
}

//  Draw the Emitter if it is drawable. In many cases you would want a hidden emitter
//
//
void Emitter::draw() {
	if (drawable) {

		if (haveImage) {
			ofSetColor(ofColor::white);
			image.draw(-image.getWidth() / 2.0 + trans.x, -image.getHeight() / 2.0 + trans.y);
		}
		else {
			ofSetColor(0, 0, 255);
			ofDrawRectangle(-width / 2 + trans.x, -height / 2 + trans.y, width, height);
		}
	}

	// draw sprite system
	//
	sys->draw();
}

//  Update the Emitter. If it has been started, spawn new sprites with
//  initial velocity, lifespan, birthtime.
//
void Emitter::update(glm::vec3 p) {
	if (!started) return;

	float time = ofGetElapsedTimeMillis();
	if ((time - lastSpawned) > (1000.0 / rate)) {
		// spawn a new sprite
		Sprite sprite;
		if (haveChildImage) sprite.setImage(childImage);
		sprite.velocity = velocity;
		sprite.lifespan = lifespan;

		//set random spawn point
		sprite.setPosition(glm::vec3(ofRandom(ofGetWindowWidth() - 1), ofRandom(ofGetWindowHeight() - 1), 0));
		//sprite.setPosition(trans);

		sprite.birthtime = time;
		sprite.width = childWidth;
		sprite.height = childHeight;
		sys->add(sprite);
		lastSpawned = time;
	}
	sys->update(p);
}

// Start/Stop the emitter.
//
void Emitter::start() {
	started = true;
	lastSpawned = ofGetElapsedTimeMillis();
}

void Emitter::stop() {
	started = false;
}


void Emitter::setLifespan(float life) {
	lifespan = life;
}

void Emitter::setVelocity(ofVec3f v) {
	velocity = v;
}

void Emitter::setChildImage(ofImage img) {
	childImage = img;
}

void Emitter::setImage(ofImage img) {
	image = img;
	haveImage = true;
}

void Emitter::setRate(float r) {
	rate = r;
}

float Emitter::maxDistPerFrame() {
	return  velocity.length() / ofGetFrameRate();
}