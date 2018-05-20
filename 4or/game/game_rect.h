#pragma once

#include "game_object.h"

class GameRect : public GameObject {
	GameRect();
	GameRect(glm::vec2 pos, glm::vec2 size, Texture2D sprt, glm::vec3 color = glm::vec3(1.0f), glm::vec2 vel = glm::vec2(0.0f, 0.0f));
	virtual GLboolean collide(GameObject* obj, const GLfloat dt) override;
	virtual void move(const GLfloat dt) override;
	virtual void draw(SpriteRenderer &renderer) override;
	virtual glm::vec2 interpolate(const GLfloat dt) override;

};