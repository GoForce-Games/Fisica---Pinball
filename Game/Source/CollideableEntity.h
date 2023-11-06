#pragma once
#include "Entity.h"

class CollideableEntity : public Entity
{
public:

    CollideableEntity(EntityType type) : Entity(type), pbody(nullptr) {}


    PhysBody* pbody;
};

