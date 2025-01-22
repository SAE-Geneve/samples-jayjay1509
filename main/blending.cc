#include <fstream>
#include <imgui.h>
#include <iostream>
#include <map>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine.h"
#include "file_utility.h"
#include "free_camera.h"
#include "model.h"
#include "scene.h"
#include "texture_loader.h"
#include "camera.h"

namespace gpr5300 {
class Blending final : public Scene {
 public:
  void Begin() override;
  void End() override;
  void Update(float dt) override;
  void OnEvent(const SDL_Event &event, const float dt) override;
  void DrawImGui() override;

 private:
  //GLuint vertexShader_ = 0;
  //GLuint fragmentShader_ = 0;
  //GLuint program_ = 0;

  GLuint cube_vao_ = 0;
  GLuint cube_vbo_ = 0;

  GLuint plane_vao_ = 0;
  GLuint plane_vbo_ = 0;

  GLuint glass_vao_ = 0;
  GLuint glass_vbo_ = 0;

  unsigned int cubeTexture = -1;
  unsigned int floorTexture = -1;
  unsigned int glassTexture = -1;



  float elapsedTime_ = 0.0f;

  float cube_vertices_[180] = {};
  float plane_vertices_[30] = {};
  float windows_vertices_[30] = {};

  std::vector<glm::vec3> windows_;

  FreeCamera *camera_ = nullptr;

  Shader *ourShader = nullptr;
};

void Blending::Begin() {
  camera_ = new FreeCamera();

  //Main program


  ourShader = new Shader("data/shaders/blending/blending.vert", "data/shaders/blending/blending.frag");



  // Configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float cubeVertices[] = {
      // positions          // texture Coords
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
  std::ranges::copy(cubeVertices, cube_vertices_);
  float planeVertices[] = {
      // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
      5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
      -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
      -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

      5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
      -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
      5.0f, -0.5f, -5.0f, 2.0f, 2.0f
  };

  std::ranges::copy(planeVertices, plane_vertices_);

  float transparentVertices[] = {
      // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
      0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
      0.0f, -0.5f, 0.0f, 0.0f, 1.0f,
      1.0f, -0.5f, 0.0f, 1.0f, 1.0f,

      0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
      1.0f, -0.5f, 0.0f, 1.0f, 1.0f,
      1.0f, 0.5f, 0.0f, 1.0f, 0.0f
  };

  std::ranges::copy(transparentVertices, windows_vertices_);

  // cube VAO
  glGenVertexArrays(1, &cube_vao_);
  glGenBuffers(1, &cube_vbo_);
  glBindVertexArray(cube_vao_);
  glBindBuffer(GL_ARRAY_BUFFER, cube_vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices_), &cube_vertices_, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));

  // plane VAO
  glGenVertexArrays(1, &plane_vao_);
  glGenBuffers(1, &plane_vbo_);
  glBindVertexArray(plane_vao_);
  glBindBuffer(GL_ARRAY_BUFFER, plane_vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices_), &plane_vertices_, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));

  //Glass VAO
  glGenVertexArrays(1, &glass_vao_);
  glGenBuffers(1, &glass_vbo_);
  glBindVertexArray(glass_vao_);
  glBindBuffer(GL_ARRAY_BUFFER, glass_vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(windows_vertices_), &windows_vertices_, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
  glBindVertexArray(0);

  // load textures
  // -------------
  cubeTexture = TextureFromFile("container2.png", "data/textures");
  floorTexture = TextureFromFile("marble.jpg", "data/textures");
  glassTexture = TextureFromFile("blending_transparent_window.png", "data/textures");

  windows_.emplace_back(-1.5f, 0.0f, -0.48f);
  windows_.emplace_back(1.5f, 0.0f, 0.51f);
  windows_.emplace_back(0.0f, 0.0f, 0.7f);
  windows_.emplace_back(-0.3f, 0.0f, -2.3f);
  windows_.emplace_back(0.5f, 0.0f, -0.6f);

  // shader configuration
  // --------------------
  glUseProgram(ourShader->ID);
  glUniform1i(glGetUniformLocation(ourShader->ID, "texture1"), 0);
}

