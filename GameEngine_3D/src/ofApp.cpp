
//--------------------------------------------------------------
//
//  CS134 - Game Development Spring 2020
//  Kevin Smith   10-20-19
//  


//  Final Project Lander Simulation
//  Student Name:   < Timothy Nguyen >
//  Date: <05-15-2020>


#include "ofApp.h"
#include "Util.h"



//--------------------------------------------------------------
// setup scene, lighting, state and load geometry
//
void ofApp::setup() {

	gameStarted = false;

	//set up landing area box
	landingAreaOne = Box(Vector3(66.45, -0.72, 109.28), Vector3(72.8, 4.28, 115.707));

	//load background music and thrust sounds
	bgMusic.load("sounds/bgMusic.mp3", true);
	bgMusic.setLoop(true);
	bgMusic.play();
	bgMusic.setVolume(.05);

	thrustSound.load("sounds/Rocket.mp3", true);
	thrustSound.setVolume(1);

	// font
	font.load("fonts/seaside.ttf", 50, true, true, true);
	font2.load("fonts/seaside.ttf", 20, true, true, true);
	font3.load("fonts/seaside.ttf", 20, true, true, true);

	ofDisableArbTex();
	//set up shader particles
	if (!ofLoadImage(particleTex, "images/dot.png")) {
		cout << "Particle Texture File: images/dot.png not found" << endl;
		ofExit();
	}

#ifdef TARGET_OPENGLES
	shader.load("shaders_gles/shader");
#else
	shader.load("shaders/shader");
#endif

	if (!ofLoadImage(particleTex1, "images/dot.png")) {
		cout << "Particle Texture File: images/dot.png not found" << endl;
		ofExit();
	}

#ifdef TARGET_OPENGLES
	shader.load("shaders_gles/shader");
#else
	shader.load("shaders/shader");
#endif

	//Set up lights
	ofEnableDepthTest();
	ofEnableLighting();
	keyLight.setup();
	keyLight.enable();
	keyLight.setAreaLight(2, 2);
	keyLight.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1));
	keyLight.setDiffuseColor(ofFloatColor(0.30, 0.64, 0.64));
	keyLight.setSpecularColor(ofFloatColor(0.30, 0.64, 0.64));

	//keyLight.rotate(45, ofVec3f(0, 1, 0));
	keyLight.rotate(-220, ofVec3f(1, 0, 0));
	keyLight.setPosition(0, 29, 29);

	fillLight.setup();
	fillLight.enable();
	fillLight.setSpotlight();
	fillLight.setScale(.05);
	fillLight.setSpotlightCutOff(5);
	fillLight.setAttenuation(.8, .0001, .0001);

	fillLight.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1));
	fillLight.setDiffuseColor(ofFloatColor(0.14, 0, 0.9));
	fillLight.setSpecularColor(ofFloatColor(0.14, 0, 0.9));
	fillLight.rotate(-20, ofVec3f(1, 0, 0));
	fillLight.rotate(40, ofVec3f(0, 1, 0));
	fillLight.setPosition(-12, 7, 15);

	rimLight.setup();
	rimLight.enable();
	rimLight.setSpotlight();
	rimLight.setScale(.05);
	rimLight.setSpotlightCutOff(13);
	rimLight.setAttenuation(.8, .0001, .0001);
	rimLight.setAmbientColor(ofFloatColor(0.1, 0.1, 0.1));
	rimLight.setDiffuseColor(ofFloatColor(0.01, 0.65, 0.86));
	rimLight.setSpecularColor(ofFloatColor(0.01, 0.65, 0.86));
	rimLight.rotate(220, ofVec3f(0, 1, 0));
	rimLight.rotate(-67, ofVec3f(1, 0, 0));
	rimLight.setPosition(4, 5, -15);





	bWireframe = false;
	bDisplayPoints = false;
	bAltKeyDown = false;
	bCtrlKeyDown = false;
	bLanderLoaded = false;
	bTerrainSelected = true;

	fuel = 300;


	//	ofSetWindowShape(1024, 768);
	// set up cameras
	cam.setDistance(40);
	cam.setNearClip(.1);
	cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
	ofSetVerticalSync(true);
	cam.disableMouseInput();
	ofEnableSmoothing();
	ofEnableDepthTest();
	frontView.lookAt(glm::vec3(0, 0, -1));
	top.setPosition(0, 25, 0);
	top.lookAt(glm::vec3(0, 0, 0));
	top.setNearClip(.1);
	top.setFov(65.5);   // approx equivalent to 28mm in 35mm format
	frontView.setNearClip(.1);
	frontView.setFov(65.5);
	sideRight.setNearClip(.1);
	sideRight.setFov(65.5);
	sideLeft.setNearClip(.1);
	sideLeft.setFov(65.5);
	sideLeft.lookAt(glm::vec3(1, 0, 0));
	sideRight.lookAt(glm::vec3(-1, 0, 0));

	fixedPoint.setPosition(191, 12, 154);
	fixedPoint.lookAt(glm::vec3(0, 0, 0));
	fixedPoint.setNearClip(.1);
	fixedPoint.setFov(65.5);
	theCam = &cam;

	//Set up terrain and background
	bBackgroundLoaded = backgroundImage.load("images/asd.png");
	terrain.loadModel("geo/moon-houdini.obj");
	terrain.setScaleNormalization(false);

	boundingBox = meshBounds(terrain.getMesh(0));

	//setup particle
	modulePosition = new Particle();
	modulePosition->position = ofVec3f(0, 0, 0);
	modulePosition->lifespan = -1;

	//Set up system
	sys = new ParticleSystem();
	sys->add(*modulePosition);
	thrustEmitter = new ParticleEmitter(sys); //Emits the thruster particles

	//Set up thrust emitter for particles
	thrustEmitter->setEmitterType(DiscEmitter);
	thrustEmitter->visible = false;
	thrustEmitter->setParticleRadius(10);
	angle = 0;

	//Add moon gravity
	sys->addForce(new GravityForce(ofVec3f(0,-0.166,0)));

	//Set up with reasonable initial vectors for each direction
	initialUp = ofVec3f(0, 0, -5);
	initialDown = ofVec3f(0, 0, 5);
	initialRight = ofVec3f(5, 0, 0);
	initialLeft = ofVec3f(-5, 0, 0);


	landingPointOne = ofVec3f(69.5, -0.83, 111.5);

	//set up kdtree
	kdtree.create(terrain.getMesh(0), 40);
	kdtree.drawLeafNodes(kdtree.root, listOfLeafNode);



}

