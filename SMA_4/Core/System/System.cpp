#include "System.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "../Components/Component.h"
#include "../Shader/Shader.h"

void MovementSystem::MoveEntity(Entity* entity)
{
    static_cast<ComponentHandler<PositionComponent>*>(componentmanager.Components[typeid(PositionComponent)])->GetComponent(entity).Position +=
        static_cast<ComponentHandler<MovementComponent>*>(componentmanager.Components[typeid(MovementComponent)])->GetComponent(entity).Movement;
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

void MeshSystem::CreateCubeMesh(Entity* entity)
{
    ComponentHandler<MeshComponent>* componenthandler = componentmanager.GetComponentHandler<MeshComponent>();
    MeshComponent& mesh = componenthandler->GetComponent(entity);
    
    Vertex v0{glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f)}; /* Front-Bot-left */
    Vertex v1{ glm::vec3(1.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f)}; /* Front-Bot-right */
    Vertex v2{ glm::vec3(1.f, 1.f, 0.f), glm::vec3(1.f, 0.f, 0.f)}; /* Front-Top-right */
    Vertex v3{ glm::vec3(0.f, 1.f, 0.f), glm::vec3(1.f, 0.f, 0.f)}; /* Front-Top-left */
    Vertex v4{ glm::vec3(0.f, 0.f, -1.f), glm::vec3(1.f, 0.f, 0.f)}; /* Back-Bot-left */
    Vertex v5{ glm::vec3(1.f, 0.f, -1.f), glm::vec3(1.f, 0.f, 0.f)}; /* Back-Bot-right */
    Vertex v6{ glm::vec3(1.f, 1.f, -1.f), glm::vec3(1.f, 0.f, 0.f)}; /* Back-Top-right */
    Vertex v7{ glm::vec3(0.f, 1.f, -1.f), glm::vec3(1.f, 0.f, 0.f)}; /* Back-Top-left */

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


