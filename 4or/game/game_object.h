
#pragma once

#include "../resources/texture.h"
#include "../resources/sprite_renderer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

class GameObject {
public:
	glm::vec3 color;
	glm::vec2 gravity, appliedF;
	GLfloat rotation;
	GLboolean isSolid, isStatic;

	Texture2D sprite;

	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprt, bool s, glm::vec3 color = glm::vec3(1.0f), glm::vec2 vel = glm::vec2(0.0f, 0.0f));
	virtual void draw(SpriteRenderer &renderer);
	virtual GLboolean collide(GameObject* obj, const GLfloat dt);
	virtual void move(const GLfloat dt);
	virtual glm::vec2 interpolate(const GLfloat dt);
	virtual glm::vec2 normal(GameObject* obj, const GLfloat dt);

	glm::vec2 getPos() const;
	glm::vec2 getSize() const;
	glm::vec2 getVel() const;
	glm::vec2 getFriction() const;
	glm::vec2 getAccel() const;
	glm::vec2 getSize() const;

	GLboolean relocate(const glm::vec2 loc);
	void setFriction(const glm::vec2 fric);
protected:
	glm::vec2 position, size, velocity, friction, acceleration, normalF;
	GLfloat calcFastest(GameObject* obj);
	GLfloat calcTime(const glm::vec2 dir, const  glm::vec2 point, const glm::vec2 line1, const glm::vec2 line2);
	virtual std::vector<glm::vec2> getVerticies();
};