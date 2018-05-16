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

}
void GameObject::draw(SpriteRenderer &renderer) {
	renderer.drawSprite(sprite, position, size, rotation, color);
}
GLboolean GameObject::collide(GameObject &obj) { ///Two axis collision
	bool colX = position.x + size.x >= obj.position.x && obj.position.x + obj.size.x >= position.x;
	bool colY = position.y + size.y >= obj.position.y && obj.position.y + obj.size.y >= position.y;
	return colX && colY;
}