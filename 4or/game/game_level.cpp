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
void GameLevel::setPlayer(GameEntity* p) {
	player = p;
	objects.push_back(player);
}
GameEntity* GameLevel::getPlayer() {
	return player;
}
GameLevel::~GameLevel() {
	for (auto i : objects) {
		delete i;
	}
}
void GameLevel::BroadPhasePair() {
	pairs.clear();
	std::vector<Pair> temp;
	for (int i = 0; i < objects.size(); i++) {
		for (int j = 0; j < objects.size(); j++) {
			if (i != j && (objects[i]->layer == objects[j]->layer)) {
				if (objects[i]->collide(objects[j])) {
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
}