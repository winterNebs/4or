#pragma 
#include "constants.h"

#include "body.h"
#include "collision.h"

struct Body;

struct Manifold {
	Body* A;
	Body* B;

	float penetration;     // Depth of penetration from collision
	glm::vec2 normal;          // From A to B
	glm::vec2 contacts[2];     // Points of contact during collision
	int contact_count; // Number of contacts that occured during collision
	float e;               // Mixed restitution
	float df;              // Mixed dynamic friction
	float sf;              // Mixed static friction
	Manifold(Body* a, Body* b);

	void solve(void);                 // Generate contact information
	void init(void);            // Precalculations for impulse solving
	void applyImpulse(void);          // Solve impulse and apply
	void positionCorrection(void);  // Naive correction of positional penetration
	void infiniteMassCorrection(void);
};