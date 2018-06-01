#pragma once

#include <glm/glm.hpp>
struct Shape;

struct Body {
	glm::vec2 position;
	glm::vec2 velocity;

	float angularVelocity;
	float torque;
	float orient; // radians

	glm::vec2 force;

	// Set by shape
	float I;  // moment of inertia
	float iI; // inverse inertia
	float m;  // mass
	float im; // inverse mass

	float staticFriction;
	float dynamicFriction;
	float restitution;

	// Shape interface
	Shape* shape;

	// Store a color in RGB format
	glm::vec3 color;

	Body(Shape *shape, float x, float y);

	void applyForce(const glm::vec2& f);
	void applyImpulse(const glm::vec2& impulse, const glm::vec2& contactVector);
	void setStatic(void);
	void setOrient(float radians);
	void setMass(float mass);
};