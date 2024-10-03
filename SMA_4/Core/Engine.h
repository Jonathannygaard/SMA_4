#pragma once
#include <GLFW/glfw3.h>
#include "Window/Camera/Camera.h"

class Engine
{
public:
  GLFWwindow *Window;
  static Camera MainCamera;
  static float DeltaTime;

  void setup();
  void update();
  void run();
    
};
