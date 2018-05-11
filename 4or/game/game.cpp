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
	// Set render-specific controls
	Shader myShader = ResourceManager::getShader("sprite");
	renderer = new SpriteRenderer(myShader);
	// Load textures
	//ResourceManager::loadTexture(".\\resources\\textures\\container.jpg", GL_TRUE, "face");
	ResourceManager::loadTexture(".\\resources\\textures\\container.jpg", GL_TRUE, "face");
}
void Game::processInput(GLfloat dt) {

}
void Game::update(GLfloat dt) {

}
void Game::render() {
	Texture2D myText = ResourceManager::getTexture("face");
	renderer->drawSprite(myText,
		glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}