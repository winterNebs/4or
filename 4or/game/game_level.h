#pragma once
#include "game_object.h"
#include "../svg/svg_reader.h"
#include "../resources/resource_manager.h"

#include <vector>

class GameLevel {
public:
	std::vector<GameObject> blocks;

	GameLevel() {

	}
	void load(std::string file);
	void draw(SpriteRenderer &renderer);
	GLboolean isCompleted();
private:
	void init(std::vector<shape*> blockData);
};
