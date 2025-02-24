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

namespace gpr5300
{
class Framebuffers final : public Scene
{
 public:
  void Begin() override;
  void End() override;
  void Update(float dt) override;
  void OnEvent(const SDL_Event& event,const float dt) override;
  void DrawImGui() override;


 private:
//  GLuint program_ = 0;
//  GLuint vertexShader_ = 0;
//  GLuint fragmentShader_ = 0;

  //GLuint screen_program_ = 0;
  GLuint screen_vertexShader_ = 0;
  GLuint screen_fragmentShader_ = 0;

  GLuint cube_vao_ = 0;
  GLuint cube_vbo_ = 0;

  GLuint plane_vao_ = 0;
  GLuint plane_vbo_ = 0;

  GLuint quad_vao_ = 0;

  GLuint fbo_ = 0;
  GLuint textureColourBuffer_ = 0;

  unsigned int cubeTexture = -1;
  unsigned int floorTexture = -1;

  float elapsedTime_ = 0.0f;

  float cube_vertices_[180] = {};
  float quad_vertices_[24] = {};
  float plane_vertices_[30] = {};

  FreeCamera* camera_ = nullptr;
  Shader* shader_ = nullptr;
  Shader* shader_screen = nullptr;
};

void Framebuffers::Begin()
{
  camera_ = new FreeCamera();

  shader_ = new Shader("data/shaders/hello_depth/depth.vert","data/shaders/hello_depth/depth.frag");
  shader_screen = new Shader("data/shaders/hello_frame/framebuffers.vert","data/shaders/hello_frame/framebuffers.frag");

  //Main program



  // Configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
  // glCullFace(GL_FRONT);

  float cubeVertices[] = {
      // Back face
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Bottom-left
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // bottom-right
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
      // Front face
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
      0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // top-right
      0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // top-right
      -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, // top-left
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
      // Left face
      -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-right
      -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-left
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-right
      -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-right
      // Right face
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-left
      0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-right
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
      0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-right
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // top-left
      0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
      // Bottom face
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // top-left
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-left
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-left
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-right
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
      // Top face
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
      -0.5f, 0.5f, 0.5f, 0.0f, 0.0f // bottom-left
  };
  std::ranges::copy(cubeVertices, cube_vertices_);

  float quadVertices[] = {
      // positions   // texCoords
      -1.0f,  1.0f,  0.0f, 1.0f,
      -1.0f, -1.0f,  0.0f, 0.0f,
      1.0f, -1.0f,  1.0f, 0.0f,

      -1.0f,  1.0f,  0.0f, 1.0f,
      1.0f, -1.0f,  1.0f, 0.0f,
      1.0f,  1.0f,  1.0f, 1.0f
  };
  std::ranges::copy(quadVertices, quad_vertices_);

  float planeVertices[] = {
      // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)

      -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
      5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
      5.0f, -0.5f, -5.0f, 2.0f, 2.0f,
      5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
      -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
      -5.0f, -0.5f, 5.0f, 0.0f, 0.0f
  };

  std::ranges::copy(planeVertices, plane_vertices_);


  // cube VAO
  glGenVertexArrays(1, &cube_vao_);
  glGenBuffers(1, &cube_vbo_);
  glBindVertexArray(cube_vao_);
  glBindBuffer(GL_ARRAY_BUFFER, cube_vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices_), &cube_vertices_, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

  // plane VAO
  glGenVertexArrays(1, &plane_vao_);
  glGenBuffers(1, &plane_vbo_);
  glBindVertexArray(plane_vao_);
  glBindBuffer(GL_ARRAY_BUFFER, plane_vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices_), &plane_vertices_, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

  //Quad VAO
  glGenVertexArrays(1, &quad_vao_);
  glGenBuffers(1, &quad_vao_);
  glBindVertexArray(quad_vao_);
  glBindBuffer(GL_ARRAY_BUFFER, quad_vao_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices_), &quad_vertices_, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

  glGenFramebuffers(1, &fbo_);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

  glGenTextures(1, &textureColourBuffer_);
  glBindTexture(GL_TEXTURE_2D, textureColourBuffer_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColourBuffer_, 0);

  unsigned int rbo;
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  //Create depth & stencil renderbuffer object
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 720);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
    std::cerr << "Error: Framebuffer incomplete\n";
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // load textures
  // -------------
  cubeTexture = TextureFromFile("container.jpg", "data/textures");
  floorTexture = TextureFromFile("marble.jpg", "data/textures");

