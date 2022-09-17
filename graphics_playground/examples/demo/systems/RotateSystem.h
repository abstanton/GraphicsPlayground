#pragma once
#include "../../../systems/System.h"

class RotateSystem :
    public System
{
    void onUpdate(ecs::Registry& registry, float delta_time) override;
};

