#include "headerSpaghetti.h"
#include <iostream>
Body::Body(Shape* shape, float x, float y) : shape(shape->clone()){
	shape->body = this;
	position = glm::vec2(x, y);
	velocity = glm::vec2(0);
	angularVelocity = 0;
	torque = 0;
	orient = 0;
	force = glm::vec2(0);
	staticFriction = 0.5f;
	dynamicFriction = 0.3f;
	restitution = 0.2f;
	color = glm::vec3(0.5f, 0.5f, 0.5f);
	shape->init();
}

void Body::setOrient(float radians) {
	orient = radians;
	shape->setOrient(radians);
}
void Body::applyForce(const glm::vec2& f) {
	force += f;
}
void Body::applyImpulse(const glm::vec2& impulse, const glm::vec2& contactVector) {
	velocity += im * impulse;
	angularVelocity += iI * crossProduct(contactVector, impulse);
}

void Body::setStatic(void) {
	I = 0.0f;
	iI = 0.0f;
	m = 0.0f;
	im = 0.0f;
}
void Body::setMass(float mass) {
	m = mass;
	im = 1.0f / mass;
}