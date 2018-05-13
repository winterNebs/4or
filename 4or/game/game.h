#ifndef _GAME_
#define _GAME_
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game_level.h"

enum class GameState {
GAME_ACTIVE,
GAME_MENU
};
class Game {
public:
	GameState state;
	GLboolean keys[1024];
	GLint width, height;
	std::vector<GameLevel> levels;
	GLuint level;
	Game(GLuint w, GLuint h);
	~Game();
	void init();
	void processInput(GLfloat dt);
	void update(GLfloat dt);
	void render();
};
#endif