#include "game_object.h"

#include <iostream>
#include <math.h>
#include <fstream>
GameObject::GameObject() :
	position(0, 0), size(1, 1), velocity(0.0f), color(1.0f), rotation(0.0f), sprite(), 
	isSolid(false), staticFriction(0.1f), dynamicFriction(0.3f){

}
GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprt, float m, glm::vec3 color) :
	position(pos), size(size), color(color), rotation(0.0f), sprite(sprt),
	isSolid(false), mass_data(m), staticFriction(0.1f), dynamicFriction(0.3f) {
}
AABB GameObject::computeAABB() {
	AABB aabb;
	aabb.min = position;
	aabb.max = position + size;
	return aabb;
}
std::vector<glm::vec2> GameObject::getVerticies() {
	std::vector<glm::vec2> verts;
	verts.push_back(glm::vec2(position.x, position.y));
	verts.push_back(glm::vec2(position.x + size.x, position.y));
	verts.push_back(glm::vec2(position.x + size.x, position.y + size.y));
	verts.push_back(glm::vec2(position.x, position.y + size.y));
	return verts;
}

glm::vec2 GameObject::interpolate(GLfloat dt) {
	return position;
}
void GameObject::draw(SpriteRenderer &renderer) {
	renderer.drawSprite(sprite, position, size, rotation, color);
	std::cout << position.x << ", " << position.y << std::endl;
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

GLboolean GameObject::relocate(glm::vec2 loc) {
	position = loc;
	return true;
}