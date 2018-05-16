#include "game.h"
#include "../resources/resource_manager.h"
#include "../resources/sprite_renderer.h"
#include <glm/gtc/matrix_transform.hpp>

SpriteRenderer* renderer;
GameObject* player;

Game::Game(GLuint w, GLuint h) :
	state(GameState::GAME_ACTIVE), keys(), width(w), height(h) {
}
Game::~Game() {
	delete renderer;
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
	GameLevel one; one.load(".\\svg\\2rect.svg");
	GameLevel two; two.load(".\\svg\\3rect.svg");
	levels.push_back(one);
	levels.push_back(two);
	level = 1;
	glm::vec2 playerPos = glm::vec2(300.0f, 100.0f);
	player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::getTexture("player"), false);
}
void Game::processInput(GLfloat dt) {
	if (state == GameState::GAME_ACTIVE) {
		player->acceleration.x = 0.0f;
		player->acceleration.y = 0.0f;
		GLfloat velocity = PLAYER_VELOCITY * dt;
		if (keys[GLFW_KEY_LEFT]) {
			player->acceleration.x = -600.0f;
		}
		if (keys[GLFW_KEY_RIGHT]) {
			player->acceleration.x = 600.0f;
		}
		if (keys[GLFW_KEY_UP]) {
			player->acceleration.y = -600.0f;
		}
		if (keys[GLFW_KEY_DOWN]) {
			player->acceleration.y = 600.0f;
		}
	}
}
void Game::update(GLfloat dt) {
	player->move(dt);
}
void Game::render() {
	if (state == GameState::GAME_ACTIVE) {
		levels[level].draw(*renderer);
		player->draw(*renderer);
	}
}