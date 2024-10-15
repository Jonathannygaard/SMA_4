#pragma once
#include "../Components/Component.h"
#include "../Entity/Entity.h"

class System{};

class MovementSystem : public System
{
public:
    ComponentManager& componentmanager;
    MovementSystem(ComponentManager& componentManager) : componentmanager(componentManager) {}
    void MoveEntity(Entity* entity);  
};

class MeshSystem : public System
{
public:
    ComponentManager& componentmanager;
    MeshSystem(ComponentManager& componentManager) : componentmanager(componentManager) {}
    void DrawMesh(Entity* entity);
    void BindBuffers(Entity* entity);
    void CreateCubeMesh(Entity* entity);
};

