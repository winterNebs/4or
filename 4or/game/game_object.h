#pragma once

#include "../resources/texture.h"
#include "../resources/sprite_renderer.h"
#include "physics/headerSpaghetti.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

class GameObject {
public:
	Body* body;
	Texture2D sprite;
	glm::vec2 size;

	GameObject(Texture2D sp);
	GameObject(Texture2D sp, glm::vec2 s, glm::vec2 pos = glm::vec2(0, 0), float mass = 0.001, float res = 0.2f , float df = 0.2f, float sf = 0.4f);
	GameObject* initRect(glm::vec2 pos, glm::vec2 s);
	virtual void draw(SpriteRenderer &renderer);

};