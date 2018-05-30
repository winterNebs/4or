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
		/*GameObject* obj = new GameObject(glm::vec2(i->x, i->y), glm::vec2(i->width, i->height),
			ResourceManager::getTexture("container"), 0, glm::vec3(0.4f, 0.4f, 0.4f));

		obj->isSolid = GL_TRUE;
		obj->mass_data = MassData(0);
		obj->mat.density = 0;
		obj->mat.restitution = 0.4;
		objects.push_back(obj);*/
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
void GameLevel::update(GLfloat dt) {
	
}