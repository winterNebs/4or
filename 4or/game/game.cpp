#include "game.h"


#include <iostream>
#include <fstream>
///TODO:
/*
- Remove all SQRTs when possible
*/
int counter = 0;
SpriteRenderer* renderer;
Game::Game(GLuint w, GLuint h) :
	state(GameState::GAME_ACTIVE), keys(), width(w), height(h) {
	counter = 0;
}
Game::~Game() {
	delete renderer;
	for (auto i : levels) {
		delete i;
	}
}

void Game::init() {
	///Deugging
	std::ofstream file;
	file.open(".\\logs\\normals.txt", std::ofstream::out | std::ofstream::trunc);
	file << "";
	file.close();
	ResourceManager::loadShader(".\\resources\\shaders\\sprite.vs", ".\\resources\\shaders\\sprite.fs", nullptr, "sprite");
	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->width), static_cast<GLfloat>(this->height), 0.0f, -1.0f, 1.0f);
	ResourceManager::getShader("sprite").use().setInteger("image", 0);
	ResourceManager::getShader("sprite").setMatrix4("projection", projection);
	// Load textures
	ResourceManager::loadTexture(".\\resources\\textures\\container.jpg", GL_TRUE, "container");
	ResourceManager::loadTexture(".\\resources\\textures\\frik.png", GL_TRUE, "player");
	// Set render-specific controls
	Shader myShader = ResourceManager::getShader("sprite");
	renderer = new SpriteRenderer(myShader);
	// Load levels
	GameLevel* one = new GameLevel(); one->load(".\\svg\\2rect.svg");
	GameLevel* two = new GameLevel(); two->load(".\\svg\\3rect.svg");
	GameLevel* three = new GameLevel(); three->load(".\\svg\\1box.svg");
	GameLevel* four = new GameLevel(); four->load(".\\svg\\5rect.svg");
	GameLevel* five = new GameLevel(); five->load(".\\svg\\normaldebug.svg");
	levels.push_back(one);
	levels.push_back(two);
	levels.push_back(three);
	levels.push_back(four);
	levels.push_back(five);
	level = 3;
	glm::vec2 playerPos = glm::vec2(300.0f, 100.0f);
	GameObject* player = new GameObject(ResourceManager::getTexture("player"));
	levels[level]->setPlayer(player->initRect(playerPos, PLAYER_SIZE));
}
void Game::processInput(GLfloat dt) {
	if (state == GameState::GAME_ACTIVE) {
		const float PS = 600.0f;
		//levels[level]->getPlayer()->body->force = glm::vec2(0);
		//levels[level]->getPlayer()->appliedF = glm::vec2(600.0f, 0);
		//GLfloat velocity = PLAYER_VELOCITY * dt;
		if (keys[GLFW_KEY_LEFT]) {
			levels[level]->getPlayer()->body->applyForce(glm::vec2(-PS, 0));
		}
		if (keys[GLFW_KEY_RIGHT]) {
			levels[level]->getPlayer()->body->applyForce(glm::vec2(PS, 0));
		}
		if (keys[GLFW_KEY_UP]) {
			levels[level]->getPlayer()->body->applyForce(glm::vec2(0, -PS));
		}
		if (keys[GLFW_KEY_DOWN]) {
			levels[level]->getPlayer()->body->applyForce(glm::vec2(0, PS));
		}
	}
}
void Game::update(GLfloat dt) {
	levels[level]->gravity = 500.0f;
	//levels[level]->update(dt);
	levels[level]->step();
}
void Game::render() {
	if (state == GameState::GAME_ACTIVE) {
		levels[level]->draw(*renderer);
	}
}