#pragma once
#include <map>
#include <string>
#include "../Components/Component.h"

class Mesh;

class Actor
{
public:
    Mesh* mesh;
    ComponentManager* Components;
    
};