// load vertex buffer in preparation for rendering
//
void ofApp::loadVbo() {
	if (thrustEmitter->sys->particles.size() < 1) return;

	vector<ofVec3f> sizes;
	vector<ofVec3f> points;
	for (int i = 0; i < thrustEmitter->sys->particles.size(); i++) {
		points.push_back(thrustEmitter->sys->particles[i].position);
		sizes.push_back(ofVec3f(10));
	}


	// upload the data to the vbo
	//
	int total = (int)points.size();
	vbo.clear();
	vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
	vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
}


//--------------------------------------------------------------
// incrementally update scene (animation)
//
void ofApp::update() {


	if (bLanderLoaded) {
	
		sys->update();
	
		ofVec3f moveTo = (sys->particles[0].position);
		lander.setRotation(0, angle, 0, 1, 0);
		calculateDirectionalVectors();
		framesFront++;
		//Cameras move with lander
		frontView.setPosition(lander.getPosition()+glm::vec3(0,25,5));
		top.setPosition(lander.getPosition() + glm::vec3(0, 20, 0));
		sideRight.setPosition(lander.getPosition() + glm::vec3(10, 0, 0));
		sideLeft.setPosition(lander.getPosition() + glm::vec3(-10, 0, 0));

		fillLight.setPosition(moveTo + (-100, 15, 5));
		rimLight.setPosition(moveTo+(-20,15,-5));
		
		float distanceToLandingAreaOne = landingPointOne.distance(ofVec3f(lander.getPosition().x, lander.getPosition().y, lander.getPosition().z));

		Vector3 min = landerBounds.parameters[0];
		Vector3 max = landerBounds.parameters[1];
		Vector3 size = max - min;
		Vector3 center = size / 2;
		float xdist = (max.x() - min.x()) / 2;
		float ydist = (max.y() - min.y()) / 2;
		float zdist = (max.z() - min.z()) / 2;

		if (moveTo.y < -5) {
			crashed = true;
		}
		if (fuel <= 0) {
			crashed = true;
		}
		if (landed) {
			if (distanceToLandingAreaOne < 5) {
		
				gameFinished = true;
				startTime = ofGetElapsedTimef();
				return;
			}
		}
		// Landing Code
		if (twoPointsOfContact() && distanceToGround < 0.6 || distanceToGround<0.6 && distanceToLandingAreaOne <2 ) {
			turbulence = false;
			for (int i = 0; i < sys->forces.size(); i++) {
				sys->forces[i]->stopTurbulence = true;
			}
			if (!movement) {
				landed = true;
			}
			for (int i = 0; i < sys->forces.size(); i++) {
				sys->forces[i]->stop = true;
			}
		}
		if (distanceToGround> 0 && !turbulence) {
			sys->addForce(new TurbulenceForce(ofVec3f(-1, -1, -1), ofVec3f(1, 1, 1)));
			turbulence = true;
			sys->addForce(new GravityForce(ofVec3f(0, -0.166, 0)));
		}
		if (!landed) { //Ensures the lander does not go "below" the surface
			thrustEmitter->update();
			thrustEmitter->setPosition(moveTo);
			lander.setPosition(moveTo.x, moveTo.y, moveTo.z);
			//cout << sys->particles[0].velocity << endl;
		}
		
		

		
		//Collision Checking
		if (bLanderLoaded) {

			Vector3 min = landerBounds.parameters[0];
			Vector3 max = landerBounds.parameters[1];
			Vector3 size = max - min;
			Vector3 center = size / 2;
			float xdist = (max.x() - min.x()) / 2;
			float ydist = (max.y() - min.y()) / 2;
			float zdist = (max.z() - min.z()) / 2;
			
			//I used the 4 points of the landing gear of the lander
			//Vector3 pointOne = landerBounds.parameters[0] + Vector3(xdist, 0, 2 * zdist); // behind
			//Vector3 pointTwo = landerBounds.parameters[1] - Vector3(0, 2 * ydist, zdist);//right
			//Vector3 pointThree = landerBounds.parameters[0] + Vector3(0, 0, zdist);//left
			//Vector3 pointFour = landerBounds.parameters[0] + Vector3(xdist, 0, 0);//front

			TreeNode belowMe;
			located = kdtree.intersect(Ray(Vector3(lander.getPosition().x, lander.getPosition().y, lander.getPosition().z), Vector3(0, -1, 0)), kdtree.root, belowMe);
			float minimum = 1000;
			for (int i = 0; i < belowMe.points.size(); i++) {
				ofVec3f groundVertex = kdtree.mesh.getVertex(kdtree.root.points[belowMe.points[i]]);
				float distance = groundVertex.distance(ofVec3f(lander.getPosition().x,lander.getPosition().y,lander.getPosition().z));
				if (distance < minimum) {
					minimum = distance;
				}
			}
			distanceToGround = minimum;
			if (distanceToGround > 100) {
				heightReached = true;
			}


	
		}


		
			Vector3 pointFour = landerBounds.parameters[0] + Vector3(xdist, 0, 0);//front
			ofVec3f front = ofVec3f(pointFour.x(), pointFour.y(), pointFour.z()) + ofVec3f(0, 0, -0.5 * zdist);
			for (int i = 0; i < listOfLeafNode.size(); i++) {
				if (listOfLeafNode[i].box.inside(pointFour + Vector3(0,0, -0.5*zdist))&&!landed) {
					collisionFront = true; //soon to collide
					
					for (int j = 0; j < listOfLeafNode[i].points.size(); i++) {
						float collisionDistance;
						ofVec3f collisionVertex = kdtree.mesh.getVertex(listOfLeafNode[i].points[j]);
						collisionDistance = collisionVertex.distance(front);
						
						if (distanceToGround > 1 && framesFront > 30 ) {
							sys->addForce(new CollisionForce(ofGetFrameRate(), 0));
							framesFront = 0;
							noCollisions = false;
						}
					}
					
				}
				else {
					collisionFront = false;
				}
			
			}



			Vector3 pointOne = landerBounds.parameters[0] + Vector3(xdist, 0, 2 * zdist); // behind
			ofVec3f back = ofVec3f(pointOne.x(), pointOne.y(), pointOne.z()) + ofVec3f(0, 0, zdist);
			for (int i = 0; i < listOfLeafNode.size(); i++) {
				if (listOfLeafNode[i].box.inside(pointOne + Vector3(0, 0, 0.5*zdist))&&!landed) {
					
					collisionBehind = true;
					if (distanceToGround > 1 && distanceToLandingAreaOne > 5) {
						if (lastCollision + 2 > ofGetElapsedTimef()) {
							sys->addForce(new CollisionForce(ofGetFrameRate(), 0));
							lastCollision = ofGetElapsedTimef();
						}
						noCollisions = false;
					}
				}
				else {
					collisionBehind = false;
				}
				
			}

			Vector3 pointThree = landerBounds.parameters[0] + Vector3(0, 0, zdist);//left
			ofVec3f left = ofVec3f(pointThree.x(), pointThree.y(), pointThree.z()) + ofVec3f(-1 * xdist, 0, 0);
			for (int i = 0; i < listOfLeafNode.size(); i++) {
				if (listOfLeafNode[i].box.inside(pointThree + Vector3(-0.5*xdist, 0, 0))&&!landed) {
					
					collisionLeft = true;
					if (distanceToGround > 1 && distanceToLandingAreaOne > 5) {
						sys->addForce(new CollisionForce(ofGetFrameRate(), 2));
						noCollisions = false;
					}
				}
				else {
					collisionLeft = false;
				}
			
			}
	
			Vector3 pointTwo = landerBounds.parameters[1] - Vector3(0, 2 * ydist, zdist);//right
			ofVec3f right = ofVec3f(pointTwo.x(), pointTwo.y(), pointTwo.z()) + ofVec3f(xdist, 0, 0);
			for (int i = 0; i < listOfLeafNode.size(); i++) {
				if (listOfLeafNode[i].box.inside(pointTwo + Vector3(0.5*xdist, 0, 0))&&!landed) {
				
					collisionRight = true; 
					if (distanceToGround > 1 && distanceToLandingAreaOne > 5) {
						sys->addForce(new CollisionForce(ofGetFrameRate(), 0));
						noCollisions = false;
					}
				}
				else {
					collisionRight = false;
				}
			
			}

	}
}

