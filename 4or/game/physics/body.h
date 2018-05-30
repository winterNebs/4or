#pragma once
#include "constants.h"

#include "shape.h"

#include <glm/glm.hpp>


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
	float im; // inverse masee

	float staticFriction;
	float dynamicFriction;
	float restitution;

	// Shape interface
	Shape *shape;

	// Store a color in RGB format
	float r, g, b;

	Body(Shape *shape, float x, float y);

	void applyForce(const glm::vec2& f);
	void applyImpulse(const glm::vec2& impulse, const glm::vec2& contactVector);
	void setStatic(void);
	void setOrient(float radians);
};