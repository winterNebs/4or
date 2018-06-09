#pragma once

#include "game_object.h"

#include "../svg/svg_reader.h"
#include "../resources/resource_manager.h"

#include <vector>
#include <string>

class DebugText {		//Onscreen text 
public:
	std::string text;
	glm::vec2 pos;
	DebugText(std::string t, glm::vec2 p) : text(t), pos(p) { }

};
class GameLevel {
public:
	float m_dt;
	int m_iterations;
	std::vector<DebugText*> debug;		//List of debug text
	std::vector<GameObject*> objects;	//List of objects in level
	std::vector<Manifold> contacts;		//List of contacts (physics)
	GamePlayer* player;					//Player

	GameLevel(float dt, int iter) : m_dt(dt), m_iterations(iter) {}
	GameLevel() : m_dt(DT), m_iterations(10) {}
	GameLevel(std::string file);
	~GameLevel();
	void setPlayer(GamePlayer* p);
	GamePlayer* getPlayer();
	void addEnemy(glm::vec2 pos);
	void load(std::string file);
	void draw(SpriteRenderer &renderer);
	GLboolean isCompleted();

	void step();
private:
	void init(std::vector<shape*> blockData);
};