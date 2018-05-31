#include "game_object.h"
#include <iostream>
GameObject::GameObject(Texture2D sp) : sprite(sp) {
	
}
GameObject* GameObject::initRect(glm::vec2 pos, glm::vec2 size) {
	PolyG poly;
	glm::vec2 *verticies = new glm::vec2[4];
	verticies[0] = pos;
	verticies[1] = glm::vec2(pos.x + size.x, pos.y);
	verticies[2] = pos + size;
	verticies[3] = glm::vec2(pos.x + pos.y + size.y);

	poly.set(verticies, 4);
	body = new Body(&poly, pos.x, pos.y);
	delete[] verticies;
	return this;
}

void GameObject::draw(SpriteRenderer &renderer) {
	renderer.drawSprite(sprite, body->position, size, body->orient, body->color);
	std::cout << body->position.x << ", " << body->position.y << std::endl;
}
///Get/Set Land
