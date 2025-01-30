//
// Created by patrick on 1/22/25.
//

#ifndef STAR_UNIFORMBUFFERDATA_HPP
#define STAR_UNIFORMBUFFERDATA_HPP

#include <glm/glm.hpp>

struct UniformBufferData {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

#endif //STAR_UNIFORMBUFFERDATA_HPP
