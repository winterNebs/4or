#include <glm/gtc/matrix_transform.hpp>
#include "game.h"
#include "../resources/resource_manager.h"
#include "../resources/sprite_renderer.h"

SpriteRenderer* renderer;

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
	ResourceManager::loadTexture(".\\resources\\textures\\frik.png", GL_TRUE, "bg");
	// Set render-specific controls
	Shader myShader = ResourceManager::getShader("sprite");
	renderer = new SpriteRenderer(myShader);
	// Load levels
	GameLevel one; one.load(".\\svg\\2rect.svg");
	levels.push_back(one);
	level = 0;

}
void Game::processInput(GLfloat dt) {

}
void Game::update(GLfloat dt) {

}
void Game::render() {
	if (state == GameState::GAME_ACTIVE) {
		levels[level].draw(*renderer);
	}
	
}