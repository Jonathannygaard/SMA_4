#pragma once
#include <map>
#include <glm/vec3.hpp>

class Component{};

class ComponentManager
{
    std::map<int,Component> Components;
};

class PositionComponent : public Component
{
public:
    glm::vec3 Position;
    PositionComponent(glm::vec3 pos) : Position(pos){}
};

class MovementComponent : public Component
{
public:
    glm::vec3 Speed;
    MovementComponent(glm::vec3 speed) : Speed(speed){}
};