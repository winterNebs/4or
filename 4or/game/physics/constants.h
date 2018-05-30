#pragma once
#include <glm/glm.hpp>
const float gravityScale = 5.0f;
const glm::vec2 gravity(0, 10.0f * gravityScale);
const float DT = 1.0f / 60.0f;
const float PI = 4 * atan(1);
const float EPSILON = 0.0001f;

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