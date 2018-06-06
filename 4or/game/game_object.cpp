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

void GamePlayer::update() {
	GameEntity::update();
}
void GameObject::move() {}
void GameEntity::move(DIR dir, float force) {
	switch (dir) {
		case DIR::left:
			body->applyForce(glm::vec2(-force,0));
			return;
		case DIR::right:
			body->applyForce(glm::vec2(force, 0));
			return;
		case DIR::up:
			body->applyForce(glm::vec2(0, -force));
			return;
		case DIR::down:
			body->applyForce(glm::vec2(0, force));
		}
	
	
}
void GamePlayer::move(DIR dir) {
	
	if (abs(body->velocity.x) < 500.0f && (dir == DIR::left || dir == DIR::right)) {
		GameEntity::move(dir, 10*PS);
		return;
	}
	if (isColliding) {
		switch (dir) {
		case DIR::up:
			GameEntity::move(dir, PS * 100);
			return;
		}
	}
}
void GameEnemy::move() {
	//std::cout << movecounter << std::endl;
	if (movecounter > 0) {
		--movecounter;
		GameEntity::move(DIR::left, -50.0f);
	}
	else if (movecounter < 0) {
		++movecounter;
		GameEntity::move(DIR::right, -50.0f);
	}
	else {
		movecounter = rand() % 300 - 150;
	}
}
GameEntity::GameEntity(Texture2D sp) : GameObject(sp) {}
GameEntity::GameEntity(Texture2D sp, glm::vec2 s, glm::vec2 pos, float mass, float res, float df, float sf) : GameObject(sp, s, pos, mass, res, df, sf) {}
GamePlayer::GamePlayer(Texture2D sp) : GameEntity(sp) {}
GamePlayer::GamePlayer(Texture2D sp, glm::vec2 s, glm::vec2 pos, float mass, float res, float df, float sf) : GameEntity(sp, s, pos, mass, res, df, sf) {}
GameEnemy::GameEnemy(Texture2D sp) : GameEntity(sp) {}
GameEnemy::GameEnemy(Texture2D sp, glm::vec2 s, glm::vec2 pos, float mass, float res, float df, float sf) : GameEntity(sp, s, pos, mass, res, df, sf) {}
