#include "game_object.h"
#include <iostream>
GameObject::GameObject(Texture2D sp) : sprite(sp) {
	
}

GameObject::GameObject(Texture2D sp, glm::vec2 s, glm::vec2 pos, float mass, float res, float df, float sf) 
	: sprite(sp) {
	initRect(pos, s);
	if (mass == 0) {
		body->setStatic();
	}
	else {
		body->setMass(mass);
	}
	body->restitution = res;
	body->dynamicFriction = df;
	body->staticFriction = sf;
	body->setOrient(0);
}
GameObject* GameObject::initRect(glm::vec2 pos, glm::vec2 s){
	size = s;
	
	PolyG poly;
	glm::vec2 *verticies = new glm::vec2[4];

	verticies[0] = glm::vec2(0);
	verticies[1] = glm::vec2(s.x, 0);
	verticies[2] = s;
	verticies[3] = glm::vec2(0, s.y);
	
	poly.set(verticies, 4);
	body = new Body(&poly, pos.x, pos.y);
	body->position = pos;
	delete[] verticies;
	return this;
}
void GameObject::update() {
	body->colliding = false;
}
void GameEntity::update() {
	isColliding = body->colliding;
	GameObject::update();
}
void GameObject::draw(SpriteRenderer &renderer) {
	renderer.drawSprite(sprite, body->position, size, body->orient, body->color);

	/*std::cout <<
		"Pos:" << body->position.x << "," << body->position.y << 
		"\t Size:" << size.x << "," << size.y <<  std::endl;*/
}

void GameEntity::move(DIR dir) {

}
void GamePlayer::update() {
	GameEntity::update();
}
void GamePlayer::move(DIR dir) {
	if (abs(body->velocity.x) < 10000.0f ) {
		switch (dir) {
		case DIR::left:
			body->applyForce(glm::vec2(-PS,0));
			break;
		case DIR::right:
			body->applyForce(glm::vec2(PS, 0));
			break;
		}
	}
	if (isColliding) {
		switch (dir) {
		case DIR::up:
			body->applyForce(glm::vec2(0,-PS*100));
			break;
		}
	}
}

GameEntity::GameEntity(Texture2D sp) : GameObject(sp) {}

GameEntity::GameEntity(Texture2D sp, glm::vec2 s, glm::vec2 pos, float mass, float res, float df, float sf) : GameObject(sp, s, pos, mass, res, df, sf) {}

GamePlayer::GamePlayer(Texture2D sp) : GameEntity(sp) {}

GamePlayer::GamePlayer(Texture2D sp, glm::vec2 s, glm::vec2 pos, float mass, float res, float df, float sf) : GameEntity(sp, s, pos, mass, res, df, sf) {}
