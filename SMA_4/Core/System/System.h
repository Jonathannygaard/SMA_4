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

class NPCMovementSystem : public System
{
public:
    ComponentManager& componentmanager;
    NPCMovementSystem(ComponentManager& componentManager) : componentmanager(componentManager) {}
    void MoveEntity(Entity* entity, Entity* target);
    void FindDirection(Entity* entity, Entity* target);
};

class MeshSystem : public System
{
public:
    ComponentManager& componentmanager;
    MeshSystem(ComponentManager& componentManager) : componentmanager(componentManager) {}
    void DrawMesh(Entity* entity);
    void BindBuffers(Entity* entity);
    void CreateCubeMesh(Entity* entity, glm::vec3 color);
};

class CollisionSystem : public System
{
public:
    ComponentManager& componentmanager;
    CollisionSystem(ComponentManager& componentManager) : componentmanager(componentManager) {}
    bool CheckCollision(Entity* entity1, Entity* entity2);
    void UpdatePosition(Entity* entity);
};

class CombatSystem : public System
{
public:
    ComponentManager& componentmanager;
    CombatSystem(ComponentManager& componentManager) : componentmanager(componentManager) {}
    void Attack(Entity* entity1, Entity* entity2);
    void TakeDamage(Entity* entity, int damage);
    void DelayTimer(Entity* entity);
};

