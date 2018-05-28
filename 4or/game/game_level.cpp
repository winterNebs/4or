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
			ResourceManager::getTexture("container"), true, glm::vec3(0.4f, 0.4f, 0.4f));
		obj->isSolid = GL_TRUE;
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
void GameLevel::positionCorrection(GameObject* obj) {
	const float percent = 0.2; // usually 20% to 80%
	const float slop = 0.01; // usually 0.01 to 0.1
	glm::vec2 correction = glm::max(penetration - slop, 0.0f) / (A.inv_mass + B.inv_mass)) * percent * n;
	A.position -= A.inv_mass * correction;
	B.position += B.inv_mass * correction;
}
void GameLevel::BroadPhasePair() {
	pairs.clear();
	std::vector<Pair> temp;
	for (int i = 0; i < objects.size(); i++) {
		for (int j = 0; j < objects.size(); j++) {
			if (i != j && (objects[i]->layer == objects[j]->layer)) {
				if (i > j) {
					for (Pair k : pairs) {
						if (k.A == objects[i] && k.B == objects[j]) {
							goto end;
						}
					}
					temp.push_back(Pair(objects[i], objects[j]));
				}
				else {
					for (Pair k : pairs) {
						if (k.A == objects[j] && k.B == objects[i]) {
							goto end;
						}
					}
					temp.push_back(Pair(objects[j], objects[i]));
				}
			end:
			}
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
	if (x_overlap > 0)	{
		// Calculate half extents along x axis for each object
		float a_extent = (abox.max.y - abox.min.y) / 2;
		float b_extent = (bbox.max.y - bbox.min.y) / 2;
		// Calculate overlap on y axis
		float y_overlap = a_extent + b_extent - abs(n.y);
			// SAT test on y axis
		if (y_overlap > 0) {
			// Find out which axis is axis of least penetration
			if (x_overlap > y_overlap) {
				// Point towards B knowing that n points from A to B
				if (n.x < 0) {
					m->normal = glm::vec2(-1, 0);
				}
				else {
					m->normal = glm::vec2(0, 0);
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
}