#include "game_level.h"

GameLevel::GameLevel(std::string f) {
	load(f);
}
void GameLevel::load(std::string file) {
	staticObjects.clear();
	svgReader test = svgReader(file);
	init(test.getShapes());
}
void GameLevel::init(std::vector<shape*> blockData) {
	///Calculate max size;
	for (auto i : blockData) {
		GameObject* obj = new GameObject(glm::vec2(i->x, i->y), glm::vec2(i->width, i->height),
			ResourceManager::getTexture("container"), true, glm::vec3(0.4f, 0.4f, 0.4f));
		obj->isSolid = GL_TRUE;
		staticObjects.push_back(obj);
	}
}
void GameLevel::draw(SpriteRenderer &renderer) {
	for (auto i : staticObjects) {
		i->draw(renderer);
	}
	for (auto i : movingObjects) {
		i->draw(renderer);
	}
}
GLboolean GameLevel::isCompleted() {
	return GL_FALSE;
}
void GameLevel::setPlayer(GameEntity* p) {
	player = p;
	movingObjects.push_back(player);
}
GameEntity* GameLevel::getPlayer() {
	return player;
}
GameLevel::~GameLevel() {
	for (auto i : staticObjects) {
		delete i;
	}
	for (auto i : movingObjects) {
		delete i;
	}
}