#include "ofApp.h"
#include "Emitter.h"

#include "ofxGui.h"

//reset total game time
// 
// 
// 
//agents pursue player
// agents slow down when near player
// need to rotate
// 
//
//
//
void TriangleShape::draw() {
	ofFill();
	ofPushMatrix();
	ofMultMatrix(getMatrix());
	ofDrawTriangle(verts[0], verts[1], verts[2]);
	ofPopMatrix();
}
//--------------------------------------------------------------

void Player::draw() {
	ofFill();
	ofPushMatrix();
	ofMultMatrix(getMatrix());
	ofDrawTriangle(verts[0], verts[1], verts[2]);
	ofPopMatrix();
}
//--------------------------------------------------------------

void ofApp::setupParameters() {

	p.pos = glm::vec3(ofGetWindowWidth() / 2.0 - 100, ofGetWindowHeight() / 2.0 - 100,
		0);
	p.rotation = -45;

	if (easy) {
		energy = 15;
		invaders->setRate(5);
		invaders->setLifespan(1000);
	}
	else if (hard) {
		energy = 5;
		invaders->setRate(10);
		invaders->setLifespan(5000);
	}
	else
	{
		energy = 10;
		invaders->setRate(5);
		invaders->setLifespan(2000);
	}
}

//--------------------------------------------------------------

void ofApp::setup() {
	ofSetVerticalSync(true);

	//setup Player
	p = Player(glm::vec3(-50, 50, 0), glm::vec3(50, 50, 0), glm::vec3(0,
		-50, 0));
	p.pos = glm::vec3(ofGetWindowWidth() / 2.0 - 100, ofGetWindowHeight() / 2.0 - 100,
		0);
	p.rotation = -45;

	//gui setup
	gui.setup();
	gui.add(speed.setup("Player speed", 5, 1, 20));
	gui.add(size.setup("Player size", 1, .1, 3));
	gui.add(agentSize.setup("Agent size", 15, 5, 50));
	gui.add(agentRate.setup("Agent spawn rate", 5, .5, 10));
	gui.add(agentLife.setup("Agent lifespan", 1000, 0, 5000));


	gui.add(head.setup("Show heading vector", false));
	gui.add(sprite.setup("Show sprites", false));
	guiHide = true ;
	gameOver = false;

	//sprite setup
	p.image.load("dolphin.png");
	p.image.resize(300, 300);

	invaders = new Emitter(new SpriteSystem);

	invaders->setPosition(glm::vec3(ofGetWindowWidth() / 2, 10, 0));
	invaders->setChildSize(10, 10);
	ofImage image;
	image.load("dolphin.png");
	image.resize(invaders->childHeight * 5, invaders->childHeight * 5);
	invaders->setChildImage(image);
	invaders->setRate(20);
	invaders->setLifespan(1000);


	invaders->start();

	setupParameters();
}

//--------------------------------------------------------------
void ofApp::update() {
	invaders->setRate(agentRate);
	invaders->setLifespan(agentLife);
	invaders->childHeight = agentSize;
	invaders->childWidth = agentSize;
	//invaders->setVelocity((p.pos-invaders->trans));
	//invaders->trans = ((p.pos - invaders->trans));
	
	if (sprite) {
		invaders->haveChildImage = true;
	}
	else {
		invaders->haveChildImage = false;
	}


	invaders->update(p.pos);

	checkCollisions();



	if (energy == 0) {
		gameOver = true;
		start = false;
	}

	if (moveUp) {
		float x = speed;
		p.pos += p.heading() * x;
		if (p.getPoint().x > ofGetWindowWidth()) {
			p.pos.x = ofGetWindowWidth() - (50 * size);
		}
		if (p.getPoint().y > ofGetWindowHeight() ) {
			p.pos.y = ofGetWindowHeight() - (50 * size);
		}
		if (p.getPoint().x < 0) {
			p.pos.x = 50 * size;
		}
		if (p.getPoint().y < 0) {
			p.pos.y = 50 * size;
		}
		moveUp = false;
	}

	if (moveBack) {
		float x = speed;
		p.pos -= p.heading() * x;
		if (p.getBackPoint().x > ofGetWindowWidth()) {
			p.pos.x = ofGetWindowWidth() - (50 * size);
		}
		if (p.getBackPoint().y > ofGetWindowHeight()) {
			p.pos.y = ofGetWindowHeight() - (50 * size);
		}
		if (p.getBackPoint().x < 0) {
			p.pos.x = 50 * size;
		}
		if (p.getBackPoint().y < 0) {
			p.pos.y = 50 * size;
		}
		moveBack = false;
	}

	if (moveRt) {
		p.rotation += speed;
		moveRt = false;
	}

	if (moveLt) {
		p.rotation -= speed;
		moveLt = false;
	}

	p.scalar = glm::vec3(size);
	
	if (!gameOver) {
		time = ofGetElapsedTimeMillis();
	}

}

//--------------------------------------------------------------
void ofApp::checkCollisions() {

	// find the distance at which the two sprites (missles and invaders) will collide
	// detect a collision when we are within that distance.
	//
	float collisionDist = 50 / 2 + invaders->childHeight / 2;

	// Loop through all the missiles, then remove any invaders that are within
	// "collisionDist" of the missiles.  the removeNear() function returns the
	// number of missiles removed.
	//
	if (energy > 0) {
		energy -= invaders->sys->removeNear(p.pos, collisionDist);
	}
}