  // shader configuration
  // --------------------
  glUseProgram(shader_->ID);
  glUniform1i(glGetUniformLocation(shader_->ID, "texture1"), 0);

  // draw as wireframe
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Framebuffers::End()
{
  //Unload program/pipeline
  glDeleteProgram(shader_->ID);

  glDeleteVertexArrays(1, &cube_vao_);
  glDeleteVertexArrays(1, &plane_vao_);
  glDeleteFramebuffers(1, &fbo_);
}

void Framebuffers::Update(const float dt)
{

  elapsedTime_ += dt;

  //First pass, in the fbo
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(shader_->ID);

  auto model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
  auto view = camera_->view();
  auto projection = glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.1f, 100.0f);

  glUniformMatrix4fv(glGetUniformLocation(shader_->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(glGetUniformLocation(shader_->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

  const glm::vec3 view_pos = camera_->camera_position_;
  glUniform3f(glGetUniformLocation(shader_->ID, "viewPos"), view_pos.x, view_pos.y, view_pos.z);

  //Cubes
  glBindVertexArray(cube_vao_);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, cubeTexture);
  model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
  glUniformMatrix4fv(glGetUniformLocation(shader_->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, 36);
  model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
  glUniformMatrix4fv(glGetUniformLocation(shader_->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, 36);
  //Floor
  glBindVertexArray(plane_vao_);
  glBindTexture(GL_TEXTURE_2D, floorTexture);
  model = glm::mat4(1.0f);
  glUniformMatrix4fv(glGetUniformLocation(shader_->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
  glDrawArrays(GL_TRIANGLES, 0, 6);

  //Second pass, out of the fbo
  glBindFramebuffer(GL_FRAMEBUFFER, 0); //Default
  glDisable(GL_DEPTH_TEST);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(shader_screen->ID);
  glBindVertexArray(quad_vao_);
  glBindTexture(GL_TEXTURE_2D, textureColourBuffer_);
  glDrawArrays(GL_TRIANGLES, 0, 6);


  glBindVertexArray(0);
}

void Framebuffers::OnEvent(const SDL_Event& event,const float dt)
{
// Get keyboard state
  const Uint8* state = SDL_GetKeyboardState(NULL);

  // Camera controls
  if (state[SDL_SCANCODE_W])
  {
    camera_->Move(FORWARD, dt);
  }
  if (state[SDL_SCANCODE_S])
  {
    camera_->Move(BACKWARD, dt);
  }
  if (state[SDL_SCANCODE_A])
  {
    camera_->Move(LEFT, dt);
  }
  if (state[SDL_SCANCODE_D])
  {
    camera_->Move(RIGHT, dt);
  }
  if (state[SDL_SCANCODE_SPACE])
  {
    camera_->Move(UP, dt);
  }
  if (state[SDL_SCANCODE_LCTRL])
  {
    camera_->Move(DOWN, dt);
  }

  int mouseX, mouseY;
  const Uint32 mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);
  if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT) && !ImGui::GetIO().WantCaptureMouse)
  {
    camera_->Update(mouseX, mouseY);
  }
}


void Framebuffers::DrawImGui()
{
  ImGui::Begin("My Window");
  static ImVec4 LightColour = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Default color
  ImGui::End();
}
}

//int main(int argc, char** argv)
//{
//  gpr5300::Framebuffers scene;
//  gpr5300::Engine engine(&scene);
//  engine.Run();
//
//  return EXIT_SUCCESS;
//}

