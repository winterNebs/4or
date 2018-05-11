#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "resources/shader.h"
#include "camera.h"
#include "svg/svg_reader.h"
#include "game.h"
#include "resources/resource_manager.h"

#include <iostream>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

///Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

///Camera Pos
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

Game game(SCR_WIDTH, SCR_HEIGHT);

int main(int argc, char *argv[]) {

	svgReader test = svgReader();
	std::vector<shape*> shapes = test.read(".\\svg\\2rect.svg");
	for (auto i : shapes) {
		std::vector<glm::vec3> convert = i->convert();
		for (auto j : convert) {
			std::cout << "(" << j.x << ", " << j.y << ")";
		}
		std::cout << std::endl;
	}
	///GLFW: initialize & configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	//MacOSX

	///GLFW: window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "4or", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	///GLAD: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	game.init();

	///Frame tracker
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	game.state = GameState::GAME_ACTIVE;

	while (!glfwWindowShouldClose(window)) {

		///Calculate frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		game.processInput(deltaTime);

		game.update(deltaTime);

		///Render stuff
		glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		game.render();
	
		glfwSwapBuffers(window);
	}
	///Deallocate 
	ResourceManager::clear();
	
	for (auto&i : shapes) {
		delete i;
	}
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			game.keys[key] = GL_TRUE;
		}
		else if (action == GLFW_RELEASE) {
			game.keys[key] = GL_FALSE;
		}
	}
}