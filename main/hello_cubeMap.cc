
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
#include "cude_map.h"

namespace gpr5300 {
class Hello_CubeMap final : public Scene {
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
  GLuint cubeVao_ = 0;
  GLuint cubeVbo_ = 0;
  GLuint vao_ = 0;
  GLuint vbo_ = 0;
  GLuint ebo_ = 0;
  GLuint textures_[32];


  Model* ourModel = nullptr;

  float elapsedTime_ = 0.0f;

  glm::vec3 cubePositions[10] = {};

  FreeCamera* camera_ = nullptr;


  Shader* ourShader = nullptr;
  Shader* skyboxShader = nullptr;

  unsigned int cubemapTexture;
};


void Hello_CubeMap::Begin() {

  ourShader = new Shader("data/shaders/hello_3d/hello_3d.vert", "data/shaders/hello_3d/hello_3d.frag");
  skyboxShader = new Shader("data/shaders/hello_cubeMap/cube_map.vert", "data/shaders/hello_cubeMap/cube_map.frag");
  camera_ = new FreeCamera();

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

  float skyboxVertices[] = {
      // positions
      -1.0f,  1.0f, -1.0f,
      -1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, -1.0f,
      1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,

      -1.0f, -1.0f,  1.0f,
      -1.0f, -1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f,  1.0f,
      -1.0f, -1.0f,  1.0f,

      1.0f, -1.0f, -1.0f,
      1.0f, -1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,
      1.0f,  1.0f, -1.0f,
      1.0f, -1.0f, -1.0f,

      -1.0f, -1.0f,  1.0f,
      -1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,
      1.0f, -1.0f,  1.0f,
      -1.0f, -1.0f,  1.0f,

      -1.0f,  1.0f, -1.0f,
      1.0f,  1.0f, -1.0f,
      1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,
      -1.0f,  1.0f,  1.0f,
      -1.0f,  1.0f, -1.0f,

      -1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f,  1.0f,
      1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f,  1.0f,
      1.0f, -1.0f,  1.0f
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

  // cube VAO
  glGenVertexArrays(1, &cubeVao_);
  glGenBuffers(1, &cubeVbo_);
  glBindVertexArray(cubeVao_);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));


  // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
  //stbi_set_flip_vertically_on_load(true);

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);




// skybox VAO
  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);
  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);




  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  cubemapTexture = loadCubemap(faces);

  ourShader->use();
  ourShader->setInt("skybox", 0);

  skyboxShader->use();
  skyboxShader->setInt("skybox", 0);
}

void Hello_CubeMap::End() {
  //Unload program/pipeline
  //glDeleteProgram(program_);

//  glDeleteShader(vertexShader_);
//  glDeleteShader(fragmentShader_);

  glDeleteVertexArrays(1, &vao_);
  glDeleteBuffers(1, &ebo_);
}

void Hello_CubeMap::Update(float dt) {
  elapsedTime_ += dt;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffers

  //glDepthMask(GL_FALSE);

  int textureIndex = static_cast<int>(elapsedTime_ * 10) % 11;

//  // Create transformations
//  glm::mat4 model = glm::mat4(1.0f); // Initialize matrix to identity matrix first
//  //glm::mat4 view = camera_->view();
//  glm::mat4 view = glm::mat4(glm::mat3(camera_->view()));
//  glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.1f, 100.0f);



//
//  skyboxShader->use();
//  glBindVertexArray(vao_);
//  // Pass view and projection matrices to shaders
//  unsigned int viewLoc = glGetUniformLocation(ourShader->ID, "view");
//  unsigned int projectionLoc = glGetUniformLocation(ourShader->ID, "projection");
//  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
//  glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
//
//
//  glBindVertexArray(vao_);
//
//  glBindVertexArray(cubeVao_);
//  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
//  glDrawArrays(GL_TRIANGLES, 0, 36);
//  glDepthMask(GL_TRUE);
//
//  ourShader->use();
//  ourShader->setMat4("model", model);
//  ourShader->setMat4("view", view);
//  ourShader->setMat4("projection", projection);
//
//  // Set shader and uniform variables
//
//  ourShader->setFloat("someUniform", 1.0f);
//
//  glBindTexture(GL_TEXTURE_2D, textures_[textureIndex]);
//
//   //Loop for drawing cubes
//  for (unsigned int i = 0; i < 10; i++) {
//    model = glm::mat4(1.0f); // Reset model matrix for each cube
//    model = glm::translate(model, cubePositions[i]);
//    float angle = 20.0f * i;
//    model = glm::rotate(model, glm::radians(angle) + elapsedTime_, glm::vec3(1.0f, 0.3f, 0.5f));
//
//    unsigned int modelLoc = glGetUniformLocation(ourShader->ID, "model");
//    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//
//    glDrawArrays(GL_TRIANGLES, 0, 36); // Draw the cube
//  }
//
//  glBindVertexArray(0);


  // draw scene as normal
  ourShader->use();
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = camera_->view();
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.1f, 100.0f);
  ourShader->setMat4("model", model);
  ourShader->setMat4("view", view);
  ourShader->setMat4("projection", projection);
  // cubes
  glBindVertexArray(cubeVao_);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textures_[textureIndex]);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);

  // draw skybox as last
  glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
  skyboxShader->use();
  view = glm::mat4(glm::mat3(camera_->view())); // remove translation from the view matrix
  skyboxShader->setMat4("view", view);
  skyboxShader->setMat4("projection", projection);
  // skybox cube
  glBindVertexArray(vao_);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS); // set depth function back to default

  // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
  // -------------------------------------------------------------------------------
  //glfwSwapBuffers();
}


void Hello_CubeMap::OnEvent(const SDL_Event& event, const float dt)
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

void Hello_CubeMap::DrawImGui()
{
  ImGui::Begin("My Window"); // Start a new window
  ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
  ImGui::End(); // End the window
}
}



//int main(int argc, char** argv)
//{
//  Camera camera;
//  gpr5300::Hello_CubeMap scene;
//  gpr5300::Engine engine(&scene);
//  engine.Run();
//
//  return EXIT_SUCCESS;
//}
