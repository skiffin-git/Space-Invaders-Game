#pragma once
//--------------------------------------------------------------
//
//  CS134 - Game Development Spring 2020
//  Kevin Smith  
//  


//  Final Project Lander Simulation
//  Student Name:   < Timothy Nguyen >
//  Date: <05-15-2020>
#include "ofMain.h"

bool rayIntersectPlane(const ofVec3f &rayPoint, const ofVec3f &raydir, ofVec3f const &planePoint,
	const ofVec3f &planeNorm, ofVec3f &point);

ofVec3f reflectVector(const ofVec3f &v, const ofVec3f &normal);



