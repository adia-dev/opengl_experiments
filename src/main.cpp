#include <GLFW/glfw3.h>
#include <config/constants.h>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <shader/Shader.h>
#include <sstream>
#include <string>

void c_speed(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

static float aspect_ratio = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);

int main(int argc, char **argv) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Fractalllllllls", NULL, NULL);
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

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	glfwSetFramebufferSizeCallback(window, c_speed);

	// just drawing a square (two triangles) on the whole screen
	const float vertices[] = {-1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO
	glBindVertexArray(0);             // Unbind the VAO

	Shader shader("fractal_vertex.glsl", "fractal_fragment.glsl");

	float displacement[2] = {0.f, 0.f};
	float c[2]            = {0.38f, 0.28f};
	float z[2]            = {0.f, 0.f};
	float scale           = 0.75f;

	const float scale_factor = 0.1f;
	const float c_speed      = 0.001f;

	while (!glfwWindowShouldClose(window)) {
		// Input
		processInput(window);

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			displacement[1] += 0.01f / scale;
		} else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			displacement[1] -= 0.01f / scale;
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			displacement[0] += 0.01f / scale;
		} else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			displacement[0] -= 0.01f / scale;
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			c[0] += c_speed / scale;
		} else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			c[0] -= c_speed / scale;
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			c[1] += c_speed / scale;
		} else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			c[1] -= c_speed / scale;
		}

		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
			scale *= (1.0f + scale_factor);
		} else if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
			scale /= (1.0f + scale_factor);
		}

		// Rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();

		shader.set_vec("displacement", displacement, 2);
		shader.set_vec("z", z, 2);
		shader.set_vec("c", c, 2);
		shader.set_float("scale", scale);
		shader.set_float("aspect_ratio", aspect_ratio);

		glBindVertexArray(VAO);           // Re-bind the VAO
		glDrawArrays(GL_TRIANGLES, 0, 6); // Draw 6 vertices (2 triangles)

		// Check and Events + Buf Swapping
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void c_speed(GLFWwindow *window, int width, int height) {
	aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
