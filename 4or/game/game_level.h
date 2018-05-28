#pragma once

#include "game_object.h"

#include "../svg/svg_reader.h"
#include "../resources/resource_manager.h"

#include <vector>
struct Pair {
	GameObject* A;
	GameObject* B;
	Pair(GameObject* a, GameObject* b) : A(a), B(b) {}
};
struct Manifold {
	GameObject* A;
	GameObject* B;
	GLfloat penetration;
	glm::vec2 normal;
};
struct AABB {
	glm::vec2 min;
	glm::vec2 max;
};
class GameLevel {
public:
	std::vector<GameObject*> objects;
	std::vector<Pair> pairs;
	GameObject* player;
	

	GameLevel() {}
	GameLevel(std::string file);
	~GameLevel();
	void setPlayer(GameObject* p);
	GameObject* getPlayer();
	void load(std::string file);
	void draw(SpriteRenderer &renderer);
	GLboolean isCompleted();
	void BroadPhasePair();
	bool AABBAABB(Manifold *m);
	void positionCorrection(GameObject* obj);
private:
	void init(std::vector<shape*> blockData);
};
