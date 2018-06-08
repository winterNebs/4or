#include "game/physics/headerSpaghetti.h"

#include "resources/shader.h"
#include "svg/svg_reader.h"
#include "game/game.h"
#include "resources/resource_manager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <time.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

///Settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

///Camera Pos
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

Game game(SCR_WIDTH, SCR_HEIGHT);

int main(int argc, char *argv[]) {

	
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
	srand((unsigned int)time(NULL));
	///Frame tracker
	float accumulator = 0.0f;
	float frameStart = (float)glfwGetTime();
	float time = 0.0;
	game.state = GameState::GAME_MENU;
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();
		glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		///Calculate frame time
		const GLfloat currentFrame = (GLfloat)glfwGetTime();
		accumulator += currentFrame - frameStart;
		frameStart = currentFrame;
		if (accumulator > 0.2f) {
			accumulator = 0.2f;
		}
		
		//std::cout << "FPS: " << 1 / (deltaTime) << std::endl;
			//std::cout << accumulator << std::endl;
		while (accumulator > DT && game.state == GameState::GAME_ACTIVE) {
			time += DT;
			game.processInput(DT);

			game.update(DT);
			
			accumulator -= DT;
		}
		if (game.state == GameState::GAME_END) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		/*
		  const float alpha = accumulator / dt;
 
			RenderGame( alpha )
 
			void RenderGame( float alpha )
			for shape in game do
			  // calculate an interpolated transform for rendering
			 Transform i = shape.previous * alpha + shape.current * (1.0f - alpha)
			 shape.previous = shape.current
			 shape.Render( i )*/
		///Render stuff

		game.render();
		glfwSwapBuffers(window);
	}
	///Deallocate 
	ResourceManager::clear();
	if (game.state == GameState::GAME_END) {

		std::vector<float> times;
		std::string line;
		std::ifstream myfile(".\\scores.txt");
		if (myfile.is_open()) {
			while (getline(myfile, line)) {
				times.push_back(std::stof(line));
			}
			myfile.close();
		}
		times.push_back(time);
		if (times.size() > 1) {
			for (int i = 1; i < times.size(); i++) {
				for (int j = i - 1; j >= 0; j--) {
					if (times[j + 1] < times[j]) {
						float temp = times[j + 1];
						times[j + 1] = times[j];
						times[j] = temp;
					}
				}
			}
		}
		std::ofstream output(".\\scores.txt", std::ios::trunc);
		for (auto i : times) {
			output << i << "\n";
		}
	}
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		if (game.state == GameState::GAME_ACTIVE) {
			game.state = GameState::GAME_MENU;
		}
		else {
			game.state = GameState::GAME_ACTIVE;
		}
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