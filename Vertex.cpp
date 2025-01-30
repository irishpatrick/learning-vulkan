//
// Created by patrick on 1/20/25.
//

#include "Vertex.hpp"

VkVertexInputBindingDescription Vertex::getBindingDesc() {
    VkVertexInputBindingDescription desc{};

    desc.binding = 0;
    desc.stride = sizeof(Vertex);
    desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return desc;
}

std::array<VkVertexInputAttributeDescription, 3> Vertex::getAttributeDescs() {
    std::array<VkVertexInputAttributeDescription, 3> descs{};

    // vertex
    descs[0].binding = 0;
    descs[0].location = 0;
    descs[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    descs[0].offset = offsetof(Vertex, pos);

    // color
    descs[1].binding = 0;
    descs[1].location = 1;
    descs[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    descs[1].offset = offsetof(Vertex, color);

    // texture
    descs[2].binding = 0;
    descs[2].location = 2;
    descs[2].format = VK_FORMAT_R32G32_SFLOAT;
    descs[2].offset = offsetof(Vertex, texture);

    return descs;
}
