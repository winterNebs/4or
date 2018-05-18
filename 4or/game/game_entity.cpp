#include "game_entity.h"

#include <iostream>

GameEntity::GameEntity() {

}
GameEntity::GameEntity(glm::vec2 pos, glm::vec2 size, Texture2D sprt, glm::vec3 color, glm::vec2 vel) :
	GameObject::GameObject(pos, size, sprt, false, color, vel) {
}

void GameEntity::move(GLfloat dt) {
	if (abs(velocity.x) < 0.01f) {
		velocity.x = 0.0f;
	}
	if (abs(velocity.y) < 0.01f) {
		velocity.y = 0.0f;
	}
	if (velocity.x >= friction.x * dt) {
		velocity.x -= friction.x *dt;
	}
	else if (velocity.x <= -friction.x * dt) {
		velocity.x += friction.x *dt;
	}
	else {
		velocity.x = 0;
	}
	if (velocity.y >= friction.y * dt) {
		velocity.y -= friction.y *dt;
	}
	else if (velocity.y <= -friction.y * dt) {
		velocity.y += friction.y *dt;
	}
	else {
		velocity.y = 0;
	}
	velocity += (acceleration * dt);
	GLfloat speed = sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));
	if (speed > 200) {
		velocity = glm::vec2(200 * velocity.x / speed, 200 * velocity.y / speed);
	}
	position += velocity * dt;
	/*std::cout << "Delta Time: " << dt <<
	"\tPostiton: (" << position.x << ", " << position.y <<
	")\tVelocity: (" << velocity.x << "," << velocity.y <<
	")\tAcceleration: (" << acceleration.x << "," << acceleration.y <<
	")" << std::endl;*/
}

GLboolean GameEntity::collide(GameObject* obj, GLfloat dt) { ///Two axis collision
	if (GameObject::collide(obj, dt)) {
		glm::vec2 normal = GameObject::normal(obj, dt);
		GLfloat speed = abs(sqrt(pow(velocity.x, 2) + pow(velocity.y, 2)));
		glm::vec2 reflection = glm::vec2(velocity - (2 * (glm::dot(velocity, normal)))*normal);
		velocity += glm::vec2(-reflection.x, reflection.y);
	}
	return GameObject::collide(obj, dt);
}

void GameEntity::draw(SpriteRenderer &renderer) {
	GameObject::draw(renderer);
}

glm::vec2 GameEntity::interpolate(GLfloat dt) {
	glm::vec2 vel = velocity;

	if (abs(vel.x) < 0.01f) {
		vel.x = 0.0f;
	}
	if (abs(vel.y) < 0.01f) {
		vel.y = 0.0f;
	}
	if (vel.x >= friction.x * dt) {
		vel.x -= friction.x *dt;
	}
	else if (vel.x <= -friction.x * dt) {
		vel.x += friction.x *dt;
	}
	else {
		vel.x = 0;
	}
	if (vel.y >= friction.y * dt) {
		vel.y -= friction.y *dt;
	}
	else if (vel.y <= -friction.y * dt) {
		vel.y += friction.y *dt;
	}
	else {
		vel.y = 0;
	}
	vel += (acceleration * dt);
	GLfloat speed = sqrt(pow(vel.x, 2) + pow(vel.y, 2));
	if (speed > 200) {
		vel = glm::vec2(200 * vel.x / speed, 200 * vel.y / speed);
	}
	return (position + (vel * dt));
}