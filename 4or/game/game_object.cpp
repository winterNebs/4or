#include "game_object.h"
#include <iostream>
GameObject::GameObject(Texture2D sp) : sprite(sp) {}	//Game object constructor

GameObject::GameObject(Texture2D sp, glm::vec2 s, glm::vec2 pos, float mass, float res, float df, float sf) 
	: sprite(sp) {	//More advanced constructor, initializes the object as a rectangle, sets mass etc.
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
	body->setOrient(0);		//Rotation is disabled for now
}
GameObject* GameObject::initRect(glm::vec2 pos, glm::vec2 s){	//Uses size to create a rectangle with top left being (0,0)
	size = s;	
	
	PolyG poly;
	glm::vec2 *verticies = new glm::vec2[4];

	verticies[0] = glm::vec2(0);
	verticies[1] = glm::vec2(s.x, 0);
	verticies[2] = s;
	verticies[3] = glm::vec2(0, s.y);
	
	poly.set(verticies, 4);
	body = new Body(&poly, pos.x, pos.y);
	body->position = pos;						// Also sets position
	delete[] verticies;
	return this;
}

void GameObject::update() {
	body->colliding = false;		//Resets the colliding attribute
}
void GameEntity::update() {			//Updates colliding for entity
	isColliding = body->colliding;
	GameObject::update();
}
void GameObject::draw(SpriteRenderer &renderer) {		//Render object
	renderer.drawSprite(sprite, body->position, size, body->orient, body->color);
}

void GamePlayer::update() {
	GameEntity::update();
}
void GameObject::move() {}
void GameEntity::move(DIR dir, float force) {		//Moves in direction with force
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
void GamePlayer::move(DIR dir) {			//Moves in direction based on input (left, right, jump)
	const float speedcap = 500.0f;
	if (body->velocity.x > -speedcap) {		//Impose a speed cap on the player
		if (dir == DIR::left) {
			GameEntity::move(dir, PS);
			return;
		}
	}
	if (body->velocity.x < speedcap) {
		if (dir == DIR::right) {
			GameEntity::move(dir, PS);
			return;
		}
	}
	if (isColliding) {						//Only allow jumping when in contact with a surface
		switch (dir) {
		case DIR::up:
			GameEntity::move(dir, PS * 10);
			return;
		}
	}
}
void GameEnemy::move() {					//Random movement for enemies (currently disabled)
	if (movecounter > 0) {
		--movecounter;
		GameEntity::move(DIR::left, ES);
	}
	else if (movecounter < 0) {
		++movecounter;
		GameEntity::move(DIR::right, ES);
	}
	else {
		movecounter = rand() % 100 - 50;
	}
}

void GameExit::update() {				//Update exit, (if collided by player end game)
	gameEnd = body->colliding || gameEnd;
	GameObject::update();
}
//Constructors
GameEntity::GameEntity(Texture2D sp) : GameObject(sp) {}
GameEntity::GameEntity(Texture2D sp, glm::vec2 s, glm::vec2 pos, float mass, float res, float df, float sf) : GameObject(sp, s, pos, mass, res, df, sf) {}
GamePlayer::GamePlayer(Texture2D sp) : GameEntity(sp) {}
GamePlayer::GamePlayer(Texture2D sp, glm::vec2 s, glm::vec2 pos, float mass, float res, float df, float sf) : GameEntity(sp, s, pos, mass, res, df, sf) {}
GameEnemy::GameEnemy(Texture2D sp) : GameEntity(sp) {}
GameEnemy::GameEnemy(Texture2D sp, glm::vec2 s, glm::vec2 pos, float mass, float res, float df, float sf) : GameEntity(sp, s, pos, mass, res, df, sf) {}
GameExit::GameExit(Texture2D sp, glm::vec2 s, glm::vec2 pos) : GameObject(sp, s, pos, 0, 0, 0, 0) {}