// I used two points of contact as necessary for a landing otherwise the lander would be unstable on only one leg
bool ofApp::twoPointsOfContact() {
	//return (collisionLeft && collisionRight) || (collisionLeft && collisionFront) || (collisionLeft &&collisionBehind) ||
	//	(collisionRight && collisionFront) || (collisionRight &&collisionBehind) ||
	//	(collisionFront && collisionBehind);
	return collisionLeft || collisionRight || collisionFront || collisionBehind;

}




//--------------------------------------------------------------
void ofApp::draw() {

	//Initial Screen
	//if (!startGame) {
	//	ofBackground(ofColor::black);
	//	ofPushMatrix();
	//	string titleText = "Untitled Lander Simulation";
	//	ofTranslate(-70, ofGetWindowHeight()/4);
	//	font.drawString(titleText, font.stringWidth(titleText)/15, 0);
	//	ofPopMatrix();
	//	return;
	//}

	if (crashed) {
			ofBackground(ofColor::black);
		ofPushMatrix();
		string titleText = "Moonlander crashed!\n To restart press space bar.";
		ofTranslate(-70, ofGetWindowHeight()/4);
		font.drawString(titleText, font.stringWidth(titleText)/15, 0);
		ofPopMatrix();
		return;
	}

	loadVbo();

	//Draw background scene
	if (bBackgroundLoaded) {
		ofPushMatrix();
		ofDisableDepthTest();
		ofSetColor(255, 255, 255);
		ofScale(2, 2);
		backgroundImage.draw(-200, -100);
		ofEnableDepthTest();
		ofPopMatrix();
	}
	else {
		ofBackground(ofColor::black);
	}



	theCam->begin();




	ofPushMatrix();



	if (bWireframe) {                    // wireframe mode  (include axis)
		ofDisableLighting();
		ofSetColor(ofColor::slateGray);
		terrain.drawWireframe();
		if (bLanderLoaded) {
			lander.drawWireframe();
			if (!bTerrainSelected) drawAxis(lander.getPosition());
		}
		if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
	}
	else {
		ofEnableLighting();              // shaded mode
		terrain.drawFaces();

		if (bLanderLoaded) {
			lander.drawFaces();
			if (!bTerrainSelected) drawAxis(lander.getPosition());

			ofVec3f min = lander.getSceneMin() + lander.getPosition();
			ofVec3f max = lander.getSceneMax() + lander.getPosition();

			Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
			// set color of bounding box, red if selected, white otherwise
			landerBounds = bounds;
			if (bLanderSelected) {
				ofSetColor(ofColor::red);
			}
			else {
				ofSetColor(ofColor::white);
			}
			//drawBox(bounds);
		}
		if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
	}
	//ofDrawSphere(frontVertex, 2);


	if (bDisplayPoints) {                // display points as an option    
		glPointSize(3);
		ofSetColor(ofColor::green);
		terrain.drawVertices();
	}


	// highlight selected point (draw sphere around selected point)
	//
	if (bPointSelected) {
		ofSetColor(ofColor::blue);
		ofDrawSphere(selectedPoint, .1);
	}

	ofNoFill();
	ofSetColor(ofColor::white);
	//	drawBox(boundingBox);
	drawBox(landingAreaOne);
	theCam->end();
	glDepthMask(GL_FALSE);
	ofSetColor(ofColor::yellow);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofEnablePointSprites();
	shader.begin();

	theCam->begin();
	//thrustEmitter->draw();
	particleTex.bind();

	vbo.draw(GL_POINTS, 0, (int)thrustEmitter->sys->particles.size());
	ofSetColor(255, 100, 90);

	particleTex.unbind();

	theCam->end();
	shader.end();

	ofDisablePointSprites();
	ofDisableBlendMode();
	ofEnableAlphaBlending();
	glDepthMask(GL_TRUE);



	// set back the depth mask
	//
	glDepthMask(GL_TRUE);
	string str;
	str += "Frame Rate: " + std::to_string(ofGetFrameRate());
	ofSetColor(ofColor::white);
	ofDrawBitmapString(str, ofGetWindowWidth() - 170, 15);
	string goals;
	goals += "GOAL: Reach the landing area without crashing";
	ofDrawBitmapString(goals, 5, 75);
	string modifiers;

	string str2;
	string str3;
	string str4;
	string str5;
	string start;
	if (!bLanderLoaded) {
		start = "Click anywhere to spawn Moonlander";
		ofTranslate(0, ofGetWindowHeight() / 4);
		font.drawString(start, font.stringWidth(start) / 15, 0);
	}
	if (bLanderLoaded) {
			str2 += "Altitide (AGL): " + std::to_string(distanceToGround);
			ofSetColor(ofColor::white);
			ofDrawBitmapString(str2, 5, 15);
			str3 += "W and S for VERTICAL movement | UP, DOWN, LEFT, RIGHT KEYS for HORIZONTAL movement";
			ofSetColor(ofColor::white);
			str4 += "CAMERAS: F1-F6";
			str5 += "FUEL: " + std::to_string(fuel);
			ofDrawBitmapString(str3, 5, 30);
			ofDrawBitmapString(str4, 5, 45);
			ofDrawBitmapString(str5, 5, 60);
	}
	if (gameFinished) {
		string finishText = "Congratualtions! You sucessfuly landed in the assigned area.";
		ofTranslate(50, ofGetWindowHeight() / 4);
		font2.drawString(finishText, font2.stringWidth(finishText) / 15, 0);
		ofPopMatrix();
	}
	 box0 = landerBounds.parameters[0];
	 box1 = landerBounds.parameters[1];
	
}
void ofApp::startThrusterAnimation() {
	if (!thrustEmitter->started) {
		thrustEmitter->start();
	}
}

