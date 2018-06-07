#pragma once

#include "body.h"
#include "shape.h"
#include "collision.h"
#include "manifold.h"

#include <glm/glm.hpp>
const float gravityScale = 10.0f;
const glm::vec2 gravity(0, 100.0f * gravityScale);
const float DT = 1.0f / 144.0f;
const float PI = (float)(4.0f * atan(1));
const float EPSILON = 0.0001f;
const float PS = 100.0f;
const float ES = 250.0f;
inline float crossProduct(const glm::vec2& a, const glm::vec2& b) {
	return a.x * b.y - a.y * b.x;
}

inline glm::vec2 crossProduct(const glm::vec2& a, float s) {
	return glm::vec2(s * a.y, -s * a.x);
}

inline glm::vec2 crossProduct(float s, const glm::vec2& a) {
	return glm::vec2(-s * a.y, s * a.x);
}

inline float len2(glm::vec2 v) {
	return (v.x * v.x + v.y * v.y);
}
inline float dist2(const glm::vec2& a, const glm::vec2& b) {
	glm::vec2 c = a - b;
	return glm::dot(c, c);
}
inline bool biasGreaterThan(float a, float b) {
	const float k_biasRelative = 0.95f;
	const float k_biasAbsolute = 0.01f;
	return a >= b * k_biasRelative + a * k_biasAbsolute;
}
