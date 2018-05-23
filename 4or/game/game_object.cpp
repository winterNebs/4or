#include "game_object.h"

#include <iostream>
#include <math.h>
#include <fstream>
GameObject::GameObject() :
	position(0, 0), size(1, 1), velocity(0.0f), color(1.0f), rotation(0.0f), sprite(), 
	isSolid(false), isStatic(true), friction(100.0f), acceleration(0.0f) {
}
GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprt, bool s, glm::vec3 color, glm::vec2 vel):
	position(pos), size(size), velocity(vel), color(color), rotation(0.0f), sprite(sprt), 
	isSolid(false), isStatic(s), friction(100.0f), acceleration(0.0f) {
}
glm::vec2 GameObject::getPos() const {
	return position;
}
glm::vec2 GameObject::getSize() const {
	return size;
}
glm::vec2 GameObject::getVel() const {
	return velocity;
}
glm::vec2 GameObject::getFriction() const {
	return friction;
}
void GameObject::setFriction(glm::vec2 fric) {
	friction = fric;
}
GLboolean GameObject::relocate(glm::vec2 loc) {
	position = loc;
	return true;
}
void GameObject::move(GLfloat dt){

}
void GameObject::draw(SpriteRenderer &renderer) {
	renderer.drawSprite(sprite, position, size, rotation, color);
	//std::cout << position.x << ", " << position.y << std::endl;
}
GLboolean GameObject::collide(GameObject* obj, GLfloat dt) { ///Two axis collision
	glm::vec2 intperPos = interpolate(dt);
	glm::vec2 objPos = obj->interpolate(dt);

	bool colX = intperPos.x + size.x >= objPos.x && objPos.x + obj->size.x >= intperPos.x;
	bool colY = intperPos.y + size.y >= objPos.y && objPos.y + obj->size.y >= intperPos.y;
	return colX && colY;
}

