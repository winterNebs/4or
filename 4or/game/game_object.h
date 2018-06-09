#pragma once

#include "../resources/texture.h"
#include "../resources/sprite_renderer.h"
#include "physics/headerSpaghetti.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
enum class DIR {//Direction enumerated types
	up, down, left, right
};
class GameObject {		//Game object, base for any interactive thing in the game
public:
	Body* body;			//Physics body
	Texture2D sprite;
	glm::vec2 size;
	std::vector<Texture2D> animation;
	GameObject(Texture2D sp);
	GameObject(Texture2D sp, glm::vec2 s, glm::vec2 pos = glm::vec2(0, 0), float mass = 0.001, float res = 0.2f , float df = 0.2f, float sf = 0.4f);
	GameObject* initRect(glm::vec2 pos, glm::vec2 s);
	virtual void draw(SpriteRenderer &renderer);
	virtual void update();
	virtual void move();
};

class GameEntity : public GameObject {		//Entities are moving game objects
public:
	bool isColliding;
	GameEntity(Texture2D sp);
	GameEntity(Texture2D sp, glm::vec2 s, glm::vec2 pos = glm::vec2(0, 0), float mass = 0.001, float res = 0.2f, float df = 0.2f, float sf = 0.4f);
	void update() override;
	virtual void move(DIR dir, float force);
	
};
class GamePlayer : public GameEntity {		//Player is player
public:
	//Control schemes in the future if needed
	GamePlayer(Texture2D sp);
	GamePlayer(Texture2D sp, glm::vec2 s, glm::vec2 pos = glm::vec2(0, 0), float mass = 0.001, float res = 0.2f, float df = 0.2f, float sf = 0.4f);
	void update() override;
	void move(DIR dir);
};
class GameEnemy : public GameEntity {		//Enemy is enemy, currently disabled
public:	
	int movecounter = 0;
	GameEnemy(Texture2D sp);
	GameEnemy(Texture2D sp, glm::vec2 s, glm::vec2 pos = glm::vec2(0, 0), float mass = 0.001, float res = 0.2f, float df = 0.2f, float sf = 0.4f);
	void move() override;
};
class GameExit : public GameObject {		//Exit is the exit to the level (win condition)
public:
	bool gameEnd = false;
	GameExit(Texture2D sp, glm::vec2 s, glm::vec2 pos = glm::vec2(0, 0));
	void update() override;
};