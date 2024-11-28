// engine.h
#pragma once

#include <SDL.h>
#include <glm/vec2.hpp>
#include "scene.h"
#include "camera.h"

namespace gpr5300
{
extern bool keys[1024];

class Engine
{
 public:
  Engine(Scene* scene, Camera& camera);
  void Run();
 private:
  void Begin();
  void End();

  SDL_Window* window_{nullptr};
  SDL_GLContext glRenderContext_{nullptr};
  Scene* scene_{nullptr};
  Camera& camera_;
};
}