glm::vec2 GameObject::interpolate(GLfloat dt) {
	return position;
}
glm::vec2 GameObject::normal(GameObject* obj, GLfloat dt) {
	///Axis rectangle assumption
	///Check collision first
	std::ofstream file;
	file.open(".\\logs\\normals.txt", std::ios_base::app);
	file << "==CALC NORMAL==\n";
	file << "Collider: ";
	for (int i = 0; i < getVerticies().size(); i++) {
		file << i << "(" << getVerticies()[i].x << "," << getVerticies()[i].y << ") ";
	}
	file << "\n"; 
	file << "Collidee: ";
	for (int i = 0; i < obj->getVerticies().size(); i++) {
		file << i << "(" << obj->getVerticies()[i].x << "," << obj->getVerticies()[i].y << ") ";
	}
	file << "\n";
	GLfloat closest = INFINITY;
	int indexj = -1;
	
	for (int i = 0; i < getVerticies().size(); i++) {
		for (int j = 0; j < obj->getVerticies().size(); j++) {
			GLfloat temp = calcDist(velocity, getVerticies()[i],
				obj->getVerticies()[j], obj->getVerticies()[(j+1)%(obj->getVerticies().size())]);
			file << "i:" << i << " j:" << j << " dist:" << temp << "\n";
			if (closest > temp && temp >= 0) {
				closest = temp;
				indexj = j;
			}
		}
	}
	file << "indexj:" << indexj << " closest:" << closest << "\n";
	//std::cout << "j:" << indexj << ", j+1:" << (indexj + 1) % obj->getVerticies().size() << std::endl;
	if (indexj == -1) {
		return glm::vec2(0, 0);
	}
	glm::vec2 normal = glm::vec2(obj->getVerticies()[indexj].y - obj->getVerticies()[(indexj + 1) % obj->getVerticies().size()].y,
		-(obj->getVerticies()[indexj].x - obj->getVerticies()[(indexj + 1) % obj->getVerticies().size()].x));
	normal = glm::normalize(normal);

	if (glm::distance(interpolate(calcTime(closest)), normal + interpolate(calcTime(closest))) > glm::distance(interpolate(calcTime(closest)), interpolate(calcTime(closest)) - normal)) {
		normal = -normal;
		std::cout << "flip" << std::endl;
	}

	file.close();
	return normal;
}
GLfloat GameObject::getCloseDist(GameObject* obj) {
	GLfloat closest = INFINITY;
	int indexj = -1;

	for (int i = 0; i < getVerticies().size(); i++) {
		for (int j = 0; j < obj->getVerticies().size(); j++) {
			GLfloat temp = calcDist(velocity, getVerticies()[i],
				obj->getVerticies()[j], obj->getVerticies()[(j + 1) % (obj->getVerticies().size())]);
			if (closest > temp && temp >= 0) {
				closest = temp;
			}
		}
	}
	return closest;
}
GLfloat GameObject::calcDist(glm::vec2 dir, glm::vec2 point, glm::vec2 line1, glm::vec2 line2) {
	//Dir * time = 
	//function y = (dir.y/dir.x)(x-point.x)+point.y
	//function y = ((line1.y-line2.y)/(line1.x-line1.y))(x-line1.x) + line1.y
	//Redo
	std::ofstream file;
	file.open(".\\logs\\normals.txt", std::ios_base::app);
	file << "Vertex: " << "(" << point.x << "," << point.y << ") Dir: (" << dir.x << "," << dir.y << ")\n";
	
	file << "Lines: " << "(" << line1.x << "," << line1.x << ") (" << line2.x << ", " << line2.x << ")\n";
	GLfloat slope1 = dir.y / dir.x;								//M1
	GLfloat slope2 = (line1.y - line2.y) / (line1.x - line2.x);	//M2
	GLfloat yCo1 = 1;
	GLfloat yCo2 = 1;
	GLfloat const1 = point.y - (slope1 * point.x);
	GLfloat const2 = line1.y - (slope2 * line1.x);
	file << "slope1, slope2: " << slope1 << "," << slope2 << "\n";
	file << "yCo1, yCo2: " << yCo1 << "," << yCo2 << "\n";
	file << "const1, const2: " << const1 << "," << const2 << "\n";

	if (slope1 == slope2 ) {
		return -1;
	}
	///Bad coding lol
	if (dir.x == 0) {
		slope1 = 1;
		const1 = point.x;
		yCo1 = 0;
	}
	if (line1.x - line2.x == 0) {
		slope2 = 1;
		const2 = line1.x;
		yCo2 = 0;
	}
		//std::cout << slope1 << "," << slope2 << "," << yCo1 << "," << yCo2 << "," << const1 << "," << const2 << std::endl;
	/*
	-slope1 x + yCo1 y = const1
	-slope2 x + yCo2 y = const2
	[-slope1 yCo1 C]
	[D E F]
	*/
	
	GLfloat xval = ((const1 * yCo2) - (yCo1 * const2)) / (((-slope1) * yCo2) - (yCo1 * (-slope2)));
	GLfloat yval = (((-slope1) * const2) - (const1 * (-slope2))) / (((-slope1) * yCo2) - (yCo1 * (-slope2)));
	file << "POI: (" << xval << "," << yval << ")\n";
	if (dir.y == 0 && line1.x - line2.x == 0) {
		xval = line1.x;
		yval = point.y;
	}
	if (dir.x == 0 && line1.y - line2.y == 0) {
		xval = point.x;
		yval = line1.y;
	}
	//std::cout << (xval / dir.x) << "," << (yval / dir.y) << std::endl;
	/*GLfloat xval = (point.y - line1.y + (line1.x * slope2) + (point.x * slope1)) /
		(slope2 - slope1);
	return xval / point.x;*/


	if (((xval >= line1.x && xval <= line2.x) || (xval >= line2.x && xval <= line1.x)) &&
		((yval >= line1.y && yval <= line2.y) || (yval >= line2.y && yval <= line1.y))) {
		return sqrt(pow(xval-point.x, 2) + pow(yval-point.y, 2));
	}

	else return -1;
}

std::vector<glm::vec2> GameObject::getVerticies() {
	std::vector<glm::vec2> verts;
	verts.push_back(glm::vec2(position.x, position.y));
	verts.push_back(glm::vec2(position.x + size.x, position.y));
	verts.push_back(glm::vec2(position.x + size.x, position.y + size.y));
	verts.push_back(glm::vec2(position.x, position.y + size.y));
	return verts;
}
GLfloat GameObject::calcTime(GLfloat dist) {
	//std::cout << "Dist:\t" << dist << " Accel:\t" << sqrt(pow(acceleration.x, 2) + pow(acceleration.y, 2));
	if (abs(acceleration.x) > 0 || abs(acceleration.y) > 0) {
		return sqrt((dist) / sqrt(pow(acceleration.x, 2) + pow(acceleration.y, 2)));
	}
	return (dist) / sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));
}