#include "game_level.h"

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
		//std::cout << typeid(i).name() << std::endl;
		//std::cout << i.acceleration.x << std::endl;
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