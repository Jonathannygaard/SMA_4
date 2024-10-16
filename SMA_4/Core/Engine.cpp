#include "Engine.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "Color.h"
#include "Shader/Shader.h"
#include "Window/Window.h"
#include "Window/Input/Input.h"
#include "GLFW/glfw3.h"

void Engine::Create()
{
    //Creating player and enemy entities and adding them to the entities vector
    player.ID = entities.size();
    entities.push_back(player);
    enemy.ID = entities.size();
    entities.push_back(enemy);
    
    //Creating player entity and adding components
    componentManager.AddComponent<MeshComponent>(&player);
    componentManager.AddComponent<PositionComponent>(&player);
    componentManager.AddComponent<MovementComponent>(&player);
    meshSystem.CreateCubeMesh(&player, Color::Green);

    //Setting up player entity
    componentManager.GetComponentHandler<MovementComponent>()->GetComponent(&player).Speed = 5.f;
    componentManager.GetComponentHandler<PositionComponent>()->GetComponent(&player).Position = glm::vec3(0.f, 0.f, -10.f);

    //Creating enemy entity and adding components
    componentManager.AddComponent<MeshComponent>(&enemy);
    componentManager.AddComponent<PositionComponent>(&enemy);
    componentManager.AddComponent<MovementComponent>(&enemy);
    meshSystem.CreateCubeMesh(&enemy, Color::Red);

    //Setting up enemy entity
    componentManager.GetComponentHandler<MovementComponent>()->GetComponent(&enemy).Speed = 3.f;
    componentManager.GetComponentHandler<PositionComponent>()->GetComponent(&enemy).Position = glm::vec3(0.f, 5.f, -10.f);
}

void Engine::setup()
{
    Window = Window::init();
    MainCamera.init();
    glLineWidth(5.f);
    Create();
}

void Engine::Draw()
{
    meshSystem.DrawMesh(&player);
    meshSystem.DrawMesh(&enemy);
}

void Engine::update()
{
    movementSystem.MoveEntity(&player);
    Draw();
}

void Engine::run()
{
    setup();
    float FirstFrame = 0.0f;
    
    glm::vec3 color(Color::Black);
    while(!glfwWindowShouldClose(Window))
    {
        const auto CurrentFrame = static_cast<float>(glfwGetTime());
        DeltaTime = CurrentFrame - FirstFrame;
        FirstFrame = CurrentFrame;
        
        glClearColor(color.x, color.y, color.z, 1.f);
        glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);
        glUseProgram(Shader::Program);
		  
        update();
		  
        glUniformMatrix4fv(MainCamera.projectionLoc, 1, GL_FALSE, glm::value_ptr(MainCamera.getProjection(Window::Width, Window::Height)));
        glUniformMatrix4fv(MainCamera.viewLoc, 1, GL_FALSE, glm::value_ptr(MainCamera.getView()));

        KeyBoardInput::processInput(Window, &player, &componentManager);
    
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }
    glfwTerminate();
}

