#include "game_entity.h"

#include <iostream>

GameEntity::GameEntity() {

}
GameEntity::GameEntity(glm::vec2 pos, glm::vec2 size, Texture2D sprt, glm::vec3 color = glm::vec3(1.0f), glm::vec2 vel = glm::vec2(0.0f, 0.0f)) :
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
	GLfloat speed = abs(sqrt(pow(velocity.x, 2) + pow(velocity.y, 2)));
	if (speed > 200) {

		velocity = glm::vec2(200 * velocity.x / speed, 200 * velocity.y / speed);
	}
	position += (velocity * dt);
	std::cout << "Delta Time: " << dt <<
		"\tPostiton: (" << position.x << ", " << position.y <<
		")\tVelocity: (" << velocity.x << "," << velocity.y <<
		")\tAcceleration: (" << acceleration.x << "," << acceleration.y <<
		")" << std::endl;
}

GLboolean GameEntity::collide(GameObject &obj) { ///Two axis collision
	bool colX = position.x + size.x >= obj.getPos().x && obj.getPos().x + obj.getSize().x >= getPos().x;
	bool colY = position.y + size.y >= obj.getPos().y && obj.getPos().y + obj.getSize().y >= getPos().y;
	return colX && colY;
}