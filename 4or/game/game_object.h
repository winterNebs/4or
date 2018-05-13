#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../resources/texture.h"
#include "../resources/sprite_renderer.h"

class GameObject {
public:
	glm::vec3 color;
	glm::vec2 acceleration;
	GLfloat rotation;
	GLboolean isSolid, destroyed;

	Texture2D sprite;

	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprt, glm::vec3 color = glm::vec3(1.0f), glm::vec2 vel = glm::vec2(0.0f, 0.0f));
	virtual void draw(SpriteRenderer &renderer);
	virtual GLboolean collide(GameObject &obj);
	virtual void move(GLfloat dt);

	glm::vec2 getPos() const;
	glm::vec2 getSize() const;
	glm::vec2 getVel() const;
	glm::vec2 getFriction() const;

	GLboolean relocate(glm::vec2 loc);
	void setFriction(glm::vec2 fric);
private:
	glm::vec2 position, size, velocity, friction;
};

#endif
