#include "game_entity.h"

#include <iostream>

GameEntity::GameEntity() {

}
GameEntity::GameEntity(glm::vec2 pos, glm::vec2 size, Texture2D sprt, glm::vec3 color, glm::vec2 vel) :
	GameObject::GameObject(pos, size, sprt, false, color, vel) {
	appliedF = glm::vec2(0);
	normalF = glm::vec2(0);
	gravity = glm::vec2(0, 400.0f);
	onGround = false;
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
	acceleration = normalF + appliedF;
	if (!onGround) {
		acceleration += gravity;
	}
	velocity += (acceleration * dt);
	
	position += velocity * dt;
	/*
	std::cout <<
		//"Delta Time: " << dt <<
		"\tPostiton: (" << position.x << ", " << position.y <<
		")\tVelocity: (" << velocity.x << "," << velocity.y <<
		")\tAcceleration: (" << acceleration.x << "," << acceleration.y <<
		")\tApplied: (" << appliedF.x << "," << appliedF.y <<
		")\tNormal: (" << normalF.x << "," << normalF.y <<
		std::endl;*/
}
//static int counter = 0;
GLboolean GameEntity::collide(GameObject* obj, GLfloat dt) { ///Two axis collision
	//if (counter == 1) {
		//int i = 0;
	//}
	GLboolean col = GameObject::collide(obj, dt);
	if (col) {
		//counter++;
		//move(dt-calcTime(getCloseDist(obj)));
		//move(getFastestTime(obj)-dt);
		
		glm::vec2 normal = GameObject::normal(obj, dt);
		if (normal.x == 0 && normal.y == 0) {
			//normal = glm::vec2(1.0f);
		}
		if (normal.y > 0) {
			onGround = true;
			//std::cout << "grounded" << std::endl;
		}
		//GLfloat speed = abs(sqrt(pow(velocity.x, 2) + pow(velocity.y, 2)));
		//glm::vec2 reflection = glm::vec2(velocity - (2 * (glm::dot(velocity, normal)))*normal);
		velocity = glm::vec2(velocity.x * normal.y, velocity.y * normal.x);
		//velocity += reflection ;
		//velocity *= -glm::vec2(normal.y, normal.x); ///Bad but whatever
		//velocity -= glm::vec2(velocity.x * normal.y, velocity.y * normal.x);
		glm::vec2 normF;
		if(onGround){
			normF = glm::vec2((appliedF) - (2 * (glm::dot((appliedF), normal)))*normal);
		}
		else {
			normF = glm::vec2((appliedF + gravity) - (2 * (glm::dot((appliedF + gravity), normal)))*normal);
		}
		normalF += glm::vec2(normF.x*normal.x, normF.y*normal.y); 
		position -= normal * 0.01f;
		
		std::cout <<
			//"Delta Time: " << dt <<
			"\tPostiton: (" << position.x << ", " << position.y <<
			")\tVelocity: (" << velocity.x << "," << velocity.y <<
			")\tAcceleration: (" << acceleration.x << "," << acceleration.y <<
			")\tNormal: (" << normal.x << "," << normal.y <<
			")\tAppliedF: (" << appliedF.x << "," << appliedF.y <<
			")\tNormalF: (" << normalF.x << "," << normalF.y <<
			")" <<
			std::endl; 
	}
	else {
		normalF = glm::vec2(0);
	}
	return col;
}

void GameEntity::draw(SpriteRenderer &renderer) {
	GameObject::draw(renderer);
}

glm::vec2 GameEntity::interpolate(GLfloat dt) {
	glm::vec2 vel = velocity;
	glm::vec2 acc = acceleration;
	if (abs(vel.x) < 0.01f) {
		vel.x = 0.0f;
	}
	if (abs(vel.y) < 0.01f) {
		vel.y = 0.0f;
	}
	if (vel.x >= friction.x) {
		vel.x -= friction.x;
	}
	else if (vel.x <= -friction.x) {
		vel.x += friction.x;
	}
	else {
		vel.x = 0;
	}
	if (vel.y >= friction.y) {
		vel.y -= friction.y;
	}
	else if (vel.y <= -friction.y) {
		vel.y += friction.y;
	}
	else {
		vel.y = 0;
	}
	acc = normalF + appliedF;
	if (!onGround) {
		acc += gravity;
	}
	vel += (acc * dt);
	
	return (position + (vel * dt));
}
