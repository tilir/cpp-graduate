//-----------------------------------------------------------------------------
//
// Source code for MIPT ILab
// Slides: https://sourceforge.net/projects/cpp-lects-rus/files/cpp-graduate/
// Licensed after GNU GPL v3
//
//-----------------------------------------------------------------------------
//
//  Motion simple example: utilizing GLM and view / projection in vertex shader
//  This example extends ogl-frag-shader
//  Show how vertex lists are indexed in VBO's
//  Shows camera motion and zoom with mouse and keyboard GLFW
//
//  cl /EHsc ogl-motion.cc /link glad.lib glfw3dll.lib opengl32.lib
//
//----------------------------------------------------------------------------

#include <algorithm>
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

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <GLFW/glfw3.h>

// initial window sizes
constexpr int SZX = 600;
constexpr int SZY = 600;

constexpr const char *VERTNAME = "shaders/models.vert";
constexpr const char *FRAGNAME = "shaders/sincolor.frag";

constexpr glm::vec3 STARTPOS = {0.0f, 0.0f, 3.0f};
constexpr glm::vec3 STARTFRONT = {0.0f, 0.0f, 0.0f};
constexpr glm::vec3 STARTUP = {0.0f, 1.0f, 0.0f};

constexpr float Sensitivity = 0.2f;
constexpr float RadiusDelta = 0.02f;

// vertices to render
GLfloat Vertices[] = {
    // positions        // colors
    // for +Z (cube front)
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f,  // top left (-X, +Y)
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,   // top right (+X, +Y)
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,  // bottom right (+X, -Y)
    -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, // bottom left (-X, -Y)

    // for -Z (cube back)
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  // top left (-X, +Y)
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,   // top right (+X, +Y)
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right (+X, -Y)
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left (-X, -Y)
};

// cube sides to render
GLubyte Indices[] = {
    // clang-format off
    // those indices better to be in cube order
    // quads
    0, 3, 2, 1,
    4, 7, 3, 0,
    1, 2, 6, 5,
    4, 0, 1, 5,
    3, 7, 6, 2,
    5, 6, 7, 4,
    // clang-format on
};

// simplest approach: do you already see problems?
class Renderer {
  GLFWwindow *Wnd_;
  GLuint ProgID;
  GLuint VAO, VBO, IBO;

  // camera params: vertical angle, horizontal angle, radius
  float HorizontalAngle = 0.0f;
  float VerticalAngle = 0.0f;
  float Radius = 3.0f;

  // projection params: Field of view, Aspect, etc...
  float FoV = 45.0f;
  float Aspect = static_cast<float>(SZX) / SZY;
  float Near = 0.1f;
  float Far = 100.0f;

  // Mouse button params to make control easier
  bool LeftPress = false;
  double OldX, OldY;

  // Sceleton mode toggle support
  bool LineMode = false;

public:
  Renderer(GLFWwindow *Wnd);
  Renderer(const Renderer &) = delete;
  Renderer(Renderer &&) = delete;
  Renderer &operator=(const Renderer &) = delete;
  Renderer &operator=(Renderer &&) = delete;
  ~Renderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
  }
  void display() const;
  void notifyKey(int key, int scancode, int action, int mods);
  void notifyMouseMove(double xpos, double ypos);
  void notifyMouseButton(int button, int action, int mods);
  void notifyAspect(float w, float h) { Aspect = w / h; }
};

// global renderer here to redirect handlers
std::unique_ptr<Renderer> TheRenderer;

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

// { different trivial callbacks

void error_callback(int, const char *err_str) { throw glfw_error(err_str); }

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
  TheRenderer->notifyKey(key, scancode, action, mods);
}

static void cursor_position_callback(GLFWwindow *window, double xpos,
                                     double ypos) {
  TheRenderer->notifyMouseMove(xpos, ypos);
}

static void mouse_button_callback(GLFWwindow *window, int button, int action,
                                  int mods) {
  TheRenderer->notifyMouseButton(button, action, mods);
}

// }

