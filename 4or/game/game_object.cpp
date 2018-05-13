#include "game_object.h"

GameObject::GameObject() :
	position(0, 0), size(1, 1), velocity(0.0f), color(1.0f), rotation(0.0f), sprite(), isSolid(false), destroyed(false) {

}
GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprt, glm::vec3 color, glm::vec2 vel):
	position(pos), size(size), velocity(vel), color(color), rotation(0.0f), sprite(sprt), isSolid(false), destroyed(false) {

}

void GameObject::draw(SpriteRenderer &renderer) {
	renderer.drawSprite(sprite, position, size, rotation, color);
}