#pragma once

#include "game_object.h"

#include "../svg/svg_reader.h"
#include "../resources/resource_manager.h"

#include <vector>

class GameLevel {
public:
	std::vector<GameObject*> objects;
	GameObject* player;
	GLfloat gravity;

	GameLevel() {}
	GameLevel(std::string file);
	~GameLevel();
	void setPlayer(GameObject* p);
	GameObject* getPlayer();
	void load(std::string file);
	void draw(SpriteRenderer &renderer);
	GLboolean isCompleted();

	void update(GLfloat dt);
private:
	void init(std::vector<shape*> blockData);
};
