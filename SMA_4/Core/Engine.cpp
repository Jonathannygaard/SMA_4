#include "Engine.h"
#include <iostream>
#include <ostream>
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
    for(int AmountOfEntities = 0; AmountOfEntities < 4; AmountOfEntities++)
    {
        entities.emplace_back(entities.size());
    }

    srand(time(NULL));
    //Adding components to entities
    for(Entity entity : entities)
    {
        componentManager.AddComponent<MeshComponent>(&entity);
        componentManager.AddComponent<PositionComponent>(&entity);
        componentManager.AddComponent<MovementComponent>(&entity);
        componentManager.AddComponent<CollisionComponent>(&entity);
        componentManager.AddComponent<HealthComponent>(&entity);
        if(entity.ID == 0)
        {
            meshSystem.CreateCubeMesh(&entity, Color::Green);
            componentManager.GetComponentHandler<MovementComponent>()->GetComponent(&entity).Speed = 5.f;
        }
        else
        {
            meshSystem.CreateCubeMesh(&entity, Color::Red);
            componentManager.AddComponent<CombatComponent>(&entity);
            componentManager.GetComponentHandler<MovementComponent>()->GetComponent(&entity).Speed = 3.f;
            componentManager.GetComponentHandler<CombatComponent>()->GetComponent(&entity).damage = 1;
        }
        componentManager.GetComponentHandler<HealthComponent>()->GetComponent(&entity).Health = 5;
        componentManager.GetComponentHandler<PositionComponent>()->GetComponent(&entity).Position =
                glm::vec3(rand()%18-9, rand()%10-5, -10.f);
    }
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
    for(Entity entity : entities)
    {
        meshSystem.DrawMesh(&entity);
    }
}

void Engine::update()
{
    for(Entity entity: entities)
    {
        collisionSystem.UpdatePosition(&entity);
        if(entity.ID != 0)
        {
            if(collisionSystem.CheckCollision(&entities[0], &entity))
            {
                combatSystem.Attack(&entity, &entities[0]);
                static_cast<ComponentHandler<MovementComponent>*>(componentManager.Components[typeid(MovementComponent)])->
                GetComponent(&entity).Movement = glm::vec3(0.f);
            }
            else
            {
                npcMovementSystem.FindDirection(&entity, &entities[0]);
                npcMovementSystem.MoveEntity(&entity, &entities[0]);
            }
            combatSystem.DelayTimer(&entity);
        }
    }
    movementSystem.MoveEntity(&entities[0]);
    Draw();
}

void Engine::run()
{
    setup();
    float FirstFrame = 0.0f;
    float secondcounter = 0.f;
    glm::vec3 color(Color::Black);
    while(!glfwWindowShouldClose(Window))
    {
        const auto CurrentFrame = static_cast<float>(glfwGetTime());
        DeltaTime = CurrentFrame - FirstFrame;
        FirstFrame = CurrentFrame;
        secondcounter += DeltaTime;
        if(secondcounter >= 1.f)
        {
            secondcounter = 0.f;
            std::cout << "Player Health: " << componentManager.GetComponentHandler<HealthComponent>()->GetComponent(&entities[0]).Health << std::endl;
        }
        
        glClearColor(color.x, color.y, color.z, 1.f);
        glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);
        glUseProgram(Shader::Program);
		  
        update();
		  
        glUniformMatrix4fv(MainCamera.projectionLoc, 1, GL_FALSE, glm::value_ptr(MainCamera.getProjection(Window::Width, Window::Height)));
        glUniformMatrix4fv(MainCamera.viewLoc, 1, GL_FALSE, glm::value_ptr(MainCamera.getView()));

        KeyBoardInput::processInput(Window, &entities[0], &componentManager);
    
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }
    glfwTerminate();
}

