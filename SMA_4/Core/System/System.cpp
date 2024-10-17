#include "System.h"

#include <iostream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "../Components/Component.h"
#include "../Shader/Shader.h"
#include "../Engine.h"

void MovementSystem::MoveEntity(Entity* entity)
{
    static_cast<ComponentHandler<PositionComponent>*>(componentmanager.Components[typeid(PositionComponent)])->GetComponent(entity).Position +=
        static_cast<ComponentHandler<MovementComponent>*>(componentmanager.Components[typeid(MovementComponent)])->GetComponent(entity).Movement *
            static_cast<ComponentHandler<MovementComponent>*>(componentmanager.Components[typeid(MovementComponent)])->GetComponent(entity).Speed
    * Engine::DeltaTime;
}

void NPCMovementSystem::MoveEntity(Entity* entity, Entity* target)
{
    CollisionSystem collisionSystem(componentmanager);
    static_cast<ComponentHandler<PositionComponent>*>(componentmanager.Components[typeid(PositionComponent)])->GetComponent(entity).Position +=
        static_cast<ComponentHandler<MovementComponent>*>(componentmanager.Components[typeid(MovementComponent)])->GetComponent(entity).Movement *
            static_cast<ComponentHandler<MovementComponent>*>(componentmanager.Components[typeid(MovementComponent)])->GetComponent(entity).Speed
    * Engine::DeltaTime;
}

void NPCMovementSystem::FindDirection(Entity* entity, Entity* target)
{
    static_cast<ComponentHandler<MovementComponent>*>(componentmanager.Components[typeid(MovementComponent)])->GetComponent(entity).Movement =
        glm::normalize(static_cast<ComponentHandler<PositionComponent>*>(componentmanager.Components[typeid(PositionComponent)])->GetComponent(target).Position -
            static_cast<ComponentHandler<PositionComponent>*>(componentmanager.Components[typeid(PositionComponent)])->GetComponent(entity).Position);
}

