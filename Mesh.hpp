//
// Created by patrick on 1/20/25.
//

#ifndef STAR_MESH_HPP
#define STAR_MESH_HPP

#include "Vertex.hpp"

#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>

#include <cstdint>

class Mesh {
public:
    Mesh() = default;
    ~Mesh() = default;

    void createBuffers();
    void setVertices(std::vector<Vertex> verts);
    void setIndices(std::vector<uint16_t> idcs);
    void upload();
    void destroy();
    void draw(VkCommandBuffer cb);

    static Mesh triangle();
    static Mesh square();

    [[nodiscard]] VkBuffer getVertexBuffer() const;
    [[nodiscard]] VkBuffer getIndexBuffer() const;

    [[nodiscard]] uint32_t getNumIndices() const;

private:
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;
    VkBuffer vertexBuffer;
    VmaAllocation vertexBufferAlloc;
    VkBuffer indexBuffer;
    VmaAllocation indexBufferAlloc;
    bool allocated{false};
};


#endif //STAR_MESH_HPP