void Blending::End() {
  //Unload program/pipeline
  glDeleteProgram(ourShader->ID);

  glDeleteShader(ourShader->ID);


  glDeleteVertexArrays(1, &cube_vao_);
  glDeleteVertexArrays(1, &plane_vao_);
  glDeleteVertexArrays(1, &glass_vao_);
}

void Blending::Update(const float dt) {

  elapsedTime_ += dt;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // also clear the depth buffer

  glUseProgram(ourShader->ID);

  auto model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
  auto view = camera_->view();
  auto projection = glm::perspective(glm::radians(45.0f), (float) 1280 / (float) 720, 0.1f, 100.0f);

  glUniformMatrix4fv(glGetUniformLocation(ourShader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(glGetUniformLocation(ourShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

  const glm::vec3 view_pos = camera_->camera_position_;
  glUniform3f(glGetUniformLocation(ourShader->ID, "viewPos"), view_pos.x, view_pos.y, view_pos.z);

  //DRAW OPAQUE OBJECTS FIRST
  //Cubes 1st pass
  glBindVertexArray(cube_vao_);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, cubeTexture);
  model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
  glUniformMatrix4fv(glGetUniformLocation(ourShader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, 36);
  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
  glUniformMatrix4fv(glGetUniformLocation(ourShader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // floor
  glBindVertexArray(plane_vao_);
  glBindTexture(GL_TEXTURE_2D, floorTexture);
  model = glm::mat4(1.0f);
  glUniformMatrix4fv(glGetUniformLocation(ourShader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, 6);

  //SORT TRANSPARENT OBJECTS (map automatically sorts from nearest (< distance) to furthest (> distance)
  //(could use a multimap in case we're afraid of having two objects at the exact same distance)
  std::map<float, glm::vec3> sorted;
  for (auto window : windows_) {
    float distance = glm::length(camera_->camera_position_ - window);
    sorted[distance] = window;
  }

  //DRAW TRANSPARENT OBJECTS in reverse order (because map is from near to far)
  //Glass
  glBindVertexArray(glass_vao_);
  glBindTexture(GL_TEXTURE_2D, glassTexture);
  for (auto it = sorted.rbegin(); it != sorted.rend(); ++it) {
    model = glm::mat4(1.0f);
    model = glm::translate(model, it->second);
    glUniformMatrix4fv(glGetUniformLocation(ourShader->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }

  glBindVertexArray(0);
}

void Blending::OnEvent(const SDL_Event &event, const float dt) {
  // Get keyboard state
  const Uint8 *state = SDL_GetKeyboardState(NULL);

  // Camera controls
  if (state[SDL_SCANCODE_W]) {
    camera_->Move(FORWARD, dt);
  }
  if (state[SDL_SCANCODE_S]) {
    camera_->Move(BACKWARD, dt);
  }
  if (state[SDL_SCANCODE_A]) {
    camera_->Move(LEFT, dt);
  }
  if (state[SDL_SCANCODE_D]) {
    camera_->Move(RIGHT, dt);
  }
  if (state[SDL_SCANCODE_SPACE]) {
    camera_->Move(UP, dt);
  }
  if (state[SDL_SCANCODE_LCTRL]) {
    camera_->Move(DOWN, dt);
  }

  int mouseX, mouseY;
  SDL_GetRelativeMouseState(&mouseX, &mouseY);
  camera_->Update(mouseX, mouseY);
}

void Blending::DrawImGui() {
  ImGui::Begin("My Window"); // Start a new window
  //ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
  static ImVec4 LightColour = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Default color
  //ImGui::ColorPicker3("Light Colour", reinterpret_cast<float*>(&light_colour_));
  ImGui::End(); // End the window
}

}



//int main(int argc, char** argv)
//{
//  Camera camera;
//  gpr5300::Blending scene;
//  gpr5300::Engine engine(&scene);
//  engine.Run();
//
//  return EXIT_SUCCESS;
//}
