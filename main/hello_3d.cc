
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
#include "imgui.h"
#include "free_camera.h"
#include "shader.h"
#include "model.h"

namespace gpr5300 {
class Hello_3d final : public Scene {
 public:
  void Begin() override;
  void End() override;
  void Update(float dt) override;
  void OnEvent(const SDL_Event& event, const float dt) override;
  void DrawImGui() override;
  //HelloTriangle(Camera& camera) : Scene(camera), camera_(camera) {}

 private:
//  GLuint vertexShader_ = 0;
//  GLuint fragmentShader_ = 0;
  //GLuint program_ = 0;
  GLuint vao_ = 0;
  GLuint vbo_ = 0;
  GLuint ebo_ = 0;
  GLuint textures_[32];

  Model* ourModel = nullptr;

  float elapsedTime_ = 0.0f;

  glm::vec3 cubePositions[10] = {};

  FreeCamera* camera_ = nullptr;


  Shader* ourShader = nullptr;
};


void Hello_3d::Begin() {

  ourShader = new Shader("data/shaders/hello_3d/hello_3d.vert", "data/shaders/hello_3d/hello_3d.frag");
  camera_ = new FreeCamera();

  ourModel = new Model(("data/model/bag/backpack.obj"));


  TextureManager texture_manager;

  textures_[0] = texture_manager.CreateTexture("data/textures/output_frames/frame_0.jpg");
  textures_[1] = texture_manager.CreateTexture("data/textures/output_frames/frame_1.jpg");
  textures_[2] = texture_manager.CreateTexture("data/textures/output_frames/frame_2.jpg");
  textures_[3] = texture_manager.CreateTexture("data/textures/output_frames/frame_3.jpg");
  textures_[4] = texture_manager.CreateTexture("data/textures/output_frames/frame_4.jpg");
  textures_[5] = texture_manager.CreateTexture("data/textures/output_frames/frame_5.jpg");
  textures_[6] = texture_manager.CreateTexture("data/textures/output_frames/frame_6.jpg");
  textures_[7] = texture_manager.CreateTexture("data/textures/output_frames/frame_7.jpg");
  textures_[8] = texture_manager.CreateTexture("data/textures/output_frames/frame_8.jpg");
  textures_[9] = texture_manager.CreateTexture("data/textures/output_frames/frame_9.jpg");
  textures_[10] = texture_manager.CreateTexture("data/textures/output_frames/frame_10.jpg");
  textures_[11] = texture_manager.CreateTexture("data/textures/output_frames/frame_11.jpg");


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


  // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
  stbi_set_flip_vertically_on_load(true);

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

}

void Hello_3d::End() {
  //Unload program/pipeline
  //glDeleteProgram(program_);

//  glDeleteShader(vertexShader_);
//  glDeleteShader(fragmentShader_);

  glDeleteVertexArrays(1, &vao_);
  glDeleteBuffers(1, &ebo_);
}

void Hello_3d::Update(float dt) {
  elapsedTime_ += dt;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffers

  int textureIndex = static_cast<int>(elapsedTime_ * 10) % 11;

  // Create transformations
  glm::mat4 model = glm::mat4(1.0f); // Initialize matrix to identity matrix first
  glm::mat4 view = camera_->view();
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.1f, 100.0f);

  // Set shader and uniform variables
  ourShader->use();
  ourShader->setFloat("someUniform", 1.0f);

  // Pass view and projection matrices to shaders
  unsigned int viewLoc = glGetUniformLocation(ourShader->ID, "view");
  unsigned int projectionLoc = glGetUniformLocation(ourShader->ID, "projection");
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

  glBindTexture(GL_TEXTURE_2D, textures_[textureIndex]);
  glBindVertexArray(vao_);

  // Loop for drawing cubes
  for (unsigned int i = 0; i < 10; i++) {
    model = glm::mat4(1.0f); // Reset model matrix for each cube
    model = glm::translate(model, cubePositions[i]);
    float angle = 20.0f * i;
    model = glm::rotate(model, glm::radians(angle) + elapsedTime_, glm::vec3(1.0f, 0.3f, 0.5f));

    unsigned int modelLoc = glGetUniformLocation(ourShader->ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glDrawArrays(GL_TRIANGLES, 0, 36); // Draw the cube
  }

  glBindVertexArray(0);


  glm::mat4 model_ = glm::mat4(1.0f);
  model_ = glm::translate(model_, glm::vec3(0.0f, 0.0f, 0.0f));
  model_ = glm::scale(model_, glm::vec3(1.0f, 1.0f, 1.0f));

  ourShader->setMat4("model", model_);
  ourModel->Draw(*ourShader);
}


void Hello_3d::OnEvent(const SDL_Event& event, const float dt)
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
  SDL_GetRelativeMouseState(&mouseX, &mouseY);
  camera_->Update(mouseX, mouseY);
}

void Hello_3d::DrawImGui()
{
  ImGui::Begin("My Window"); // Start a new window
  ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
  ImGui::End(); // End the window
}
}



int main(int argc, char** argv)
{
  Camera camera;
  gpr5300::Hello_3d scene;
  gpr5300::Engine engine(&scene);
  engine.Run();

  return EXIT_SUCCESS;
}
