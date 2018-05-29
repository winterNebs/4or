#include "game_level.h"

GameLevel::GameLevel(std::string f) {
	load(f);
}
void GameLevel::load(std::string file) {
	objects.clear();
	svgReader test = svgReader(file);
	init(test.getShapes());
}
void GameLevel::init(std::vector<shape*> blockData) {
	///Calculate max size;
	for (auto i : blockData) {
		GameObject* obj = new GameObject(glm::vec2(i->x, i->y), glm::vec2(i->width, i->height),
			ResourceManager::getTexture("container"), 0, glm::vec3(0.4f, 0.4f, 0.4f));

		obj->isSolid = GL_TRUE;
		obj->mass_data = MassData(0);
		obj->mat.density = 0;
		obj->mat.restitution = 0.4;
		objects.push_back(obj);
	}
}
void GameLevel::draw(SpriteRenderer &renderer) {
	for (auto i : objects) {
		i->draw(renderer);
	}
}
GLboolean GameLevel::isCompleted() {
	return GL_FALSE;
}
void GameLevel::setPlayer(GameObject* p) {
	player = p;
	objects.push_back(player);
}
GameObject* GameLevel::getPlayer() {
	return player;
}
GameLevel::~GameLevel() {
	for (auto i : objects) {
		delete i;
	}
}

void GameLevel::resolveCollision(Manifold *m) {
	glm::vec2 rv = m->B->velocity - m->A->velocity;
	glm::vec2 tangent = rv - glm::dot(rv, m->normal) * m->normal;
	glm::normalize(tangent);
	float velAlongNormal = glm::dot(rv, m->normal);

	if (velAlongNormal > 0) {
		return;
	}

	float e = glm::max(m->A->mat.restitution, m->B->mat.restitution);

	float j = -(1 + e) * velAlongNormal;
	j /= m->A->mass_data.invMass + m->B->mass_data.invMass;


	glm::vec2 impulse = j * m->normal;
	float jt = -glm::dot(rv, tangent);
	jt /= (m->A->mass_data.invMass + m->B->mass_data.invMass);
	float mu = glm::length(glm::vec2(m->A->staticFriction, m->B->staticFriction));
	glm::vec2 frictionImpulse;

	if (abs(jt) < j * mu) {
		frictionImpulse = jt * tangent;
	}
	else {
		float dyn = glm::length(glm::vec2(m->A->dynamicFriction, m->B->dynamicFriction));
		frictionImpulse = -j * tangent * dyn;
	}

	m->A->velocity -= m->A->mass_data.invMass * impulse;
	m->B->velocity -= m->B->mass_data.invMass * impulse;
	m->A->velocity -= m->A->mass_data.invMass * frictionImpulse;
	m->B->velocity += m->B->mass_data.invMass * frictionImpulse;
	const float percent = 0.2f; // usually 20% to 80%
	const float slop = 0.01f; // usually 0.01 to 0.1
	glm::vec2 correction = glm::max(m->penetration - slop, 0.0f) / (m->A->mass_data.invMass + m->B->mass_data.invMass) * percent * m->normal;
	m->A->position -= m->A->mass_data.invMass * correction;
	m->B->position += m->B->mass_data.invMass * correction;
}
bool AABBvsAABB(AABB a, AABB b) {
	// Exit with no intersection if found separated along an axis
	if (a.max.x < b.min.x || a.min.x > b.max.x) { return false; }
	if (a.max.y < b.min.y || a.min.y > b.max.y) { return false; }
	// No separating axis found, therefor there is at least one overlapping axis
	return true;
}

void GameLevel::BroadPhasePair() {
	pairs.clear();
	for (int i = 0; i < objects.size(); i++) {
		for (int j = 0; j < objects.size(); j++) {
			if (i != j && (objects[i]->layer == objects[j]->layer)) {
				if (AABBvsAABB(objects[i]->computeAABB(), objects[j]->computeAABB())) {
					for (Pair k : pairs) {
						if (i > j) {
							if ((k.A == objects[i] && k.B == objects[j]) || (k.A == objects[j] && k.B == objects[i])) {
								goto end;
							}
						}
					}
					if (i > j) {
						pairs.push_back(Pair(objects[i], objects[j]));
					}
					else {
						pairs.push_back(Pair(objects[j], objects[i]));
					}
				}		
			}
		end:;
		}
	}

}
bool GameLevel::AABBAABB(Manifold *m) {
	GameObject *A = m->A;
	GameObject *B = m->B;

	// Vector from A to B
	glm::vec2 n = B->getPos() - A->getPos();

	AABB abox = A->computeAABB();
	AABB bbox = B->computeAABB();

	// Calculate half extents along x axis for each object
	float a_extent = (abox.max.x - abox.min.x) / 2;
	float b_extent = (bbox.max.x - bbox.min.x) / 2;

	// Calculate overlap on x axis
	float x_overlap = a_extent + b_extent - abs(n.x);

	// SAT test on x axis
	if (x_overlap > 0) {
		// Calculate half extents along x axis for each object
		float a_extent = (abox.max.y - abox.min.y) / 2;
		float b_extent = (bbox.max.y - bbox.min.y) / 2;
		// Calculate overlap on y axis
		float y_overlap = a_extent + b_extent - abs(n.y);
		// SAT test on y axis
		if (y_overlap > 0) {
			// Find out which axis is axis of least penetration
			if (x_overlap < y_overlap) {
				// Point towards B knowing that n points from A to B
				if (n.x < 0) {
					m->normal = glm::vec2(-1, 0);
				}
				else {
					m->normal = glm::vec2(1, 0);
				}
				m->penetration = x_overlap;
				return true;
			}
			else {
				// Point toward B knowing that n points from A to B
				if (n.y < 0) {
					m->normal = glm::vec2(0, -1);
				}
				else {
					m->normal = glm::vec2(0, 1);
				}
				m->penetration = y_overlap;
				return true;
			}

		}
	}
	return false;
}
void GameLevel::update(GLfloat dt) {
	BroadPhasePair();
	//std::cout << pairs.size() << std::endl;
	for (Pair i : pairs) {
		Manifold* m = new Manifold;
		m->A = i.A;
		m->B = i.B;
		if (AABBAABB(m)) {
			std::cout << "COLLIDECOLLIDECOLLIDECOLLIDECOLLIDE" << std::endl;
			resolveCollision(m);
		}
		delete m;
	}
	for (GameObject* i : objects) {
		i->force = glm::vec2(0);
		i->force = glm::vec2(0, gravity) + i->appliedF;
		i->velocity += i->mass_data.invMass * i->force * dt;
		i->position += i->velocity * dt;
		std::cout <<
			"Force(" << i->force.x << "," << i->force.y <<
			")\tMass(" << i->mass_data.mass << "," << i->mass_data.invMass <<
			")\tVelocity(" << i->velocity.x << "," << i->velocity.y <<
			")\tPosition(" << i->position.x << "," << i->position.y <<
			")\tdt: " << dt <<
			std::endl;

	}
}