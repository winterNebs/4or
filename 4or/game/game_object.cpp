#include "game_object.h"

#include <iostream>
#include <math.h>
#include <fstream>
GameObject::GameObject() :
	position(0, 0), size(1, 1), velocity(0.0f), color(1.0f), rotation(0.0f), sprite(), 
	isSolid(false){

}
GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprt, bool s, glm::vec3 color, glm::vec2 vel, GLfloat m):
	position(pos), size(size), velocity(vel), color(color), rotation(0.0f), sprite(sprt), 
	isSolid(false), mass_data(m){
}
GLfloat magnitude(glm::vec2 input) {
	return sqrt(pow(input.x, 2) + pow(input.y, 2)); //dont sqrt if possible
}

AABB GameObject::computeAABB() {
	AABB aabb;
	aabb.min = position;
	aabb.max = position + size;
	return aabb;
}
GLfloat calcAng(glm::vec2 input) {
	GLfloat ang = acos(input.x / magnitude(input));
	if (input.y < 0) {
		ang += (GLfloat)(4 * atan(1));
	}
	return ang;
}

void GameObject::resolveCollision(GameObject* obj, GLfloat dt) {
	glm::vec2 rv = obj->getVel() - velocity;
	glm::vec2 tangent = rv - glm::dot(rv, normal) * normal;
	glm::normalize(tangent);
	float velAlongNormal = glm::dot(rv, normal(obj));

	if (velAlongNormal > 0) {
		return;
	}

	float e;
	if (mat.restitution >= obj->mat.restitution) {
		e = obj->mat.restitution;
	}
	else {
		e = mat.restitution;
	}
	float jt = -glm::dot(rv, t);
	jt /= (mass_data.invMass + obj->mass_data.invMass);

	//float mu = sqrt(a.statfric^2 + b.statfric^2)
	glm::vec2 frictionImpulse;

	float j = -(1 + e) * velAlongNormal;
	j /= mass_data.invMass + obj->mass_data.invMass;

	if (abs(jt) < j * mu) {
		frictionImpulse = jt * t;
	}
	glm::vec2 impulse = j * normal(obj);
	velocity -= mass_data.invMass * impulse;
	obj->velocity += obj->mass_data.invMass * impulse;

}

void GameObject::positionCorrection(GameObject* obj) {
	const GLfloat slop = 0.01; // usually 0.01 to 0.1
	const GLfloat percent = 0.2; // 0.2-0.8
								 //glm::vec2 correction = penetrationDepth//
}

GLfloat GameObject::getCloseDist(GameObject* obj) {
	GLfloat closest = INFINITY;
	int indexj = -1;

	for (unsigned int i = 0; i < getVerticies().size(); i++) {
		for (unsigned int j = 0; j < obj->getVerticies().size(); j++) {
			GLfloat temp = calcDist(velocity, getVerticies()[i],
				obj->getVerticies()[j], obj->getVerticies()[(j + 1) % (obj->getVerticies().size())]);
			if (closest > temp && temp >= 0) {
				closest = temp;
			}
		}
	}
	return closest;
}

std::vector<glm::vec2> GameObject::getVerticies() {
	std::vector<glm::vec2> verts;
	verts.push_back(glm::vec2(position.x, position.y));
	verts.push_back(glm::vec2(position.x + size.x, position.y));
	verts.push_back(glm::vec2(position.x + size.x, position.y + size.y));
	verts.push_back(glm::vec2(position.x, position.y + size.y));
	return verts;
}

void GameObject::move(GLfloat dt) {

}
glm::vec2 GameObject::interpolate(GLfloat dt) {
	return position;
}
void GameObject::draw(SpriteRenderer &renderer) {
	renderer.drawSprite(sprite, position, size, rotation, color);
	//std::cout << position.x << ", " << position.y << std::endl;
}
///Get/Set Land
glm::vec2 GameObject::getPos() const {
	return position;
}
glm::vec2 GameObject::getSize() const {
	return size;
}
glm::vec2 GameObject::getVel() const {
	return velocity;
}
void GameObject::move(GLfloat dt) {

	velocity += mass_data.invMass * force * dt;
	position += velocity * dt;
}
GLboolean GameObject::relocate(glm::vec2 loc) {
	position = loc;
	return true;
}