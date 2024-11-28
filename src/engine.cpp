// engine.cpp
#include "engine.h"
#include <GL/glew.h>
#include <glm/vec2.hpp>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <cassert>
#include <chrono>

namespace gpr5300
{
bool keys[1024] = { false };

void KeyCallback(SDL_Event& event)
{
  if (event.type == SDL_KEYDOWN)
  {
    if (event.key.keysym.sym < 1024)
    {
      keys[event.key.keysym.sym] = true;
    }
  }
  else if (event.type == SDL_KEYUP)
  {
    if (event.key.keysym.sym < 1024)
    {
      keys[event.key.keysym.sym] = false;
    }
  }
}


void HandleMouseMovement(SDL_Event& event, Camera& camera)
{
  if (event.type == SDL_MOUSEMOTION)
  {
    float xoffset = event.motion.xrel;
    float yoffset = event.motion.yrel;
    camera.ProcessMouseMovement(xoffset, yoffset);
  }
}

void HandleMouseScroll(SDL_Event& event, Camera& camera)
{
  if (event.type == SDL_MOUSEWHEEL)
  {
    camera.ProcessMouseScroll(event.wheel.y);
  }
}

void DoMovement(float deltaTime, Camera& camera)
{
  if (keys[SDLK_w])
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (keys[SDLK_s])
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (keys[SDLK_a])
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (keys[SDLK_d])
    camera.ProcessKeyboard(RIGHT, deltaTime);
}

Engine::Engine(Scene* scene, Camera& camera) : scene_(scene), camera_(camera)
{
}

void Engine::Run()
{
  Begin();
  bool isOpen = true;
  SDL_ShowCursor(SDL_FALSE);
  SDL_SetRelativeMouseMode(SDL_TRUE);
  bool isImGuiVisible = true;

  std::chrono::time_point<std::chrono::system_clock> clock = std::chrono::system_clock::now();
  while (isOpen)
  {
    const auto start = std::chrono::system_clock::now();
    using seconds = std::chrono::duration<float, std::ratio<1, 1>>;
    const auto dt = std::chrono::duration_cast<seconds>(start - clock);
    clock = start;

    //Manage SDL event
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      KeyCallback(event);  // Add this line to handle key events
      switch (event.type)
      {
        case SDL_QUIT:
          isOpen = false;
          break;
        case SDL_WINDOWEVENT:
        {
          switch (event.window.event)
          {
            case SDL_WINDOWEVENT_CLOSE:
              isOpen = false;
              break;
            case SDL_WINDOWEVENT_RESIZED:
            {
              glm::uvec2 newWindowSize;
              newWindowSize.x = event.window.data1;
              newWindowSize.y = event.window.data2;

              // Mettre à jour la taille du viewport OpenGL
              glViewport(0, 0, newWindowSize.x, newWindowSize.y);

              // Optionnel : mettre à jour la caméra si nécessaire
              // Exemple pour une caméra utilisant une projection perspective
              float aspectRatio = static_cast<float>(newWindowSize.x) / static_cast<float>(newWindowSize.y);
              camera_.UpdateProjectionMatrix(aspectRatio);

              break;
            }
            default:
              break;
          }
          break;

          case SDL_MOUSEMOTION:
            // Gérer le mouvement de la souris
            HandleMouseMovement(event, camera_);
          break;

          case SDL_MOUSEWHEEL:
            // Gérer le défilement de la souris
            HandleMouseScroll(event, camera_);
          break;

          case SDL_KEYDOWN:
            // Vérifier si la touche TAB est appuyée
            if (event.key.keysym.sym == SDLK_TAB)
            {
              isImGuiVisible = !isImGuiVisible;  // Alterner l'état d'ImGui
              if (isImGuiVisible)
              {
                // Afficher ImGui et désactiver le mode relatif de la souris
                SDL_ShowCursor(SDL_FALSE);
                SDL_SetRelativeMouseMode(SDL_TRUE);
              }
              else
              {
                // Cacher ImGui et activer le mode relatif de la souris
                SDL_ShowCursor(SDL_TRUE);
                SDL_SetRelativeMouseMode(SDL_FALSE);
              }
            }
        }
        default:
          break;
      }
      scene_->OnEvent(event);
      ImGui_ImplSDL2_ProcessEvent(&event);
    }
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    DoMovement(dt.count(), camera_);


    scene_->Update(dt.count());

    //Generate new ImGui frame
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplSDL2_NewFrame();
      ImGui::NewFrame();
      scene_->DrawImGui();
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


    SDL_GL_SwapWindow(window_);
  }
  End();
}

void Engine::Begin()
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
  // Set our OpenGL version.
#if true
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
#else
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#endif

  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  constexpr auto windowSize = glm::ivec2(1280, 720);
  window_ = SDL_CreateWindow(
      "GPR5300",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      windowSize.x,
      windowSize.y,
      SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
  );
  glRenderContext_ = SDL_GL_CreateContext(window_);
  //setting vsync
  SDL_GL_SetSwapInterval(1);

  if (GLEW_OK != glewInit())
  {
    assert(false && "Failed to initialize OpenGL context");
  }

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;

  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  // Setup Dear ImGui style
  //ImGui::StyleColorsDark();
  ImGui::StyleColorsClassic();
  ImGui_ImplSDL2_InitForOpenGL(window_, glRenderContext_);
  ImGui_ImplOpenGL3_Init("#version 300 es");

  scene_->Begin();
}

void Engine::End()
{
  scene_->End();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  SDL_GL_DeleteContext(glRenderContext_);
  SDL_DestroyWindow(window_);
  SDL_Quit();
}
} // namespace gpr5300
