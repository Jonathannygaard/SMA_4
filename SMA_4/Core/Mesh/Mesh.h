#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Color;
    glm::vec3 Normal;
    Vertex(glm::vec3 Pos, glm::vec3 rgb) : Position(Pos), Color(rgb){}
};

struct Triangles
{
    unsigned int a, b, c;
    Triangles(unsigned int a, unsigned int b, unsigned int c) : a(a), b(b), c(c){}
};

class Mesh
{
public:
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;

    virtual void Draw(glm::vec4 model);
    virtual void Create(glm::vec3 color);
    virtual void BindBuffers();

    virtual ~Mesh();

    unsigned int VAO;
private:
    unsigned int VBO;
    unsigned int EBO;
};

class Cube : public Mesh
{
public:
    void Create(glm::vec3 color) override;
    void Draw(glm::vec4 model) override;
};
