//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Simplest example: blank square on the screen
//  This example also show how to use GLFW library which is rather standard
//  Note really short time to first triangle (quad in this case)
//
//  cl /EHsc ogl-simplest.cc /link glfw3dll.lib opengl32.lib
//
//----------------------------------------------------------------------------

#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>

#include <GLFW/glfw3.h>

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
  return Window;
}

// vertices to render
GLfloat Vertices[4][3] = {
    {-0.5f, 0.5f, 0.0f},  // top left
    {0.5f, 0.5f, 0.0f},   // top right
    {0.5f, -0.5f, 0.0f},  // bottom right
    {-0.5f, -0.5f, 0.0f}, // bottom left
};

// render routine
void do_render() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_QUADS);
  glColor3f(1.0, 1.0, 1.0);
  for (auto Coord : Vertices)
    glVertex3fv(Coord);
  glEnd();
}

// entry point
int main() try {
  auto Cleanup = [](GLFWwindow *) { glfwTerminate(); };
  using UWnd = std::unique_ptr<GLFWwindow, decltype(Cleanup)>;
  UWnd Wnd(initialize_window(), Cleanup);

  while (!glfwWindowShouldClose(Wnd.get())) {
    do_render();
    glfwSwapBuffers(Wnd.get());
    glfwPollEvents();
  }

} catch (glfw_error &E) {
  std::cout << "GLFW error: " << E.what() << std::endl;
} catch (std::exception &E) {
  std::cout << "Standard error: " << E.what() << std::endl;
} catch (...) {
  std::cout << "Unknown error\n";
}