// make sure the viewport matches the new window dimensions
static void framebuffer_size_callback(GLFWwindow *window, int width,
                                      int height) {
  TheRenderer->notifyAspect(width, height);
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

// set uniform value: float 4x4 matrix
void setFloatMatrix4(GLuint ProgID, const char *Name, float *Vals) {
  GLint Loc = glGetUniformLocation(ProgID, Name);
  glUniformMatrix4fv(Loc, 1, GL_FALSE, Vals);
}

// renderer ctor: all OpenGL initialization goes here
Renderer::Renderer(GLFWwindow *Wnd) : Wnd_(Wnd), ProgID(linkShaders()) {
  // viewport
  glViewport(0, 0, SZX, SZY);

  // depth
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // depth clamp
  glEnable(GL_DEPTH_CLAMP);

  // face culling
  glEnable(GL_CULL_FACE);

  // create vertex buffer arrays
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

  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices,
               GL_STATIC_DRAW);
  glBindVertexArray(VAO);
}

// render routine
void Renderer::display() const {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(ProgID);
  float CurTime = glfwGetTime();
  setFloat(ProgID, "time", CurTime);

  glm::mat4 Model = glm::mat4(1.0f);
  glm::mat4 View = glm::mat4(1.0f);
  glm::mat4 Projection = glm::mat4(1.0f);

  float Phi = glm::radians(90.0f - VerticalAngle);
  float Theta = glm::radians(HorizontalAngle + 180.0f);

  glm::vec3 Position(-sin(Phi) * sin(Theta) * Radius, cos(Phi) * Radius,
                     sin(Phi) * cos(Theta) * Radius);

  glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

  View = glm::lookAt(Position, glm::vec3(0.0f, 0.0f, 0.0f), Up);
  Projection = glm::perspective(glm::radians(FoV), Aspect, Near, Far);

  setFloatMatrix4(ProgID, "model", glm::value_ptr(Model));
  setFloatMatrix4(ProgID, "view", glm::value_ptr(View));
  setFloatMatrix4(ProgID, "projection", glm::value_ptr(Projection));
  glDrawElements(GL_QUADS, 4 * 6, GL_UNSIGNED_BYTE, (GLvoid *)0);
}

// Key control: zoom in/out
void Renderer::notifyKey(int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    if (key == GLFW_KEY_UP || key == GLFW_KEY_W)
      Radius -= RadiusDelta;
    if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S)
      Radius += RadiusDelta;
  }

  if (action == GLFW_PRESS) {
    if (key == GLFW_KEY_L) {
      LineMode = !LineMode;
      glPolygonMode(GL_FRONT_AND_BACK, LineMode ? GL_LINE : GL_FILL);
      if (LineMode) {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
      } else {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
      }
    }
  }
}

// WA for MSVS
template <typename T> T myclamp(T x, T low, T hi) {
  if (x > hi)
    return hi;
  if (x < low)
    return low;
  return x;
}

// We need to recognize move only if mouse is pressed
void Renderer::notifyMouseMove(double xpos, double ypos) {
  if (LeftPress) {
    float xoffset = xpos - OldX;
    float yoffset = ypos - OldY;
    OldX = xpos;
    OldY = ypos;
    HorizontalAngle += xoffset * Sensitivity;
    VerticalAngle += yoffset * Sensitivity;
    VerticalAngle = myclamp(VerticalAngle, -89.0f, 89.0f);
    HorizontalAngle = myclamp(HorizontalAngle, -89.0f, 89.0f);
  }
}

// Notify press/release, remember x/y
void Renderer::notifyMouseButton(int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    LeftPress = true;
    glfwGetCursorPos(Wnd_, &OldX, &OldY);
  }

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    LeftPress = false;
}

// entry point
int main() try {
  auto Cleanup = [](GLFWwindow *) { glfwTerminate(); };
  using UWnd = std::unique_ptr<GLFWwindow, decltype(Cleanup)>;
  UWnd Wnd(initialize_window(), Cleanup);
  TheRenderer = std::make_unique<Renderer>(Wnd.get());

  glfwSetKeyCallback(Wnd.get(), key_callback);
  glfwSetMouseButtonCallback(Wnd.get(), mouse_button_callback);
  glfwSetCursorPosCallback(Wnd.get(), cursor_position_callback);

  while (!glfwWindowShouldClose(Wnd.get())) {
    TheRenderer->display();
    glfwSwapBuffers(Wnd.get());
    glfwPollEvents();
  }

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
