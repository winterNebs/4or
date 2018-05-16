#include "game_level.h"

void GameLevel::load(std::string file) {
	blocks.clear();
	svgReader test = svgReader(file);
	init(test.getShapes());
}
void GameLevel::init(std::vector<shape*> blockData) {
	///Calculate max size;
	for (auto i : blockData) {
		GameObject obj(glm::vec2(i->x, i->y), glm::vec2(i->width, i->height),
			ResourceManager::getTexture("container"), true, glm::vec3(0.4f, 0.4f, 0.4f));
		obj.isSolid = GL_TRUE;
		blocks.push_back(obj);
	}
}
void GameLevel::draw(SpriteRenderer &renderer) {
	for (auto i : blocks) {
		i.draw(renderer);
	}
}
GLboolean GameLevel::isCompleted() {
	return GL_FALSE;
}