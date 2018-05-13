#include <iostream>
#include <math.h>
#include "game_object.h"

GameObject::GameObject() :
	position(0, 0), size(1, 1), velocity(0.0f), color(1.0f), rotation(0.0f), sprite(), 
	isSolid(false), destroyed(false), friction(100.0f), acceleration(0.0f) {
}
GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprt, glm::vec3 color, glm::vec2 vel):
	position(pos), size(size), velocity(vel), color(color), rotation(0.0f), sprite(sprt), 
	isSolid(false), destroyed(false), friction(100.0f), acceleration(0.0f) {
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
	/*if (fric.x > 1.0f) {
		fric.x = 1.0f;
	}
	if (fric.x < 0.0f) {
		fric.x = 0.0f;
	}
	if (fric.y > 1.0f) {
		fric.y = 1.0f;
	}
	if (fric.y < 0.0f) {
		fric.y = 0.0f;
	}*/
	friction = fric;
}
GLboolean GameObject::relocate(glm::vec2 loc) {
	position = loc;
	return true;
}
void GameObject::move(GLfloat dt){
	if (abs(velocity.x) < 0.01f) {
		velocity.x = 0.0f;
	}
	if (abs(velocity.y) < 0.01f) {
		velocity.y = 0.0f;
	}
	if (velocity.x >= friction.x * dt) {
		velocity.x -= friction.x *dt;
	}
	else if (velocity.x <= -friction.x * dt) {
		velocity.x += friction.x *dt;
	}
	else {
		velocity.x = 0;
	}
	if (velocity.y >= friction.y * dt) {
		velocity.y -= friction.y *dt;
	}
	else if (velocity.y <= -friction.y * dt) {
		velocity.y += friction.y *dt;
	}
	else {
		velocity.y = 0;
	}
	
	velocity += (acceleration * dt);
	GLfloat speed = abs(sqrt(pow(velocity.x, 2) + pow(velocity.y, 2)));
	if (speed > 200) {
		
		velocity = glm::vec2(200 * velocity.x/speed, 200 * velocity.y/speed);
	}
	position += (velocity * dt);
	std::cout << "Delta Time: " << dt <<
		"\tPostiton: (" << position.x << ", " << position.y <<
		")\tVelocity: (" << velocity.x << "," << velocity.y <<
		")\tAcceleration: (" << acceleration.x << "," << acceleration.y <<
		")"<< std::endl;
}
void GameObject::draw(SpriteRenderer &renderer) {
	renderer.drawSprite(sprite, position, size, rotation, color);
}
GLboolean GameObject::collide(GameObject &obj) { ///Two axis collision
	bool colX = position.x + size.x >= obj.position.x && obj.position.x + obj.size.x >= position.x;
	bool colY = position.y + size.y >= obj.position.y && obj.position.y + obj.size.y >= position.y;
	return colX && colY;
}