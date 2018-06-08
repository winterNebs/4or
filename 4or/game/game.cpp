#include "game.h"

#include <sstream>
#include <iostream>
#include <fstream>
///TODO:
/*
- Remove all SQRTs when possible
*/
int counter = 0;
SpriteRenderer* renderer;
TextRenderer* text;
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
GamePlayer* player;
GameExit* exitE;
void Game::init() {
	glm::vec2 playerPos = glm::vec2(300.0f, 100.0f);

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
	ResourceManager::loadTexture(".\\resources\\textures\\frik.png", GL_TRUE, "enemy");
	ResourceManager::loadTexture(".\\resources\\textures\\stand.png", GL_TRUE, "player");
	ResourceManager::loadTexture(".\\resources\\textures\\exit.png", GL_TRUE, "exit");
	// Set render-specific controls
	Shader myShader = ResourceManager::getShader("sprite");
	renderer = new SpriteRenderer(myShader);
	// Load Text Renderer
	text = new TextRenderer(this->width, this->width);
	text->load(".\\resources\\fonts\\OpenSans-Regular.ttf", 24);

	// Load levels
	GameLevel* one = new GameLevel(); one->load(".\\svg\\2rect.svg");
	GameLevel* two = new GameLevel(); two->load(".\\svg\\3rect.svg");
	GameLevel* three = new GameLevel(); three->load(".\\svg\\1box.svg");
	GameLevel* four = new GameLevel(); four->load(".\\svg\\5rect.svg");
	GameLevel* five = new GameLevel(); five->load(".\\svg\\normaldebug.svg");
	GameLevel* six = new GameLevel(); six->load(".\\svg\\level1.svg");
	levels.push_back(one);
	levels.push_back(two);
	levels.push_back(three);
	levels.push_back(four);
	levels.push_back(five);
	levels.push_back(six);
	level = 5;
	player = new GamePlayer(ResourceManager::getTexture("player"), PLAYER_SIZE, playerPos, 0.01f, 0.9f, 0.6f, 0.9f);
	exitE = new GameExit(ResourceManager::getTexture("exit"), PLAYER_SIZE, glm::vec2(3000.0f, 1000.0f));
	levels[level]->objects.push_back(exitE);
	levels[level]->setPlayer(player);
	//levels[level]->addEnemy(glm::vec2(400.0f, 300.0f));
}
void Game::processInput(GLfloat dt) {
	if (state == GameState::GAME_ACTIVE) {
		//levels[level]->getPlayer()->body->force = glm::vec2(0);
		//levels[level]->getPlayer()->body->applyForce(glm::vec2(PS, 0));
		//GLfloat velocity = PLAYER_VELOCITY * dt;
		//std::cout << player->isColliding << std::endl;
		if (keys[GLFW_KEY_LEFT]) {
			player->move(DIR::left);
		}
		if (keys[GLFW_KEY_RIGHT]) {
			player->move(DIR::right);
		}
		if (keys[GLFW_KEY_UP]) {
			keys[GLFW_KEY_UP] = false;
			player->move(DIR::up);
		}
		if (keys[GLFW_KEY_DOWN]) {
			//levels[level]->getPlayer()->body->applyForce(glm::vec2(0, PS));

		}
	}
}
void Game::update(GLfloat dt) {
	//levels[level]->update(dt);
	levels[level]->step();
	if (exitE->gameEnd) {
		state = GameState::GAME_END;
	}
}
void Game::render() {
	if (state == GameState::GAME_ACTIVE) {
		levels[level]->draw(*renderer);
		//text->resetMatrix(this->width, this->height);


		glm::vec2 pps = player->body->position;
		glm::mat4 projection = glm::ortho(
			static_cast<GLfloat>(pps.x - this->width / 2.0f),
			static_cast<GLfloat>(pps.x + this->width / 2.0f),
			static_cast<GLfloat>(pps.y + this->height / 2.0f),
			static_cast<GLfloat>(pps.y - this->height / 2.0f), -1.0f, 1.0f);
		ResourceManager::getShader("sprite").setMatrix4("projection", projection);
	
		//text->renderText("''''''", glm::vec2(0.0f), 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		

		for (auto i : levels[level]->debug) {
//			std::cout << i->pos.x << std::endl;
			text->renderText(i->text, i->pos, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		text->setMatrix(projection);
	}
	else {
		text->resetMatrix(this->width, this->height);
		text->renderText("Paused", glm::vec2(this->width/2.0f, this->height/8.0f), 4.0f, glm::vec3(1.0f));
	}
}