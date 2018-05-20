#include "game_object.h"

#include <iostream>
#include <math.h>

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
glm::vec2 GameObject::getAccel() const {
	return acceleration;
}
glm::vec2 GameObject::getSize() const {
	return size;
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
GLboolean GameObject::collide(GameObject* obj, const GLfloat dt) { ///Two axis collision
	glm::vec2 intperPos = interpolate(dt);
	glm::vec2 objPos = obj->interpolate(dt);

	bool colX = intperPos.x + size.x >= objPos.x && objPos.x + obj->size.x >= intperPos.x;
	bool colY = intperPos.y + size.y >= objPos.y && objPos.y + obj->size.y >= intperPos.y;
	return colX && colY;
}

glm::vec2 GameObject::interpolate(const GLfloat dt) {
	return position;
}
GLfloat GameObject::calcFastest(GameObject* obj) {
	GLfloat fastest = calcTime(velocity, getVerticies()[0],
		obj->getVerticies()[0], obj->getVerticies()[(0 + 1) % obj->getVerticies().size()]);;
	for (int i = 0; i < getVerticies().size(); i++) {
		for (int j = 0; j < obj->getVerticies().size(); j++) {
			GLfloat temp = calcTime(velocity, getVerticies()[i],
				obj->getVerticies()[j], obj->getVerticies()[(j + 1) % (obj->getVerticies().size())]);
			if (fastest > temp) {
				fastest = temp;
			}
		}
	}
	return fastest;
}
glm::vec2 GameObject::normal(GameObject* obj, const  GLfloat dt) {
	///Axis rectangle assumption
	///Check collision first
	GLfloat fastest = calcTime(velocity, getVerticies()[0],
		obj->getVerticies()[0], obj->getVerticies()[(0 + 1) % obj->getVerticies().size()]);;
	int indexj = 0;
	
	for (int i = 0; i < getVerticies().size(); i++) {
		for (int j = 0; j < obj->getVerticies().size(); j++) {
			GLfloat temp = calcTime(velocity, getVerticies()[i],
				obj->getVerticies()[j], obj->getVerticies()[(j+1)%(obj->getVerticies().size())]);
			
			//std::cout << "i:" << i << "\tj:" << j << "\tTime:" << temp << "\tFastest:" << fastest << std::endl;
			
			if (fastest > temp) {
				fastest = temp;
				indexj = j;
			}
			else if (fastest == temp) {

				std::cout << "SAME" << std::endl;
			}
		}
	}
	glm::vec2 normal = glm::vec2(obj->getVerticies()[indexj].y - obj->getVerticies()[(indexj + 1) % obj->getVerticies().size()].y,
		-(obj->getVerticies()[indexj].x - obj->getVerticies()[(indexj + 1) % obj->getVerticies().size()].x));
	normal = glm::normalize(normal);
	if (glm::distance(interpolate(fastest), normal + interpolate(fastest)) > glm::distance(interpolate(fastest), interpolate(fastest) - normal)) {
		normal = -normal;
		std::cout << "flip" << std::endl;
	}

	return normal;
}
GLfloat GameObject::calcTime(glm::vec2 dir, glm::vec2 point, glm::vec2 line1, glm::vec2 line2) {
	//Dir * time = 
	//function y = (dir.y/dir.x)(x-point.x)+point.y
	//function y = ((line1.y-line2.y)/(line1.x-line1.y))(x-line1.x) + line1.y
	//Trust me the algebra works out

	GLfloat slope1 = dir.y / dir.x;			//X coefficient 1
	GLfloat deltay = -(line1.y - line2.y);
	GLfloat deltax = -(line1.x - line2.y);
	GLfloat slope2 = deltay / deltax; //X coefficient 2
	GLfloat yCo1 = 1;
	GLfloat yCo2 = 1;
	GLfloat const1 = point.y + (slope1 * point.x);
	GLfloat const2 = line1.y + (slope2 * line1.x);
	
	if (dir.x == 0) {
		yCo1 = 0;
		const1 = point.x;
		slope1 = 1;
	}
	if (deltax == 0) {
		yCo2 = 0;
		const2 = line1.x;
		slope2 = 1;
	}
	glm::mat2x2 A(yCo1, slope1, yCo2, slope2);
	glm::vec2 B(const1, const2);
	glm::vec2 ans;
	ans = glm::inverse(A) * B;
	/*
	[yCo1, slope1] * [x] = [const1]
	[yCo2, slope2] * [y] = [const2]
	*/
	//std::cout << ans.x << "," << ans.y << std::endl;
	if (dir.x == 0) {
		return (ans.y - point.y) / dir.y;
	}
	return (ans.x - point.x) / dir.x;
}

std::vector<glm::vec2> GameObject::getVerticies() {
	std::vector<glm::vec2> verts;
	verts.push_back(glm::vec2(position.x, position.y));
	verts.push_back(glm::vec2(position.x + size.x, position.y));
	verts.push_back(glm::vec2(position.x + size.x, position.y + size.y));
	verts.push_back(glm::vec2(position.x, position.y + size.y));
	return verts;
}