//--------------------------------------------------------------
void ofApp::draw() {
	if (!start) {
		ofSetBackgroundColor(ofColor::blue);

		string startText;
		startText += "Press space to start game ";
		ofSetColor(ofColor::white);
		ofDrawBitmapString(startText, ofPoint(10,20));

		string diffText;
		diffText += "Press q for easy";
		diffText += "Press w for medium";
		diffText += "Press e for hard";

		ofSetColor(ofColor::white);
		ofDrawBitmapString(diffText, ofPoint(10, 50));

		string modeText;
		if (easy) {
			modeText += "Easy";
		}
		else if (hard) {
			modeText += "Hard";
		}
		else {
			modeText += "Normal";
		}
		ofSetColor(ofColor::white);
		ofDrawBitmapString(modeText, ofPoint(ofGetWindowWidth()/2, 50));
	}
	if (gameOver) {
		
		start = false;
		ofSetBackgroundColor(ofColor::black);

		string gameOverText;
		gameOverText += "Game Over ";
		ofSetColor(ofColor::red);
		ofDrawBitmapString(gameOverText, ofPoint(ofGetWindowWidth()/2, ofGetWindowHeight()/2));

		string timeText;
		timeText += "Total time: " + std::to_string(time);
		ofSetColor(ofColor::red);
		ofDrawBitmapString(timeText, ofPoint(ofGetWindowWidth() / 2, (ofGetWindowHeight() / 2) + 50));


		string modeText;
		if (easy) {
			modeText += "Easy";
		}
		else if (hard) {
			modeText += "Hard";
		}
		else {
			modeText += "Normal";
		}
		ofSetColor(ofColor::white);
		ofDrawBitmapString(modeText, ofPoint(ofGetWindowWidth() / 2, 50));
	}
	if (start) {
		//ofResetElapsedTimeCounter();
		//setupParameters();
		gameOver = false;

		ofSetBackgroundColor(ofColor::black);

		invaders->draw();

		string scoreText;
		scoreText += "Energy: " + std::to_string(energy);
		ofSetColor(ofColor::red);
		ofDrawBitmapString(scoreText, ofPoint(10, 20));

		string frameRateText;
		frameRateText += "Frame rate "; 
		frameRateText += std::to_string(ofGetFrameRate());
		ofSetColor(ofColor::white);
		ofDrawBitmapString(frameRateText, ofPoint(ofGetWindowWidth() - 200, 20));

		string timeText;
		timeText += "Elasped time: " + std::to_string(ofGetElapsedTimeMillis());
		ofSetColor(ofColor::white);
		ofDrawBitmapString(timeText, ofPoint(ofGetWindowWidth() - 200, 40));


		//draw player triangle
		if (!sprite) {
			ofSetColor(ofColor::darkViolet);
			p.draw();
		}

		//draw gui
		if (!guiHide) {
			gui.draw();
		}

		//draw heading vector
		if (head) {
			float x = size;									//get scalar of player
			glm::vec3 o = p.heading();
			ofSetColor(ofColor::purple);
			ofSetLineWidth(2);
			ofDrawLine(p.pos, (p.pos + length * x * o));

			for (int i = 0; i < invaders->sys->sprites.size(); i++) {
				glm::vec3 p1 = invaders->sys->sprites[i].heading(p.pos);
				ofSetColor(ofColor::purple);
				ofSetLineWidth(2);
				ofDrawLine(invaders->sys->sprites[i].trans, (invaders->sys->sprites[i].trans + length * p1));
			}
		}

		//draw sprite
		if (sprite) {
			ofSetColor(ofColor::white);
			ofPushMatrix();
			ofMultMatrix(p.getMatrix());
			p.image.draw(0, 0);
			p.image.setAnchorPoint(p.image.getWidth() / 2, p.image.getHeight() / 2);
			ofPopMatrix();
		}
	}
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	keymap[key] = true;
	if (keymap[OF_KEY_LEFT]) {
		moveLt = true;
	}
	if (keymap[OF_KEY_RIGHT]) {
		moveRt = true;
	}
	if (keymap[OF_KEY_UP]) {
		moveUp = true;
	}
	if (keymap[OF_KEY_DOWN]) {
		moveBack = true;
	}
	if (keymap['h']) {
		guiHide = !guiHide;
	}
	if (keymap[OF_KEY_CONTROL]) {
		bCtrlKeyDown = true;
	}
	if (keymap[' ']) {
		if (!start) {
			start = true;
			gameOver = false;
			setupParameters();
		}
	}
	if (keymap['q']) {
		easy = true;
		hard = false;

	}
	if (keymap['e']) {
		easy = false;
		hard = true;

	}
	if (keymap['w']) {
		easy = false;
		hard = false;

	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	keymap[key] = false;

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	glm::vec3 currentMouse(x, y, 0);
	if (bInDrag) {
		glm::vec3 delta = currentMouse - mouseLast;
		p.pos += delta;
		mouseLast = currentMouse;
	}
	else if (bInRotate) {
		glm::vec3 delta = currentMouse - mouseLast;
		p.rotation += delta.x / 2.0;
		mouseLast = currentMouse;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	if (p.inside(glm::vec3(x, y, 0))) {
		if (bCtrlKeyDown) {
			bInRotate = true;
			mouseLast = glm::vec3(x, y, 0);
		}
		else {
			bInDrag = true;
			mouseLast = glm::vec3(x, y, 0);
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	bInDrag = false;
	bCtrlKeyDown = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
