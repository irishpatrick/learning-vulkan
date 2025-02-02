//
// Created by patrick on 2/1/25.
//

#ifndef STAR_ENTITIES_HPP
#define STAR_ENTITIES_HPP

#include "components.hpp"

#include <flecs.h>

namespace entity {
    inline flecs::entity Player(flecs::world& world) {
        flecs::entity e = world.entity()
                .set(component::Position{glm::vec3(0.0f)})
                .set(component::Rotation{glm::angleAxis(0.0f, glm::vec3(0.0f, 1.0f, 0.0f))})
                .set(component::Scale{glm::vec3(1.0f, 1.0f, 1.0f)})
                .set(component::ModelMatrix{glm::mat4(1.0f)});

        return e;
    }
}

#endif //STAR_ENTITIES_HPP
