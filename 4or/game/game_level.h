#pragma once

#include "game_object.h"

#include "../svg/svg_reader.h"
#include "../resources/resource_manager.h"

#include <vector>

class GameLevel {
public:
	float m_dt;
	int m_iterations;

	std::vector<GameObject*> objects;
	std::vector<Manifold> contacts;
	GamePlayer* player;

	GameLevel(float dt, int iter) : m_dt(dt), m_iterations(iter) {}
	GameLevel() : m_dt(DT), m_iterations(10) {}
	GameLevel(std::string file);
	~GameLevel();
	void setPlayer(GamePlayer* p);
	GamePlayer* getPlayer();
	void load(std::string file);
	void draw(SpriteRenderer &renderer);
	GLboolean isCompleted();

	void update(GLfloat dt);
	void step();
private:
	void init(std::vector<shape*> blockData);
};