//Tells the DiscEmitter to stop
void ofApp::stopThrusterAnimation() {
	if (thrustEmitter->started) {

		thrustEmitter->stop();
	}
}

void ofApp::calculateDirectionalVectors() {
	thrusterUp = ofVec3f(initialUp.x*cos(ofDegToRad(angle)) - initialUp.z*sin(ofDegToRad(angle))*-1, 0, initialUp.x*sin(ofDegToRad(angle)) + initialUp.z*cos(ofDegToRad(angle)));
	thrusterDown = ofVec3f(initialDown.x*cos(ofDegToRad(angle)) - initialDown.z*sin(ofDegToRad(angle))*-1, 0, initialDown.x*sin(ofDegToRad(angle)) + initialDown.z*cos(ofDegToRad(angle)));
	thrusterLeft = ofVec3f(initialLeft.x*cos(ofDegToRad(angle)) - initialLeft.z*sin(ofDegToRad(angle)), 0, initialLeft.x*sin(ofDegToRad(angle))*-1 - initialLeft.z*cos(ofDegToRad(angle)));
	thrusterRight = ofVec3f(initialRight.x*cos(ofDegToRad(angle)) - initialRight.z*sin(ofDegToRad(angle)), 0, initialRight.x*sin(ofDegToRad(angle))*-1 + initialRight.z*cos(ofDegToRad(angle)));
}

