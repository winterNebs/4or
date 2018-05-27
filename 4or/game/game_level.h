#pragma once

#include "game_object.h"
#include "game_entity.h"

#include "../svg/svg_reader.h"
#include "../resources/resource_manager.h"

#include <vector>
struct Pair {
	GameObject* A;
	GameObject* B;
	Pair(GameObject* a, GameObject* b) : A(a), B(b) {}
};

class GameLevel {
public:
	std::vector<GameObject*> objects;
	std::vector<Pair> pairs;
	GameEntity* player;

	GameLevel() {}
	GameLevel(std::string file);
	~GameLevel();
	void setPlayer(GameEntity* p);
	GameEntity* getPlayer();
	void load(std::string file);
	void draw(SpriteRenderer &renderer);
	GLboolean isCompleted();
	void BroadPhasePair();
private:
	void init(std::vector<shape*> blockData);
};
