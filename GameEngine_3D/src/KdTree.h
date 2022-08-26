//--------------------------------------------------------------
//
//  CS134 - Game Development Spring 2020
//  Kevin Smith   10-20-19
//  


//  Final Project Lander Simulation
//  Student Name:   < Timothy Nguyen >
//  Date: <05-15-2020>


#pragma once
#include "ofMain.h"
#include "box.h"
#include "ray.h"

//  General purpose TreeNode class
//

class TreeNode {
public:
	Box box;
	vector<int> points;
	vector<TreeNode> children;    // for binary KdTree, this array has just two(2) members)
};

class KdTree {
public:

	// Create and initilize tree given an OpenFrameWorks Mesh and # of levels.  if numLevels
	// is larger than the number of levels possible in the data, then the tree will be built to
	// its maximum depth with only one point in each leaf node.
	//
	void create(const ofMesh & mesh, int numLevels);

	//  Recursive call to subdivide mesh starting at the "node"
	//
	void subdivide(const ofMesh & mesh, TreeNode & node, int numLevels, int currentLevel);

	//  Test for intersection of Tree given a ray and root node.  If a node is hit, return true
	//  and the correspondind node in "nodeRtn"
	//
	bool intersect(const Ray &, const TreeNode & node, TreeNode & nodeRtn);

	//  Recursive calls to draw the tree
	//
	void draw(TreeNode & node, int numLevels, int level);
	void draw(int numLevels, int level) {
		draw(root, numLevels, level);
	}

	// Draw just the leaf nodes
	//  
	void drawLeafNodes(TreeNode & node,vector <TreeNode> &leafList);

	// utility function to draw a box in OF from a "Box" class
	//
	static void drawBox(const Box &box);

	//  Returns a bounding Box for the mesh
	//
	static Box meshBounds(const ofMesh &);

	//  Test which mesh points in "points" are inside "box"; return inside box in "pointsRtn"
	//
	int getMeshPointsInBox(const ofMesh &mesh, const vector<int> & points, Box & box, vector<int> & pointsRtn);

	//   subdvide a Box into separate boxes.  for a binary tree, subdivide into two (2) boxes.
	//
	void subDivideBox(const Box &b, vector<Box> & boxList);


	vector<Box> myChildren;
	//  local data
	//
	ofMesh mesh;
	TreeNode root;
	int myLevel; //Used for axis splitting


};