
#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine.h"
#include "file_utility.h"
#include "scene.h"
#include "texture_loader.h"
#include "camera.h"

namespace gpr5300 {
class HelloTriangle final : public Scene {
 public:
  void Begin() override;
  void End() override;
  void Update(float dt) override;
  HelloTriangle(Camera& camera) : Scene(camera), camera_(camera) {}

 private:
  GLuint vertexShader_ = 0;
  GLuint fragmentShader_ = 0;
  GLuint program_ = 0;
  GLuint vao_ = 0;
  GLuint vbo_ = 0;
  GLuint ebo_ = 0;
  unsigned int texture_;

  float elapsedTime_ = 0.0f;

  glm::vec3 cubePositions[10] = {};

  Camera& camera_;
};

void HelloTriangle::Begin() {
  TextureManager texture_manager;
  texture_ = texture_manager.CreateTexture("data/textures/container.jpg");

  camera_ = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

  //Load shaders
  const auto vertexContent = LoadFile("data/shaders/hello_3d/hello_3d.vert");
  const auto *ptr = vertexContent.data();
  vertexShader_ = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader_, 1, &ptr, nullptr);
  glCompileShader(vertexShader_);
  //Check success status of shader compilation
  GLint success;
  glGetShaderiv(vertexShader_, GL_COMPILE_STATUS, &success);
  if (!success) {
    std::cerr << "Error while loading vertex shader\n";
  }
  const auto fragmentContent = LoadFile("data/shaders/hello_3d/hello_3d.frag");
  ptr = fragmentContent.data();
  fragmentShader_ = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader_, 1, &ptr, nullptr);
  glCompileShader(fragmentShader_);
  //Check success status of shader compilation

  glGetShaderiv(fragmentShader_, GL_COMPILE_STATUS, &success);
  if (!success) {
    std::cerr << "Error while loading fragment shader\n";
  }
  //Load program/pipeline
  program_ = glCreateProgram();
  glAttachShader(program_, vertexShader_);
  glAttachShader(program_, fragmentShader_);
  glLinkProgram(program_);
  //Check if shader program was linked correctly

  glGetProgramiv(program_, GL_LINK_STATUS, &success);
  if (!success) {
    std::cerr << "Error while linking shader program\n";
  }

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
      0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
      0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

      -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
      -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
      -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
      0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
      -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
  };

  cubePositions[0] = glm::vec3(0.0f, 0.0f, 0.0f);
  cubePositions[1] = glm::vec3(2.0f, 5.0f, -15.0f);
  cubePositions[2] = glm::vec3(-1.5f, -2.2f, -2.5f);
  cubePositions[3] = glm::vec3(-3.8f, -2.0f, -12.3f);
  cubePositions[4] = glm::vec3(2.4f, -0.4f, -3.5f);
  cubePositions[5] = glm::vec3(-1.7f, 3.0f, -7.5f);
  cubePositions[6] = glm::vec3(1.3f, -2.0f, -2.5f);
  cubePositions[7] = glm::vec3(1.5f, 2.0f, -2.5f);
  cubePositions[8] = glm::vec3(1.5f, 0.2f, -1.5f);
  cubePositions[9] = glm::vec3(-1.3f, 1.0f, -1.5f);

  unsigned int indices[] = {
      0, 1, 3,
      1, 2, 3
  };

  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);
  glGenBuffers(1, &ebo_);

  // Bind VAO
  glBindVertexArray(vao_);

  // Bind and set VBO
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Bind and set EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(0);

  // TexCoord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Unbind VAO (optional)
  glBindVertexArray(0);
}

void HelloTriangle::End() {
  //Unload program/pipeline
  glDeleteProgram(program_);

  glDeleteShader(vertexShader_);
  glDeleteShader(fragmentShader_);

  glDeleteVertexArrays(1, &vao_);
  glDeleteBuffers(1, &ebo_);
}

void HelloTriangle::Update(float dt) {
  elapsedTime_ += dt;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffers

  // Create transformations
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = camera_.GetViewMatrix(); // Matrice de vue à partir de la caméra
  glm::mat4 projection = glm::perspective(glm::radians(camera_.Zoom), (float) 800 / (float) 600, 0.1f, 100.0f);
  model = glm::rotate(model, elapsedTime_, glm::vec3(0.5f, 1.0f, 0.0f));

  // retrieve the matrix uniform locations
  unsigned int viewLoc = glGetUniformLocation(program_, "view");
  unsigned int projectionLoc = glGetUniformLocation(program_, "projection");

  // Pass transformations to shaders
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

  // Draw program
  glUseProgram(program_);
  glBindTexture(GL_TEXTURE_2D, texture_);
  glBindVertexArray(vao_);

  for (unsigned int i = 0; i < 10; i++) {
    // Calculate the model matrix for each object and pass it to the shader before drawing
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cubePositions[i]);
    float angle = 20.0f * i;
    model = glm::rotate(model, glm::radians(angle) + elapsedTime_, glm::vec3(1.0f, 0.3f, 0.5f));
    unsigned int modelLoc = glGetUniformLocation(program_, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
  glBindVertexArray(0);
}

}

int main(int argc, char** argv)
{
  Camera camera;
  gpr5300::HelloTriangle scene(camera);
  gpr5300::Engine engine(&scene, camera);
  engine.Run();

  return EXIT_SUCCESS;
}