#include "config/core.h"
#include "shaders/Shader.h"

#include <GLFW/glfw3.h>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <string>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

// Input Handling
void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main(int argc, char **argv) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "FirstOpenGLProject", NULL, NULL);
	if (window == NULL) {
		printf("Failed to create GLFW Window.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD.\n");
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO
	glBindVertexArray(0);             // Unbind the VAO

	Shader shader("triangle.vert", "triangle.frag");

	float color[]    = {0.1f, 1.0f, 0.f, 1.0f};
	float position[] = {0.0f, 0.0f, 0.0f};

	while (!glfwWindowShouldClose(window)) {
		// Input
		processInput(window);

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            position[0] -= 0.1f;
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            position[0] += 0.1f;
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            position[1] += 0.1f;
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            position[1] -= 0.1f;
		}

		// Rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float time_value = glfwGetTime();
		color[1]         = std::sin(time_value) / 2.0f + 0.5f;
		shader.use();

		shader.set_vec("ourColor", color, 4);
		shader.set_vec("position", position, 3);

		glBindVertexArray(VAO); // Re-bind the VAO
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Check and Events + Buf Swapping
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}
