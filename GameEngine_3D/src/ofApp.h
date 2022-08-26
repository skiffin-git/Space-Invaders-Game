#pragma once

#include "ofMain.h"
#include  "ofxAssimpModelLoader.h"
#include "box.h"
#include "ray.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"
#include "KdTree.h"
#include "ofxGui.h"
#include "glm/gtx/intersect.hpp"

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
		void drawAxis(ofVec3f);
		void initLightingAndMaterials();
		void savePicture();
		void toggleWireframeMode();
		void togglePointsDisplay();
		void toggleSelectTerrain();
		void setCameraTarget();
		void loadVbo();
		void loadVboEnvironment();
		void drawBox(const Box &box);
		void startThrusterAnimation();        //Starts the DiskEmitter
		void stopThrusterAnimation();         //Stops the DiskEmitter
		void calculateDirectionalVectors();   //Calculates the new heading values with respect to angle
		
		bool twoPointsOfContact();

		Box meshBounds(const ofMesh &);
	
		ofTexture particleTex, particleTex1;
		ofVbo vbo, vbo1;
		ofShader shader, shader1;

		ofEasyCam cam;
		ofCamera top;
		ofCamera *theCam;
		ofCamera frontView;
		ofCamera sideRight;
		ofCamera sideLeft;
		ofCamera fixedPoint;
		ofxAssimpModelLoader terrain, lander;
		ofLight light, keyLight, fillLight, rimLight;

		Box boundingBox;
		Box landerBounds;
		Box landingAreaOne;

		bool bAltKeyDown;
		bool bCtrlKeyDown;
		bool bWireframe;
		bool bDisplayPoints;
		bool bPointSelected;
		bool turbulence; //Turbulence should only be applied when actually "flying"
		bool bInDrag;
		bool bLanderLoaded = false;
		bool bTerrainSelected;
		bool bLanderSelected = false;
		bool bBackgroundLoaded = false;
		bool startGame;
		bool leftLocated;
		bool located;
		bool gameStarted;
		bool landed;
		bool collisionLeft, collisionRight, collisionFront, collisionBehind;
		bool movement;
		bool gameFinished;
		bool noCollisions = true;
		bool heightReached;
		bool crashed;
		float fuel;

		ofTrueTypeFont font, font2, font3, font4, font5;
		ofSoundPlayer bgMusic, thrustSound;

		ofImage backgroundImage;

		ofVec3f selectedPoint;
		ofVec3f intersectPoint;
		ofVec3f initialUp, initialDown, initialRight, initialLeft;     //Initial Heading vectors for each direction	
		ofVec3f thrusterUp, thrusterDown, thrusterRight, thrusterLeft; //Applies the angle of the rocket  to the initial heading vectors to get a new heading vector
		ofVec3f landingPointOne;

		glm::vec3 mouseLastPos;
		glm::vec3 mouseDownPos;
		glm::vec3 getMousePointOnPlane();
		glm::vec3 ofApp::getMousePointOnPlane(glm::vec3 planePt, glm::vec3 planeNorm);

		ofVec3f frontVertex, backVertex, rightVertex, leftVertex;
			
		Particle *modulePosition;  //Simulates one particle as the module
		ParticleSystem *sys;
		ParticleEmitter *thrustEmitter; //Provides the DiskEmitter for our LEM

		

		const float selectionRange = 4.0;

		float startTime;
		float totalScore;
		float distance;
		float angle;
		float distanceToGround;
		float framesLeft, framesRight, framesBehind, framesFront;
		float distanceFront;
		float lastCollision;
		int frames = 0;
	
		vector<TreeNode> listOfLeafNode;
		KdTree kdtree;

		Vector3 box0, box1;
	

		
};


