#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>

class Shape {
public:
	glm::vec3 pos = glm::vec3(0,0,0);
	float rot = 0; 
	glm::vec3 scl = glm::vec3(1,1,1);
	std::vector<glm::vec3> vert;

	Shape() {}
	virtual void draw() {}
	virtual bool inside(glm::vec3 p, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
		return true;
	}

	glm::mat4 transform() {
		glm::mat4 transformation = glm::translate(glm::mat4(1.0), glm::vec3(pos));
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0), glm::radians(rot), glm::vec3(0, 0, 1));
		glm::mat4 scale = glm::scale(glm::mat4(1.0), scl);

		glm::mat4 T = transformation * rotation * scale;

		return T;
	}
};