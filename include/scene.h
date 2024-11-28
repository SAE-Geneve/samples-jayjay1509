#pragma once

#include <SDL.h>
#include "camera.h"

namespace gpr5300
{

class Scene
{
 public:
  Scene(Camera& camera) : camera_(camera) {}
  virtual ~Scene() = default;
  virtual void Begin() = 0;
  virtual void End() = 0;
  virtual void Update(float dt) = 0;
  virtual void DrawImGui() {}
  virtual void OnEvent(const SDL_Event& event) {}

 protected:
  Camera& camera_;  // Référence à Camera
};

} // namespace gpr5300
