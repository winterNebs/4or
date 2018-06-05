#include "headerSpaghetti.h"
#include <iostream>
void Manifold::solve(void) {
	dispatch[A->shape->getType()][B->shape->getType()](this, A, B);
}

void Manifold::init(void) {
	// Calculate average restitution
	e = glm::min(A->restitution, B->restitution);

	// Calculate static and dynamic friction
	sf = std::sqrt(A->staticFriction * A->staticFriction);
	df = std::sqrt(A->dynamicFriction * A->dynamicFriction);

	for (int i = 0; i < contact_count; ++i) {
		// Calculate radii from COM to contact
		glm::vec2 ra = contacts[i] - A->position;
		glm::vec2 rb = contacts[i] - B->position;

		glm::vec2 rv = B->velocity + crossProduct(B->angularVelocity, rb) -
			A->velocity - crossProduct(A->angularVelocity, ra);
		//glm::vec2 rv = B->velocity - A->velocity;

		// Determine if we should perform a resting collision or not
		// The idea is if the only thing moving this object is gravity,
		// then the collision should be performed without any restitution
		if (len2(rv) < len2(DT * gravity) + EPSILON) {
			e = 0.0f;
		}
	}
}

void Manifold::applyImpulse(void) {
	// Early out and positional correct if both objects have infinite mass
	if (A->im + B->im <= EPSILON) {
		infiniteMassCorrection();
		return;
	}
	for (int i = 0; i < contact_count; ++i) {
		// Calculate radii from COM to contact
		glm::vec2 ra = contacts[i] - A->position;
		glm::vec2 rb = contacts[i] - B->position;

		// Relative velocity
		//glm::vec2 rv = B->velocity - A->velocity;
		glm::vec2 rv = B->velocity + crossProduct(B->angularVelocity, rb) - A->velocity - crossProduct(A->angularVelocity, ra);

		// Relative velocity along the normal
		float contactVel = glm::dot(rv, normal);

		// Do not resolve if velocities are separating
		if (contactVel > 0) {
			return;
		}
		float raCrossN = crossProduct(ra, normal);
		float rbCrossN = crossProduct(rb, normal);
		float invMassSum = A->im + B->im + (raCrossN * raCrossN) * A->iI + (rbCrossN * rbCrossN) * B->iI;
		//float invMassSum = A->im + B->im;

		// Calculate impulse scalar
		float j = - (1.0f + e) * contactVel;
		j /= invMassSum;
		j /= (float)contact_count;

		// Apply impulse
		glm::vec2 impulse = normal * j;
		A->applyImpulse(-impulse, ra);
		B->applyImpulse(impulse, rb);

		// Friction impulse
		rv = B->velocity + crossProduct(B->angularVelocity, rb) -
			A->velocity - crossProduct(A->angularVelocity, ra);
		//rv = B->velocity -	A->velocity;

		glm::vec2 t = rv - (normal * glm::dot(rv, normal));
		t = glm::normalize(t);

		// j tangent magnitude
		float jt = -glm::dot(rv, t);
		jt /= invMassSum;
		jt /= (float)contact_count;

		// Don't apply tiny friction impulses
		if (abs(jt) <= EPSILON)
			return;

		// Coulumb's law
		glm::vec2 tangentImpulse;
		if (std::abs(jt) < j * sf) {
			tangentImpulse = t * jt;
		}
		else {
			tangentImpulse = t * -j * df;
		}
		// Apply friction impulse
		A->applyImpulse(-tangentImpulse, ra);
		B->applyImpulse(tangentImpulse, rb);
	}
}

void Manifold::positionCorrection(void) {
	const float k_slop = 0.05f; // Penetration allowance usually 0.01 to 0.1
	const float percent = 0.4f; // Penetration percentage to correct  usually 20% to 80%
	
	glm::vec2 correction = (glm::max(penetration - k_slop, 0.0f) / (A->im + B->im)) * normal * percent;
	A->position -= correction * A->im;
	B->position += correction * B->im;
	A->colliding = true;
	B->colliding = true;
	//std::cout << (correction * B->im).x << "," << (correction * B->im).y << std::endl;
}

void Manifold::infiniteMassCorrection(void) {
	A->velocity = glm::vec2(0);
	B->velocity = glm::vec2(0);
}