void MeshSystem::DrawMesh(Entity* entity)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, static_cast<ComponentHandler<PositionComponent>*>(componentmanager.Components[typeid(PositionComponent)])->GetComponent(entity).Position);
    glUniformMatrix4fv(glGetUniformLocation(Shader::Program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    MeshComponent& mesh = static_cast<ComponentHandler<MeshComponent>*>(componentmanager.Components[typeid(MeshComponent)])->GetComponent(entity);
    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.Indices.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void MeshSystem::BindBuffers(Entity* entity)
{
    ComponentHandler<MeshComponent>* componenthandler = componentmanager.GetComponentHandler<MeshComponent>();
    MeshComponent& mesh = componenthandler->GetComponent(entity);
    // VAO
    glGenBuffers(1, &mesh.VBO);

    // VAO
    glGenVertexArrays(1, &mesh.VAO);
    glBindVertexArray(mesh.VAO);

    // VBO
    glGenBuffers(1, &mesh.EBO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.Vertices.size() * sizeof(Vertex), mesh.Vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.Indices.size() * sizeof(Triangles), mesh.Indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Position)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Color)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void MeshSystem::CreateCubeMesh(Entity* entity, glm::vec3 color)
{
    ComponentHandler<MeshComponent>* componenthandler = componentmanager.GetComponentHandler<MeshComponent>();
    MeshComponent& mesh = componenthandler->GetComponent(entity);
    
    Vertex v0{glm::vec3(0.f, 0.f, 0.f), color}; /* Front-Bot-left */
    Vertex v1{ glm::vec3(1.f, 0.f, 0.f), color}; /* Front-Bot-right */
    Vertex v2{ glm::vec3(1.f, 1.f, 0.f), color}; /* Front-Top-right */
    Vertex v3{ glm::vec3(0.f, 1.f, 0.f), color}; /* Front-Top-left */
    Vertex v4{ glm::vec3(0.f, 0.f, -1.f),color}; /* Back-Bot-left */
    Vertex v5{ glm::vec3(1.f, 0.f, -1.f),color}; /* Back-Bot-right */
    Vertex v6{ glm::vec3(1.f, 1.f, -1.f),color}; /* Back-Top-right */
    Vertex v7{ glm::vec3(0.f, 1.f, -1.f),color}; /* Back-Top-left */

    mesh.Vertices.emplace_back(v0);
    mesh.Vertices.emplace_back(v1);
    mesh.Vertices.emplace_back(v2);
    mesh.Vertices.emplace_back(v3);
    mesh.Vertices.emplace_back(v4);
    mesh.Vertices.emplace_back(v5);
    mesh.Vertices.emplace_back(v6);
    mesh.Vertices.emplace_back(v7);

    /* Front */
    mesh.Indices.emplace_back(0,1,2);
    mesh.Indices.emplace_back(2,3,0);

    /* Right */
    mesh.Indices.emplace_back(1,5,6);
    mesh.Indices.emplace_back(6,2,1);

    /* Left */			   
    mesh.Indices.emplace_back(0,3,7);
    mesh.Indices.emplace_back(7,4,0);
						   
    /* Back */
    mesh.Indices.emplace_back(4,7,6);
    mesh.Indices.emplace_back(6,5,4);

    /* Top */
    mesh.Indices.emplace_back(3,2,6);
    mesh.Indices.emplace_back(6,7,3);

    /* Bottom */
    mesh.Indices.emplace_back(0,4,5);
    mesh.Indices.emplace_back(5,1,0);

    BindBuffers(entity);
}

bool CollisionSystem::CheckCollision(Entity* entity1, Entity* entity2)
{
    if(entity1 == nullptr || entity2 == nullptr)
    {
        return false;
    }
    if(entity1 == entity2)
    {
        return false;
    }
    glm::vec3 min1 = static_cast<ComponentHandler<CollisionComponent>*>(componentmanager.Components[typeid(CollisionComponent)])->GetComponent(entity1).min;
    glm::vec3 max1 = static_cast<ComponentHandler<CollisionComponent>*>(componentmanager.Components[typeid(CollisionComponent)])->GetComponent(entity1).max;
    glm::vec3 min2 = static_cast<ComponentHandler<CollisionComponent>*>(componentmanager.Components[typeid(CollisionComponent)])->GetComponent(entity2).min;
    glm::vec3 max2 = static_cast<ComponentHandler<CollisionComponent>*>(componentmanager.Components[typeid(CollisionComponent)])->GetComponent(entity2).max;
    
    if(min2.x < max1.x && max2.x > min1.x &&
        min2.y < max1.y && max2.y > min1.y &&
        max1.z < min2.z && min1.z > max2.z)
    {
        return true;
    }
    return false;
}

void CollisionSystem::UpdatePosition(Entity* entity)
{
    static_cast<ComponentHandler<CollisionComponent>*>(componentmanager.Components[typeid(CollisionComponent)])->GetComponent(entity).min =
        static_cast<ComponentHandler<PositionComponent>*>(componentmanager.Components[typeid(PositionComponent)])->GetComponent(entity).Position;

    static_cast<ComponentHandler<CollisionComponent>*>(componentmanager.Components[typeid(CollisionComponent)])->GetComponent(entity).max =
        static_cast<ComponentHandler<PositionComponent>*>(componentmanager.Components[typeid(PositionComponent)])->GetComponent(entity).Position
            + glm::vec3 (1.f, 1.f, -1.f);
}

void CombatSystem::Attack(Entity* entity1, Entity* entity2)
{
    if(static_cast<ComponentHandler<CombatComponent>*>(componentmanager.Components[typeid(CombatComponent)])->GetComponent(entity1).delay <= 0)
    {
        TakeDamage(entity2, static_cast<ComponentHandler<CombatComponent>*>(componentmanager.Components[typeid(CombatComponent)])->GetComponent(entity1).damage);
        static_cast<ComponentHandler<CombatComponent>*>(componentmanager.Components[typeid(CombatComponent)])->GetComponent(entity1).delay = 1.f;
    }
}

void CombatSystem::TakeDamage(Entity* entity, int damage)
{
    static_cast<ComponentHandler<HealthComponent>*>(componentmanager.Components[typeid(HealthComponent)])->GetComponent(entity).Health -= damage;
}

void CombatSystem::DelayTimer(Entity* entity)
{
    if(static_cast<ComponentHandler<CombatComponent>*>(componentmanager.Components[typeid(CombatComponent)])->GetComponent(entity).delay >= 0)
    {
        static_cast<ComponentHandler<CombatComponent>*>(componentmanager.Components[typeid(CombatComponent)])->GetComponent(entity).delay -= Engine::DeltaTime;
    }
}


