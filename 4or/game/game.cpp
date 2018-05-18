#include "game.h"
#include "game_entity.h"
#include "../resources/resource_manager.h"
#include "../resources/sprite_renderer.h"
#include <glm/gtc/matrix_transform.hpp>

SpriteRenderer* renderer;

Game::Game(GLuint w, GLuint h) :
	state(GameState::GAME_ACTIVE), keys(), width(w), height(h) {
}
Game::~Game() {
	delete renderer;
	for (auto i : levels) {
		delete i;
	}
}

void Game::init() {
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
	levels.push_back(one);
	levels.push_back(two);
	level = 1;
	glm::vec2 playerPos = glm::vec2(300.0f, 100.0f);
	GameEntity* player = new GameEntity(playerPos, PLAYER_SIZE, ResourceManager::getTexture("player"));
	levels[level]->setPlayer(player);
}
void Game::processInput(GLfloat dt) {
	if (state == GameState::GAME_ACTIVE) {
		levels[level]->getPlayer()->acceleration.x = 0.0f;
		levels[level]->getPlayer()->acceleration.y = 0.0f;
		GLfloat velocity = PLAYER_VELOCITY * dt;
		if (keys[GLFW_KEY_LEFT]) {
			levels[level]->getPlayer()->acceleration.x = -600.0f;
		}
		if (keys[GLFW_KEY_RIGHT]) {
			levels[level]->getPlayer()->acceleration.x = 600.0f;
		}
		if (keys[GLFW_KEY_UP]) {
			levels[level]->getPlayer()->acceleration.y = -600.0f;
		}
		if (keys[GLFW_KEY_DOWN]) {
			levels[level]->getPlayer()->acceleration.y = 600.0f;
		}
	}
}
void Game::update(GLfloat dt) {
	for (GameObject* i : levels[level]->objects) {
		i->move(dt);
		if (!i->isSolid) {
			for (GameObject* j : levels[level]->objects) {
				if (j != i) {
					if (i->collide(j, dt)) {

						std::cout << "collide lol";
					}
				}
			}
		}
	}
}
void Game::render() {
	if (state == GameState::GAME_ACTIVE) {
		levels[level]->draw(*renderer);
	}
}