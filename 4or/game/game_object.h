#pragma once

#include "../resources/texture.h"
#include "../resources/sprite_renderer.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
struct AABB {
	glm::vec2 min;
	glm::vec2 max;
};
struct MassData {
	GLfloat mass;
	GLfloat invMass;
	MassData() : mass(0), invMass(0) {}
	MassData(GLfloat m) : mass(m) {
		if (m == 0) {
			invMass = 0;
		}
		else {
			invMass = 1 / mass;
		}
	}
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
	glm::vec2 force, velocity, position, size, appliedF;
	Layer layer;
	GLfloat rotation;
	MassData mass_data;
	Material mat;
	GLboolean isSolid;
	GLboolean onGround;
	GLfloat staticFriction, dynamicFriction;

	Texture2D sprite;

	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprt, float = 0, glm::vec3 color = glm::vec3(1.0f));
	virtual void draw(SpriteRenderer &renderer);
	virtual glm::vec2 interpolate(GLfloat dt);

	glm::vec2 getPos() const;
	glm::vec2 getSize() const;
	glm::vec2 getVel() const;

	GLboolean relocate(glm::vec2 loc);
	AABB computeAABB();
protected:
	virtual std::vector<glm::vec2> getVerticies();
};