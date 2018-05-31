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
	GameObject* initRect(glm::vec2 pos, glm::vec2 size);
	virtual void draw(SpriteRenderer &renderer);

};