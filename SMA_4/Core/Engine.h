#pragma once
#include "Components/Component.h"
#include "System/System.h"
#include "Window/Camera/Camera.h"

struct GLFWwindow;

class Engine
{
public:
  GLFWwindow *Window;
  static inline Camera MainCamera;
  static inline float DeltaTime;

  ComponentManager componentManager;
  MeshSystem meshSystem;
  MovementSystem movementSystem;

  void Create();
  void setup();
  void update();
  void run();
  void Draw();

  std::vector<Entity> entities;
    
  Entity player;
  Entity enemy;

  
  Engine() : meshSystem(componentManager), movementSystem(componentManager) {}

 
    
};