// 

// Draw an XYZ axis in RGB at world (0,0,0) for reference.
//
void ofApp::drawAxis(ofVec3f location) {

	ofPushMatrix();
	ofTranslate(location);

	ofSetLineWidth(1.0);

	// X Axis
	ofSetColor(ofColor(255, 0, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));


	// Y Axis
	ofSetColor(ofColor(0, 255, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));

	// Z Axis
	ofSetColor(ofColor(0, 0, 255));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));

	ofPopMatrix();
}


void ofApp::keyPressed(int key) {

	switch (key) {
	case 'a':
		angle += 15;
		break;
	case 'C':
	case 'c':
		if (cam.getMouseInputEnabled()) cam.disableMouseInput();
		else cam.enableMouseInput();
		break;
	case 'd':
		angle -= 15;
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'H':
	case 'h':
		cout << " my current position is " << lander.getPosition() << endl;
		cout << " my box position min is " << box0.x() <<", " << box0.y() << ", " << box0.z() << endl;
		cout << " my box position max is " << box1.x() << ", " << box1.y() << ", " << box1.z() << endl;
		break;
	case 'r':
		cam.reset();
		break;
	case 's':     // spacefraft thrust DOWN
		sys->addForce(new ThrusterForce(ofVec3f(0, -10, 0)));
		startThrusterAnimation();
		thrustSound.play();
		fuel -= 0.1;
		break;
	case 't':
		setCameraTarget();
		break;
	case 'u':
		break;
	case 'v':
		togglePointsDisplay();
		break;
	case 'V':
		toggleWireframeMode();
		break;
	case 'w':
			sys->addForce(new ThrusterForce(ofVec3f(0, 10, 0)));       //Because "Up" and "Down" don't change I just set an arbitrary value
			startThrusterAnimation();
			thrustSound.play();
			fuel -= 0.1;
		movement = true;
	
			landed = false;
		
		
		break;
		
	case OF_KEY_ALT:
		cam.enableMouseInput();
		bAltKeyDown = true;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = true;
		break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_DEL:
		break;
	case OF_KEY_F1:
		theCam = &cam;
		break;
	case OF_KEY_F2:
		theCam = &top;
		break;
	case OF_KEY_F3:
		theCam = &frontView;
		break;
	case OF_KEY_F4:
		theCam = &sideRight;
		break;
	case OF_KEY_F5:
		theCam = &sideLeft;
		break;
	case OF_KEY_F6:
		theCam = &fixedPoint;
		break;
	case OF_KEY_UP:    // move forward
		sys->addForce(new ThrusterForce(thrusterUp));
		startThrusterAnimation();
		thrustSound.play();
		fuel -= 0.1;
		break;
	case OF_KEY_DOWN:   // move backward
		sys->addForce(new ThrusterForce(thrusterDown));
		startThrusterAnimation();
		thrustSound.play();
		fuel -= 0.1;
		break;
	case OF_KEY_LEFT:   // move left
		sys->addForce(new ThrusterForce(thrusterLeft));
		startThrusterAnimation();
		thrustSound.play();
		fuel -= 0.1;
		break;
	case OF_KEY_RIGHT:   // move right
		sys->addForce(new ThrusterForce(thrusterRight));
		startThrusterAnimation();
		thrustSound.play();
		fuel -= 0.1;
		break;
	case ' ':
		if (crashed) {
			crashed = false;
			heightReached = false;
			noCollisions = true;
			bLanderLoaded = false;
		}
		startGame = true;
		ofResetElapsedTimeCounter();
		break;
	default:
		break;
	}
}

