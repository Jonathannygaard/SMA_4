#include "Mesh.h"
#include "../Shader/Shader.h"
#include <glm/ext/quaternion_geometric.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "glad/glad.h"

void Mesh::Draw(glm::vec4 model)
{
    glUniformMatrix4fv(glGetUniformLocation(Shader::Program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, Indices.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::BindBuffers()
{// VAO
    glGenBuffers(1, &VBO);

    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), Vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(Triangles), Indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Position)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Color)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Cube::Create(glm::vec3 color)
{
    Vertex v0{glm::vec3(0.f, 0.f, 0.f), color}; /* Front-Bot-left */
    Vertex v1{ glm::vec3(1.f, 0.f, 0.f), color}; /* Front-Bot-right */
    Vertex v2{ glm::vec3(1.f, 1.f, 0.f), color}; /* Front-Top-right */
    Vertex v3{ glm::vec3(0.f, 1.f, 0.f), color}; /* Front-Top-left */
    Vertex v4{ glm::vec3(0.f, 0.f, -1.f), color}; /* Back-Bot-left */
    Vertex v5{ glm::vec3(1.f, 0.f, -1.f), color}; /* Back-Bot-right */
    Vertex v6{ glm::vec3(1.f, 1.f, -1.f), color}; /* Back-Top-right */
    Vertex v7{ glm::vec3(0.f, 1.f, -1.f), color}; /* Back-Top-left */

    Vertices.emplace_back(v0);
    Vertices.emplace_back(v1);
    Vertices.emplace_back(v2);
    Vertices.emplace_back(v3);
    Vertices.emplace_back(v4);
    Vertices.emplace_back(v5);
    Vertices.emplace_back(v6);
    Vertices.emplace_back(v7);

    /* Front */
    Indices.emplace_back(0,1,2);
    Indices.emplace_back(2,3,0);

    /* Right */
    Indices.emplace_back(1,5,6);
    Indices.emplace_back(6,2,1);

    /* Left */			   
    Indices.emplace_back(0,3,7);
    Indices.emplace_back(7,4,0);
						   
    /* Back */
    Indices.emplace_back(4,7,6);
    Indices.emplace_back(6,5,4);

    /* Top */
    Indices.emplace_back(3,2,6);
    Indices.emplace_back(6,7,3);

    /* Bottom */
    Indices.emplace_back(0,4,5);
    Indices.emplace_back(5,1,0);

    //Can create problem later, had to make indices a pointer to fix the problem
    for (auto Triangles : &Indices)
    {
        glm::vec3 Normal = glm::normalize(glm::cross(Vertices[Triangles.b].Position - Vertices[Triangles.a].Position,
            Vertices[Triangles.c].Position - Vertices[Triangles.a].Position));
        Vertices[Triangles].Normal += Normal;
        Vertices[Triangles.b].Normal += Normal;
        Vertices[Triangles.c].Normal += Normal;
    }
    BindBuffers();
}
