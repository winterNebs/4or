#pragma once

#include "../resources/texture.h"
#include "../resources/sprite_renderer.h"
#include "physics/body.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

class GameObject {
public:
	Shape* shape;
	Texture2D sprite;
	glm::vec2 size;

	GameObject(Texture2D sp);
	void initRect(glm::vec2 pos, glm::vec2 size);
	virtual void draw(SpriteRenderer &renderer);

};