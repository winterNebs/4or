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
	GLfloat fastest = calcTime(velocity, getVerticies()[0],
		obj->getVerticies()[0], obj->getVerticies()[(0 + 1) % obj->getVerticies().size()]);;
	int indexj = 0;
	
	for (int i = 0; i < getVerticies().size(); i++) {
		for (int j = 0; j < obj->getVerticies().size(); j++) {
			GLfloat temp = calcTime(velocity, getVerticies()[i],
				obj->getVerticies()[j], obj->getVerticies()[(j+1)%(obj->getVerticies().size())]);
			if (fastest > temp) {
				fastest = temp;
				indexj = j;
			}
		}
	}
	glm::vec2 normal = glm::vec2(obj->getVerticies()[indexj].y - obj->getVerticies()[(indexj + 1) % obj->getVerticies().size()].y,
		-(obj->getVerticies()[indexj].x - obj->getVerticies()[(indexj + 1) % obj->getVerticies().size()].x));
	normal = glm::normalize(normal);
	if (glm::distance(interpolate(dt), normal + interpolate(dt)) > glm::distance(interpolate(dt), interpolate(dt) - normal)) {
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
	GLfloat slope1 = dir.y / dir.x;
	GLfloat slope2 = (line1.y - line2.y) / (line1.x - line2.x);
	GLfloat xval = (point.y - line1.y + (line1.x * slope2) + (point.x * slope1)) /
		(slope2 - slope1);
	return xval / point.x;
}

std::vector<glm::vec2> GameObject::getVerticies() {
	std::vector<glm::vec2> verts;
	verts.push_back(glm::vec2(position.x, position.y));
	verts.push_back(glm::vec2(position.x + size.x, position.y));
	verts.push_back(glm::vec2(position.x + size.x, position.y + size.y));
	verts.push_back(glm::vec2(position.x, position.y + size.y));
	return verts;
}