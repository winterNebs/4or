
#include "shape.h"
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

const float PI = (float)(4.0f * atan(1));
Circle::Circle(float r) {
	radius = r;
}
Shape* Circle::clone(void) const {
	return new Circle(radius);
}
void Circle::init(void) {
	computeMass(1.0f);
}
void Circle::computeMass(float density) {
	body->m = PI * radius * radius * density;
	body->im = (body->m) ? 1.0f / body->m : 0.0f;
	body->I = body->m * radius * radius;
	body->iI = (body->I) ? 1.0f / body->I : 0.0f;
}
Shape::Type Circle::getType(void) const {
	return Type::CIRCLE;
}
void PolyG::init(void) {
	computeMass(1.0f);
}
Shape* PolyG::clone(void) const {
	PolyG* poly = new PolyG();
	poly->u = u;
	poly->body = body;
	for (int i = 0; i < m_vertexCount; i++) {
		poly->m_vertices[i] = m_vertices[i];
		poly->m_normals[i] = m_normals[i];
	}
	poly->m_vertexCount = m_vertexCount;
	return poly;
}

void PolyG::computeMass(float density) {
	glm::vec2 c(0);
	float area = 0.0f;
	float I = 0.0f;
	const float k_inv3 = 1.0f / 3.0f;

	for (int i1 = 0; i1 < m_vertexCount; i1++) {
		glm::vec2 p1(m_vertices[i1]);
		int i2 = i1 + 1 < m_vertexCount ? i1 + 1 : 0;
		glm::vec2 p2(m_vertices[i2]);

		float D = crossProduct(p1, p2);
		float triangleArea = 0.5f * D;

		area += triangleArea;
		// Use area to weight the centroid average, not just vertex position
		c += triangleArea * k_inv3 * (p1 + p2);

		float intx2 = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
		float inty2 = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
		I += (0.25f * k_inv3 * D) * (intx2 + inty2);
	}
	c *= 1.0f / area;

	for (int i = 0; i < m_vertexCount; ++i) {
		m_vertices[i] -= c;
	}

	body->m = density * area;
	body->im = (body->m) ? 1.0f / body->m : 0.0f;
	body->I = I * density;
	body->iI = body->I ? 1.0f / body->I : 0.0f;
}
void PolyG::setOrient(float radians) {
	float c = cos(radians);
	float s = sin(radians);
	u = glm::mat2(
		glm::vec2(c, -s),
		glm::vec2(s, c)
	);
}
Shape::Type PolyG::getType(void) const {
	return Type::POLY;
}
void PolyG::setBox(float hw, float hh) {
	m_vertexCount = 4;
	m_vertices[0] = glm::vec2(-hw, -hh);
	m_vertices[1] = glm::vec2(hw, -hh);
	m_vertices[2] = glm::vec2(hw, hh);
	m_vertices[3] = glm::vec2(-hw, hh);
	m_normals[0] = glm::vec2(0.0f, -1.0f);
	m_normals[1] = glm::vec2(1.0f, 0.0f);
	m_normals[2] = glm::vec2(0.0f, 1.0f);
	m_normals[3] = glm::vec2(-1.0f, 0.0f);
}

void PolyG::set(glm::vec2 *vertices, int count) {
	// No hulls with less than 3 vertices (ensure actual polygon)
	assert(count > 2 && count <= MaxPolyVertexCount);
	count = glm::min(count, MaxPolyVertexCount);

	// Find the right most point on the hull
	int rightMost = 0;
	float highestXCoord = vertices[0].x;
	for (int i = 1; i < count; ++i) {
		float x = vertices[i].x;
		if (x > highestXCoord) {
			highestXCoord = x;
			rightMost = i;
		}

		// If matching x then take farthest negative y
		else if (x == highestXCoord) {
			if (vertices[i].y < vertices[rightMost].y) {
				rightMost = i;
			}
		}
	}

	int hull[MaxPolyVertexCount];
	int outCount = 0;
	int indexHull = rightMost;

	for (;;) {
		hull[outCount] = indexHull;

		// Search for next index that wraps around the hull
		// by computing cross products to find the most counter-clockwise
		// vertex in the set, given the previos hull index
		int nextHullIndex = 0;
		for (int i = 1; i < count; ++i)
		{
			// Skip if same coordinate as we need three unique
			// points in the set to perform a cross product
			if (nextHullIndex == indexHull)
			{
				nextHullIndex = i;
				continue;
			}

			// Cross every set of three unique vertices
			// Record each counter clockwise third vertex and add
			// to the output hull
			// See : http://www.oocities.org/pcgpe/math2d.html
			glm::vec2 e1 = vertices[nextHullIndex] - vertices[hull[outCount]];
			glm::vec2 e2 = vertices[i] - vertices[hull[outCount]];
			float c = crossProduct(e1, e2);
			if (c < 0.0f) {
				nextHullIndex = i;
			}

			// Cross product is zero then e vectors are on same line
			// therefor want to record vertex farthest along that line
			if (c == 0.0f && len2(e2) > len2(e1))
				nextHullIndex = i;
		}

		++outCount;
		indexHull = nextHullIndex;

		// Conclude algorithm upon wrap-around
		if (nextHullIndex == rightMost)
		{
			m_vertexCount = outCount;
			break;
		}
	}

	// Copy vertices into shape's vertices
	for (int i = 0; i < m_vertexCount; ++i) {
		m_vertices[i] = vertices[hull[i]];
	}

	// Compute face normals
	for (int i1 = 0; i1 < m_vertexCount; ++i1) {
		int i2 = i1 + 1 < m_vertexCount ? i1 + 1 : 0;
		glm::vec2 face = m_vertices[i2] - m_vertices[i1];

		// Ensure no zero-length edges, because that's bad
		assert(len2(face) > EPSILON * EPSILON);

		// Calculate normal with 2D cross product between vector and scalar
		m_normals[i1] = glm::vec2(face.y, -face.x);
		m_normals[i1] = glm::normalize(m_normals[i1]);

	}
}

glm::vec2 PolyG::getSupport(const glm::vec2& dir) {
	float bestProjection = -FLT_MAX;
	glm::vec2 bestVertex;

	for (int i = 0; i < m_vertexCount; ++i)	{
		glm::vec2 v = m_vertices[i];
		float projection = glm::dot(v, dir);

		if (projection > bestProjection) {
			bestVertex = v;
			bestProjection = projection;
		}
	}

	return bestVertex;
}