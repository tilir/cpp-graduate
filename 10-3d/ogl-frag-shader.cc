//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Extensions simplest example: blank square on the screen
//  This example extends ogl_extensions
//  Show how vertex attributes starts playing in coloring
//  Show simplest fragment shader and uniform variable
//
//  cl /EHsc ogl-frag-shader.cc /link glad.lib glfw3dll.lib opengl32.lib
//
//----------------------------------------------------------------------------

#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <vector>

// clang-format off
// this headers shall be in this position
#include <glad/glad.h>
// clang-format on

#include <GLFW/glfw3.h>

// initial window sizes
constexpr int SZX = 600;
constexpr int SZY = 600;

constexpr const char *VERTNAME = "shaders/simplest.vert";
#ifdef SINCOLOR
constexpr const char *FRAGNAME = "shaders/sincolor.frag";
#else
constexpr const char *FRAGNAME = "shaders/simplest.frag";
#endif

// custom error handler class: GLFW
struct glfw_error : public std::runtime_error {
  glfw_error(const char *s) : std::runtime_error(s) {}
};

// custom error handler class: GLSL
struct glsl_error : public std::runtime_error {
  std::string ShaderLog;
  glsl_error(const char *s) : std::runtime_error(s) {}
};

// custom error handler class: GLSL compilation
struct glsl_compile_error : glsl_error {
  glsl_compile_error(const char *s, GLuint ShaderID) : glsl_error(s) {
    GLint Length;
    glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &Length);
    std::vector<char> ShaderLogV(Length);
    glGetShaderInfoLog(ShaderID, Length, NULL, ShaderLogV.data());
    ShaderLog.assign(ShaderLogV.begin(), ShaderLogV.end());
  }
};

// custom error handler class: GLSL link
struct glsl_link_error : glsl_error {
  glsl_link_error(const char *s, GLuint ProgID) : glsl_error(s) {
    GLint Length;
    glGetProgramiv(ProgID, GL_INFO_LOG_LENGTH, &Length);
    std::vector<char> ShaderLogV(Length);
    glGetProgramInfoLog(ProgID, Length, NULL, ShaderLogV.data());
    ShaderLog.assign(ShaderLogV.begin(), ShaderLogV.end());
  }
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
    0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, // top right
    -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 0.0f, // top left
    0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom left
};

// read program code from file
std::string readFile(const char *Path) {
  std::string Code;
  std::ifstream ShaderFile;
  ShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  ShaderFile.open(Path);
  std::stringstream ShaderStream;
  ShaderStream << ShaderFile.rdbuf();
  ShaderFile.close();
  Code = ShaderStream.str();
  return Code;
}

// compile shader, check errors, return ID
GLuint installShader(std::string ShaderCode, GLenum ShaderType) {
  GLuint ShaderID = glCreateShader(ShaderType);
  const char *Str = ShaderCode.c_str();
  glShaderSource(ShaderID, 1, &Str, NULL);
  glCompileShader(ShaderID);

  GLint Success;
  glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Success);
  if (!Success)
    throw glsl_compile_error("Failed to compile shader", ShaderID);

  return ShaderID;
}

// compile vertex and fragment shaders, then link program
GLuint linkShaders() {
  GLuint ProgID = glCreateProgram();
  GLuint VertexID = installShader(readFile(VERTNAME), GL_VERTEX_SHADER);
  GLuint FragmentID = installShader(readFile(FRAGNAME), GL_FRAGMENT_SHADER);
  glAttachShader(ProgID, VertexID);
  glAttachShader(ProgID, FragmentID);
  glLinkProgram(ProgID);
  GLint Success;
  glGetShaderiv(ProgID, GL_LINK_STATUS, &Success);
  if (!Success)
    throw glsl_link_error("Failed to link program", ProgID);
  return ProgID;
}

// set uniform value: 1 float
void setFloat(GLuint ProgID, const char *Name, float Val) {
  GLint Loc = glGetUniformLocation(ProgID, Name);
  glUniform1f(Loc, Val);
}

// render routine
void do_render(GLuint VAO, GLuint ProgID) {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(ProgID);
  setFloat(ProgID, "time", glfwGetTime());
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

// entry point
int main() try {
  auto Cleanup = [](GLFWwindow *) { glfwTerminate(); };
  using UWnd = std::unique_ptr<GLFWwindow, decltype(Cleanup)>;
  UWnd Wnd(initialize_window(), Cleanup);

  GLuint VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                        reinterpret_cast<void *>(0 * sizeof(GLfloat)));
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                        reinterpret_cast<void *>(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  // create/install shaders
  GLuint ProgID = linkShaders();

  while (!glfwWindowShouldClose(Wnd.get())) {
    do_render(VAO, ProgID);
    glfwSwapBuffers(Wnd.get());
    glfwPollEvents();
  }

  // NOTE: non-exception safe here. Will be fixed in following example
  //       how will you fix it, BTW?
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

} catch (glsl_error &E) {
  std::cout << "GLSL error: " << E.what() << std::endl;
  std::cout << E.ShaderLog << std::endl;
} catch (glfw_error &E) {
  std::cout << "GLFW error: " << E.what() << std::endl;
} catch (std::exception &E) {
  std::cout << "Standard error: " << E.what() << std::endl;
} catch (...) {
  std::cout << "Unknown error\n";
}