void ofApp::toggleWireframeMode() {
	bWireframe = !bWireframe;
}

void ofApp::toggleSelectTerrain() {
	bTerrainSelected = !bTerrainSelected;
}

void ofApp::togglePointsDisplay() {
	bDisplayPoints = !bDisplayPoints;
}

void ofApp::keyReleased(int key) {

	switch (key) {

	case OF_KEY_ALT:
		cam.disableMouseInput();
		bAltKeyDown = false;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = false;
		break;
	case OF_KEY_SHIFT:
		break;
	case 'w':
		stopThrusterAnimation();
		thrustSound.stop();
		movement = false;
		break;
	case 's':
		stopThrusterAnimation();
		thrustSound.stop();
		break;
	case  OF_KEY_UP:
		stopThrusterAnimation();
		thrustSound.stop();
		break;
	case OF_KEY_RIGHT:
		stopThrusterAnimation();
		thrustSound.stop();
		break;
	case OF_KEY_LEFT:
		stopThrusterAnimation();
		thrustSound.stop();
		break;
	case OF_KEY_DOWN:
		stopThrusterAnimation();
		thrustSound.stop();
		break;
	default:
		break;

	}
}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {



}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	// if moving camera, don't allow mouse interaction
	//
	if (cam.getMouseInputEnabled()) return;
	
		
	//Check for lander selection
	if (bLanderLoaded) {
		glm::vec3 p = theCam->screenToWorld(glm::vec3(x, y, 0));
		glm::vec3 rayDir = glm::normalize(p - theCam->getPosition());

		if (bLanderLoaded) { 
			glm::vec3 origin = cam.getPosition();
			glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
			glm::vec3 mouseDir = glm::normalize(mouseWorld - origin); 
			ofVec3f min = lander.getSceneMin() + lander.getPosition(); 
			ofVec3f max = lander.getSceneMax() + lander.getPosition(); 
			Box bounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z)); 
			bool hit = bounds.intersect(Ray(Vector3(origin.x, origin.y, origin.z), Vector3(mouseDir.x, mouseDir.y, mouseDir.z)), 0, 10000); 
			if (hit) { 
				bLanderSelected = true; 
				mouseDownPos = getMousePointOnPlane(lander.getPosition(), cam.getZAxis());
				mouseLastPos = mouseDownPos; 
				bInDrag = true; 
			} else {
				bLanderSelected = false; 
			} 
		}
	}
	//Load lander at mouse click
	else {
		lander.loadModel("geo/lander.obj");
		bLanderLoaded = true;
		lander.setScaleNormalization(false);
		//	lander.setScale(.5, .5, .5);
		glm::vec3 origin = theCam->getPosition();
		glm::vec3 camAxis = theCam->getZAxis();
		glm::vec3 mouseWorld = theCam->screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
		float distance;

		bool hit = glm::intersectRayPlane(origin, mouseDir, glm::vec3(0, 0, 0), camAxis, distance);
		if (hit) {
		
			glm::vec3 intersectPoint = origin + distance * mouseDir;

		
			glm::vec3 min = lander.getSceneMin();
			glm::vec3 max = lander.getSceneMax();
			float offset = (max.y - min.y) / 2.0;
			if (intersectPoint.y - offset > 0) {
				lander.setPosition(intersectPoint.x, intersectPoint.y - offset, intersectPoint.z);
			}
			else {
				lander.setPosition(intersectPoint.x, 0, intersectPoint.z);
			}
			sys->particles[0].position = lander.getPosition();
			thrustEmitter->setPosition(lander.getPosition());
		
			landerBounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
		}
	}



}


