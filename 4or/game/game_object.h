#pragma once

#include "../resources/texture.h"
#include "../resources/sprite_renderer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

class GameObject {
public:
	glm::vec3 color;
	glm::vec2 acceleration, appliedF, normalF, gravity;
	GLfloat rotation;
	GLboolean isSolid, isStatic;
	GLboolean onGround;

	Texture2D sprite;

	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprt, bool s, glm::vec3 color = glm::vec3(1.0f), glm::vec2 vel = glm::vec2(0.0f, 0.0f));
	virtual void draw(SpriteRenderer &renderer);
	virtual GLboolean collide(GameObject* obj, GLfloat dt);
	virtual void move(GLfloat dt);
	virtual glm::vec2 interpolate(GLfloat dt);
	virtual glm::vec2 normal(GameObject* obj, GLfloat dt);

	glm::vec2 getPos() const;
	glm::vec2 getSize() const;
	glm::vec2 getVel() const;
	glm::vec2 getFriction() const;

	GLboolean relocate(glm::vec2 loc);
	void setFriction(glm::vec2 fric);

	GLfloat getCloseDist(GameObject* obj);
	GLfloat calcTime(GLfloat dist);
protected:
	glm::vec2 position, size, velocity, friction;
	GLfloat calcDist(glm::vec2 dir, glm::vec2 point, glm::vec2 line1, glm::vec2 line2);
	virtual std::vector<glm::vec2> getVerticies();
};