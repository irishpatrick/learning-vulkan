//
// Created by patrick on 2/1/25.
//

#ifndef STAR_COMPONENTS_HPP
#define STAR_COMPONENTS_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <flecs.h>

namespace component {
    struct Position {
        glm::vec3 v;
    };
    struct Rotation {
        glm::quat q;
    };
    struct Scale {
        glm::vec3 v;
    };
    struct ModelMatrix {
        glm::mat4 m;
    };

    struct Object3D {
        explicit Object3D(flecs::world& world) {
            world.component<Position>();
            world.component<Rotation>();
            world.component<Scale>();
            world.component<ModelMatrix>();
        }

    };
}


#endif //STAR_COMPONENTS_HPP
