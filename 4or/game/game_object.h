#pragma once

#include "../resources/texture.h"
#include "../resources/sprite_renderer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
struct MassData {
	GLfloat mass;
	GLfloat invMass;
};

struct Material {
	GLfloat density;
	GLfloat restitution;
	/*Rock       Density : 0.6  Restitution : 0.1
	Wood       Density : 0.3  Restitution : 0.2
	Metal      Density : 1.2  Restitution : 0.05
	BouncyBall Density : 0.3  Restitution : 0.8
	SuperBall  Density : 0.3  Restitution : 0.95
	Pillow     Density : 0.1  Restitution : 0.2
	Static     Density : 0.0  Restitution : 0.4*/
};
enum Layer {
	Default = 1
};
class GameObject {
public:
	glm::vec3 color;
	glm::vec2 force, velocity;
	Layer layer;
	GLfloat rotation;
	MassData mass_data;
	Material mat;
	GLboolean isSolid;
	GLboolean onGround;
	GLfloat gravity;

	Texture2D sprite;

	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprt, bool s, glm::vec3 color = glm::vec3(1.0f), glm::vec2 vel = glm::vec2(0.0f, 0.0f));
	virtual void draw(SpriteRenderer &renderer);
	virtual GLboolean collide(GameObject* obj);
	virtual void move(GLfloat dt);
	virtual glm::vec2 interpolate(GLfloat dt);
	virtual glm::vec2 normal(GameObject* obj);
	virtual void resolveCollision(GameObject* obj, GLfloat dt);
	virtual void positionCorrection(GameObject* obj);

	glm::vec2 getPos() const;
	glm::vec2 getSize() const;
	glm::vec2 getVel() const;

	GLboolean relocate(glm::vec2 loc);

	GLfloat getCloseDist(GameObject* obj);
	GLfloat calcTime(GLfloat dist);
protected:
	glm::vec2 position, size;
	GLfloat calcDist(glm::vec2 dir, glm::vec2 point, glm::vec2 line1, glm::vec2 line2);
	virtual std::vector<glm::vec2> getVerticies();
};