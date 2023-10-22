#include "config/core.h"
#include <GLFW/glfw3.h>
#include <cstddef>
#include <cstdio>
#include <glad/glad.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(int argc, char **argv) {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "FirstOpenGLProject", NULL, NULL);
  if(window == NULL) {
      printf("Failed to create GLFW Window.\n");
      glfwTerminate();
      return -1;
  }
  glfwMakeContextCurrent(window);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      printf("Failed to initialize GLAD.\n");
      return -1;
  }

  glViewport(0, 0, 800, 600);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  while(!glfwWindowShouldClose(window)){
      glfwSwapBuffers(window);
      glfwPollEvents();
  }


  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
