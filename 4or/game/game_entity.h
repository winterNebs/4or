#pragma once 

#include "game_object.h"

class GameEntity : public GameObject {
public:
	GameEntity();
	GameEntity(glm::vec2 pos, glm::vec2 size, Texture2D sprt, glm::vec3 color = glm::vec3(1.0f), glm::vec2 vel = glm::vec2(0.0f, 0.0f));

	virtual GLboolean collide(GameObject &obj) override;
	virtual void move(GLfloat dt) override;
};