//draw a box from a "Box" class  
//
void ofApp::drawBox(const Box &box) {
	Vector3 min = box.parameters[0];
	Vector3 max = box.parameters[1];
	Vector3 size = max - min;
	Vector3 center = size / 2 + min;
	ofVec3f p = ofVec3f(center.x(), center.y(), center.z());
	float w = size.x();
	float h = size.y();
	float d = size.z();
	ofDrawBox(p, w, h, d);
}

// return a Mesh Bounding Box for the entire Mesh
//
Box ofApp::meshBounds(const ofMesh & mesh) {
	int n = mesh.getNumVertices();
	ofVec3f v = mesh.getVertex(0);
	ofVec3f max = v;
	ofVec3f min = v;
	for (int i = 1; i < n; i++) {
		ofVec3f v = mesh.getVertex(i);

		if (v.x > max.x) max.x = v.x;
		else if (v.x < min.x) min.x = v.x;

		if (v.y > max.y) max.y = v.y;
		else if (v.y < min.y) min.y = v.y;

		if (v.z > max.z) max.z = v.z;
		else if (v.z < min.z) min.z = v.z;
	}
	return Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
}


//Mouse drags are dependent on the lander being situated in the middle of the obj.
void ofApp::mouseDragged(int x, int y, int button) {
	//Essentially is the same as dragging an .obj file into port except updating it with the mouse dragged location
	//Setting up rays + cam plane
	glm::vec3 origin = theCam->getPosition();
	glm::vec3 camAxis = theCam->getZAxis();
	glm::vec3 mouseWorld = theCam->screenToWorld(glm::vec3(mouseX, mouseY, 0));
	glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
	float distance;
	if (bInDrag&&bLanderSelected) {
		glm::vec3 landerPos = lander.getPosition();
		glm::vec3 mousePos = getMousePointOnPlane(landerPos, cam.getZAxis()); 
		glm::vec3 delta = mousePos - mouseLastPos; 
		landerPos += delta;
		lander.setPosition(landerPos.x, landerPos.y, landerPos.z); mouseLastPos = mousePos;
		sys->particles[0].position = lander.getPosition();
		thrustEmitter->setPosition(lander.getPosition());
	}

	bool hit = glm::intersectRayPlane(origin, mouseDir, glm::vec3(0, 0, 0), camAxis, distance);


}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	bLanderSelected = false;
}



