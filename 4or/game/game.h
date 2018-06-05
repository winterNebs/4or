#pragma once

#include "game_level.h"
#include "../resources/resource_manager.h"
#include "../resources/sprite_renderer.h"
#include "../resources/text_renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const glm::vec2 PLAYER_SIZE(50.0f, 100.0f);
const GLfloat PLAYER_VELOCITY(200.0f);

enum class GameState {
GAME_ACTIVE,
GAME_MENU
};
class Game {
public:
	GameState state;
	GLboolean keys[1024];
	GLint width, height;
	std::vector<GameLevel*> levels;
	GLuint level;
	Game(GLuint w, GLuint h);
	~Game();
	void init();
	void processInput(GLfloat dt);
	void update(GLfloat dt);
	void render();

};