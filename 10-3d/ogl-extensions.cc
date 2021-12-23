//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Extensions simplest example: blank square on the screen
//  This example extends ogl_simlest
//  glad required for using extensions like GL_ARRAY_BUFFER or GL_STATIC_DRAW
//
//  cl /EHsc ogl-extensions.cc /link glad.lib glfw3dll.lib opengl32.lib
//
//----------------------------------------------------------------------------

#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

// initial window sizes
constexpr int SZX = 600;
constexpr int SZY = 600;

// custom error handler class
struct glfw_error : public std::runtime_error {
  glfw_error(const char *s) : std::runtime_error(s) {}
};

// throw on errors
void error_callback(int, const char *err_str) { throw glfw_error(err_str); }

// make sure the viewport matches the new window dimensions
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// initialization routine
GLFWwindow *initialize_window() {
  GLFWwindow *Window;
  glfwSetErrorCallback(error_callback);
  glfwInit();
  Window = glfwCreateWindow(SZX, SZY, "Hello World", NULL, NULL);
  assert(Window); // error callback shall throw otherwise
  glfwMakeContextCurrent(Window);
  glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    throw glfw_error("Failed to initialize GLAD");
  return Window;
}

// vertices to render
GLfloat Vertices[] = {
    // positions        // colors
    -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 1.0f, // top left
    0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f, // top right
    0.5f,  -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, // bottom left
};

// render routine
void do_render(unsigned VAO) {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glBindVertexArray(VAO);
  glDrawArrays(GL_QUADS, 0, 4);
}

// entry point
int main() try {
  auto Cleanup = [](GLFWwindow *) { glfwTerminate(); };
  using UWnd = std::unique_ptr<GLFWwindow, decltype(Cleanup)>;
  UWnd Wnd(initialize_window(), Cleanup);

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                        (void *)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                        (void *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  while (!glfwWindowShouldClose(Wnd.get())) {
    do_render(VAO);
    glfwSwapBuffers(Wnd.get());
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

} catch (glfw_error &E) {
  std::cout << "GLFW error: " << E.what() << std::endl;
} catch (std::exception &E) {
  std::cout << "Standard error: " << E.what() << std::endl;
} catch (...) {
  std::cout << "Unknown error\n";
}
