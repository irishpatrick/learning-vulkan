//
// Created by patrick on 1/20/25.
//

#ifndef STAR_VERTEX_HPP
#define STAR_VERTEX_HPP

#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

#include <array>

class Vertex {
public:
    explicit Vertex(
            glm::vec3 p=glm::vec3(0.0f),
            glm::vec3 n=glm::vec3(0.0f),
            glm::vec3 c=glm::vec3(0.0f),
            glm::vec2 t=glm::vec2(0.0f)
                    ) : pos(p), norm(n), color(c), texture(t) {};
    ~Vertex() = default;

    glm::vec3 pos{};
    glm::vec3 norm{};
    glm::vec3 color{};
    glm::vec2 texture{};

    static VkVertexInputBindingDescription getBindingDesc();
    static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescs();
};


#endif //STAR_VERTEX_HPP