// Set the camera to use the selected point as it's new target
//  
void ofApp::setCameraTarget() {

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
// setup basic ambient lighting in GL  (for now, enable just 1 light)
//
void ofApp::initLightingAndMaterials() {

	static float ambient[] =
	{ .5f, .5f, .5, 1.0f };
	static float diffuse[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float position[] =
	{ 5.0, 5.0, 5.0, 0.0 };

	static float lmodel_ambient[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float lmodel_twoside[] =
	{ GL_TRUE };


	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
}

void ofApp::savePicture() {
	ofImage picture;
	picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	picture.save("screenshot.png");
	cout << "picture saved" << endl;
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent(ofDragInfo dragInfo) {
	if (lander.loadModel(dragInfo.files[0])) {
		bLanderLoaded = true;
		lander.setScaleNormalization(false);
		//	lander.setScale(.5, .5, .5);
		lander.setPosition(0, 0, 0);
		//	lander.setRotation(1, 180, 1, 0, 0);

			// We want to drag and drop a 3D object in space so that the model appears 
			// under the mouse pointer where you drop it !
			//
			// Our strategy: intersect a plane parallel to the camera plane where the mouse drops the model
			// once we find the point of intersection, we can position the lander/lander
			// at that location.
			//

			// Setup our rays
			//
		glm::vec3 origin = theCam->getPosition();
		glm::vec3 camAxis = theCam->getZAxis();
		glm::vec3 mouseWorld = theCam->screenToWorld(glm::vec3(mouseX, mouseY, 0));
		glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
		float distance;

		bool hit = glm::intersectRayPlane(origin, mouseDir, glm::vec3(0, 0, 0), camAxis, distance);
		if (hit) {
			// find the point of intersection on the plane using the distance 
			// We use the parameteric line or vector representation of a line to compute
			//
			// p' = p + s * dir;
			//
			glm::vec3 intersectPoint = origin + distance * mouseDir;

			// Now position the lander's origin at that intersection point
			//
			glm::vec3 min = lander.getSceneMin();
			glm::vec3 max = lander.getSceneMax();
			float offset = (max.y - min.y) / 2.0;
			lander.setPosition(intersectPoint.x, intersectPoint.y - offset, intersectPoint.z);
			sys->particles[0].position = lander.getPosition();
			thrustEmitter->setPosition(lander.getPosition());
			// set up bounding box for lander while we are at it
			//
			landerBounds = Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
		}
	}


}


//  intersect the mouse ray with the plane normal to the camera 
//  return intersection point.   (package code above into function)
//
glm::vec3 ofApp::getMousePointOnPlane() {
	// Setup our rays
	//
	glm::vec3 origin = theCam->getPosition();
	glm::vec3 camAxis = theCam->getZAxis();
	glm::vec3 mouseWorld = theCam->screenToWorld(glm::vec3(mouseX, mouseY, 0));
	glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
	float distance;

	bool hit = glm::intersectRayPlane(origin, mouseDir, glm::vec3(0, 0, 0), camAxis, distance);

	if (hit) {
		// find the point of intersection on the plane using the distance 
		// We use the parameteric line or vector representation of a line to compute
		//
		// p' = p + s * dir;
		//
		glm::vec3 intersectPoint = origin + distance * mouseDir;

		return intersectPoint;
	}
	else return glm::vec3(0, 0, 0);
}

glm::vec3 ofApp::getMousePointOnPlane(glm::vec3 planePt, glm::vec3 planeNorm) {
	// Setup our rays//
	glm::vec3 origin = cam.getPosition();
	glm::vec3 camAxis = cam.getZAxis();
	glm::vec3 mouseWorld = cam.screenToWorld(glm::vec3(mouseX, mouseY, 0));
	glm::vec3 mouseDir = glm::normalize(mouseWorld - origin);
	float distance;bool hit = glm::intersectRayPlane(origin, mouseDir, planePt, planeNorm, distance);
	if (hit) {
		// find the point of intersection on the plane using the distance 
		//We use the parameteric line or vector representation of a line to compute//// p' = p + s * dir;//
		glm::vec3 intersectPoint = origin + distance * mouseDir;
		return intersectPoint;}
	else return glm::vec3(0, 0, 0);}