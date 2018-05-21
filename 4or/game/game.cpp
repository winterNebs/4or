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
	GameLevel* three = new GameLevel(); three->load(".\\svg\\1box.svg");
	levels.push_back(one);
	levels.push_back(two);
	levels.push_back(three);
	level = 2;
	glm::vec2 playerPos = glm::vec2(300.0f, 100.0f);
	GameEntity* player = new GameEntity(playerPos, PLAYER_SIZE, ResourceManager::getTexture("player"));
	levels[level]->setPlayer(player);
}
void Game::processInput(GLfloat dt) {
	if (state == GameState::GAME_ACTIVE) {
		levels[level]->getPlayer()->appliedF = glm::vec2(0);
		GLfloat velocity = PLAYER_VELOCITY * dt;
		if (keys[GLFW_KEY_LEFT]) {
			levels[level]->getPlayer()->appliedF.x -= 600.0f;
		}
		if (keys[GLFW_KEY_RIGHT]) {
			levels[level]->getPlayer()->appliedF.x += 600.0f;
		}
		if (keys[GLFW_KEY_UP]) {
			levels[level]->getPlayer()->appliedF.y -= 600.0f;
		}
		if (keys[GLFW_KEY_DOWN]) {
			levels[level]->getPlayer()->appliedF.y += 600.0f;
		}
	}
}
void Game::update(GLfloat dt) {
	for (GameObject* i : levels[level]->objects) {
		if (!i->isStatic) {
			for (GameObject* j : levels[level]->objects) {
				if (j != i) {
					if (i->collide(j, dt)) {
						GLfloat temp = i->calcTime(i->getCloseDist(j));
						//std::cout << "Dist:\t"<< i->getCloseDist(j) << " Time:\t" << temp << " DT:\t" << dt << std::endl;
						//i->move(temp);
						//std::cout << "collide lol" << std::endl;
					}
					else {
						///I know this is bad but im hopeful

						i->move(dt);
					}
				}
			}
			//i->normalF = glm::vec2(0);

		}
	}
}
void Game::render() {
	if (state == GameState::GAME_ACTIVE) {
		levels[level]->draw(*renderer);
	}
}