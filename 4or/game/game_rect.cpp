#include "game_rect.h"
#include <iostream>

GameRect::GameRect() {

}
GameRect::GameRect(glm::vec2 pos, glm::vec2 size, Texture2D sprt, glm::vec3 color, glm::vec2 vel) :
	GameObject::GameObject(pos, size, sprt, false, color, vel) {
	gravity = glm::vec2(0, 500);
	appliedF = glm::vec2(0, 0);
	normalF = glm::vec2(0, 0);
}

void GameRect::move(const GLfloat dt) {
	if (!isStatic) {
		/*///SIMPLIFY
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
		acceleration = normalF + gravity + appliedF;
		velocity += (acceleration)* dt;
		//acceleration = glm::vec2(0, 0);
		//GLfloat speed = sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));
		//if (speed > 200) {
		//	velocity = glm::vec2(200 * velocity.x / speed, 200 * velocity.y / speed);
		//}
		position += velocity * dt;
		//std::cout << gravity.x << gravity.y << std::endl;
		/*std::cout <<
		//"Delta Time: " << dt <<
		//"\tPostiton: (" << position.x << "," << position.y <<
		")\tVelocity: (" << velocity.x << "," << velocity.y <<
		")\tAcceleration: (" << acceleration.x << "," << acceleration.y <<
		")\tApplied Force: (" << appliedF.x << "," << appliedF.y <<
		")\tNormal Force: (" << normalF.x << "," << normalF.y <<
		")\tGravity F: (" << gravity.x << "," << gravity.y <<
		std::endl;*/
		position += velocity * dt;
	}
}
void pointRectInt(glm::vec2 point, std::vector<glm::vec2> rect) {
	glm::vec2 diagonal = (rect[0]-rect[1]) + (rect[1] - rect[2])l
}
GLboolean GameRect::collide(GameObject* obj, const GLfloat dt) { ///Two axis collision

	glm::vec2 interper = interpolate(dt);
	glm::vec2 objPos = obj->interpolate(dt);
	if (collide) {

		glm::vec2 normal = GameObject::normal(obj, dt);
		glm::vec2 reflection = glm::vec2(velocity - (2 * (glm::dot(velocity, normal)))*normal);
		glm::vec2 normalForce = glm::vec2((gravity + appliedF) - (2 * (glm::dot((gravity + appliedF), normal)))*normal);
		//std::cout << normal.x << "," << normal.y << std::endl;
		velocity += glm::vec2(-reflection.x, reflection.y);
		normalF = glm::vec2(-normalForce.x, normalForce.y);
	}
	else {
		normalF = glm::vec2(0, 0);
	}
	//Do reclacl
	return GameObject::collide(obj, dt);
}

void GameRect::draw(SpriteRenderer &renderer) {
	GameObject::draw(renderer);
}

glm::vec2 GameRect::interpolate(const GLfloat dt) {
	/*
	glm::vec2 vel = velocity;
	glm::vec2 acc = acceleration;

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
	acc = gravity + appliedF + normalF;
	vel += (acc)* dt;
	//GLfloat speed = sqrt(pow(vel.x, 2) + pow(vel.y, 2));
	//if (speed > 200) {
	//	vel = glm::vec2(200 * vel.x / speed, 200 * vel.y / speed);
	//}
	*/
	return (position + (velocity * dt));
}