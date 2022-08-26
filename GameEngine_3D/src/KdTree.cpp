//--------------------------------------------------------------
//
//  CS134 - Game Development Spring 2020
//  Kevin Smith   10-20-19
//  


//  Final Project Lander Simulation
//  Student Name:   < Timothy Nguyen >
//  Date: <05-15-2020>


#include "KdTree.h"

// draw KdTree (recursively)
//
void KdTree::draw(TreeNode & node, int numLevels, int level) {
	if (level >= numLevels) return;
	drawBox(node.box);
	//cout << "I have vertices: " << node.points.size() << endl;
	level++;
	for (int i = 0; i < node.children.size(); i++) {
		draw(node.children[i], numLevels, level);
	}


}

// draw only leaf Nodes
// Currently not a working method due to inability to get to 1 vertex per node
void KdTree::drawLeafNodes(TreeNode & node, vector<TreeNode> & leafList) {
	if (node.points.size() <=5) {
		drawBox(node.box);
		leafList.push_back(node);
		return;
	}
	for (int i = 0; i < node.children.size(); i++) {
		if (node.children[i].points.size()>0)
		drawLeafNodes(node.children[i],leafList);
	}


}


//draw a box from a "Box" class  
//
void KdTree::drawBox(const Box &box) {
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
Box KdTree::meshBounds(const ofMesh & mesh) {
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
	cout << "vertices: " << n << endl;
	//	cout << "min: " << min << "max: " << max << endl;
	return Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
}

// getMeshPointsInBox:  return an array of indices to points in mesh that are contained 
//                      inside the Box.  Return count of points found;
//
int KdTree::getMeshPointsInBox(const ofMesh & mesh, const vector<int>& points,
	Box & box, vector<int> & pointsRtn)
{
	//Given the mesh
	//Check if a mesh vertex is located in the box
	//if so, add the vertex to points
	pointsRtn.clear();
	vector<int> pointHolder = points;
	int count = 0;
	for (int i = 0; i < pointHolder.size(); i++) {
		ofVec3f point = mesh.getVertex(pointHolder[i]);
		Vector3 pointVec = Vector3(point.x, point.y, point.z);
		if (box.inside(Vector3(point.x, point.y, point.z))) {
			count++;
			pointsRtn.push_back(pointHolder[i]);
		}
	}
	return count;
}



//  Subdivide a Box; return children in  boxList
//
void KdTree::subDivideBox(const Box &box, vector<Box> & boxList) {
	Vector3 min = box.parameters[0];
	Vector3 max = box.parameters[1];
	Vector3 size = max - min;
	Vector3 center = size / 2;
	float xdist = (max.x() - min.x()) / 2;
	float ydist = (max.y() - min.y()) / 2;
	float zdist = (max.z() - min.z()) / 2;
	Box b[2];

	//3 Different boxes to help cut down on compiling/creation time of tree as it's able to get down to fewer vertices faster
	//X Axis
	if (myLevel % 3 == 0) { //Divisible by 3; Every third level 
		b[0] = Box(min, max - Vector3(xdist, 0, 0));
		b[1] = Box(min + Vector3(xdist, 0, 0), max);

	}
	//Z Axis
	else if ((myLevel % 3 == 1)) { //Remainder of 1; every level after third
		b[0] = Box(min, max - Vector3(0, 0, zdist));
		b[1] = Box(min + Vector3(0, 0, zdist), max);
	}
	//Y Axis
	else { //Remainder of 2; every level after above ^
		b[0] = Box(min, max - Vector3(0, ydist, 0));
		b[1] = Box(min + Vector3(0, ydist, 0), max);
	}


	boxList.clear();
	boxList.push_back(b[0]);
	boxList.push_back(b[1]);
}



void KdTree::create(const ofMesh & geo, int numLevels) {
	// initialize KdTree structure
	//
	mesh = geo;
	int level = 0;

	root.box = meshBounds(mesh);
	for (int i = 0; i < mesh.getNumVertices(); i++) {
		root.points.push_back(i);
	}
	//cout << log2(root.points.size()) << " is the max levels " << endl; //Theoretically, each level means a power of 2 Boxes. E.g 0 level = 1 box, 1 level = 2 box 2 level = 4 box.. etc
	level++;

	subdivide(mesh, root, numLevels, level);

}

void KdTree::subdivide(const ofMesh & mesh, TreeNode & node, int numLevels, int level) {
	if (level >= numLevels) return;
	level++;

	TreeNode left;
	TreeNode right;
	myLevel = level;
	//Divide node into 2 boxes and put into left/right
	subDivideBox(node.box, myChildren);
	left.box = myChildren[0];
	right.box = myChildren[1];

	//Set up the points in each node and then continue subdividing
	if (getMeshPointsInBox(mesh, node.points, right.box, right.points) > 1) {
		subdivide(mesh, right, numLevels, level);
	}
	if (getMeshPointsInBox(mesh, node.points, left.box, left.points) > 1) {
		subdivide(mesh, left, numLevels, level);
	}


	//If it does not have points inside, do not add to tree
	if (left.points.size() == 0) {
	}
	else {
		node.children.push_back(left);
	}
	if (right.points.size() == 0) {
	}
	else {
		node.children.push_back(right);
	}



}


//Finds the smallest(or smallest possible) leaf node box that contains the ray and as gives the nearest vertex
bool KdTree::intersect(const Ray &ray, const TreeNode & node, TreeNode & nodeRtn) {
	bool found = false;


	//If the box intersects and contains less than 5 points, it is the node we want
	if (node.box.intersect(ray, 0, 1000) && node.points.size() <= 10) { // I used 5 because that seems to be my average number of vertices I can get to for the low res. The moon/mars can go lower if using half box (1-2).
		nodeRtn = node;
		found = true;
		return found;
	}

	//Otherwise, recursively go through the children
	for (int i = 0; i < node.children.size(); i++) {
		if (node.children[i].points.size() > 0) {
			found = intersect(ray, node.children[i], nodeRtn);
		}
	}



	return found;

}

