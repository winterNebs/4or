#pragma once

#include "body.h"

#include <cassert>

#define MaxPolyVertexCount 64
struct Body;
struct Shape {
	enum Type {
		CIRCLE, POLY, COUNT
	};

	Body* body;

	float radius;
	
	glm::mat2 u;

	Shape() {}

	virtual Shape* clone(void) const = 0;
	virtual void init(void) = 0;
	virtual void computeMass(float density) = 0;
	virtual void setOrient(float radians) = 0;
	virtual Type getType(void) const = 0;
};

struct Circle : public Shape {
	Circle(float r);
	Shape* clone(void) const;
	void init(void);
	void computeMass(float density);
	void setOrient(float radians) {}
	Type getType(void) const;
};

struct PolyG : public Shape {
	int m_vertexCount;
	glm::vec2 m_vertices[MaxPolyVertexCount];
	glm::vec2 m_normals[MaxPolyVertexCount];

	void init(void);
	Shape* clone(void) const;

	void computeMass(float density);
	void setOrient(float radians);
	Type getType(void) const;
	void setBox(float hw, float hh);

	void set(glm::vec2 *vertices, int count);
	glm::vec2 getSupport(const glm::vec2& dir);

};