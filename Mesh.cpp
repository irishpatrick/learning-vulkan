//
// Created by patrick on 1/20/25.
//

#include "Mesh.hpp"
#include "Screen.hpp"


#include <utility>

void Mesh::createBuffers() {
    if (allocated) {
        return;
    }

    auto& screen = Screen::getInstance();
    assert(screen.getAllocator());

    VkBufferCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    info.size = sizeof(vertices[0]) * vertices.size();
    info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo vmaInfo{};
    vmaInfo.usage = VMA_MEMORY_USAGE_AUTO;
    vmaInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;
    vmaInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    if (vmaCreateBuffer(screen.getAllocator(), &info, &vmaInfo, &vertexBuffer, &vertexBufferAlloc, nullptr) != VK_SUCCESS) {
        throw std::runtime_error("cannot create vertex buffer");
    }

    VkBufferCreateInfo iInfo{};
    iInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    iInfo.size = sizeof(indices[0]) * indices.size();
    iInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo  iVmaInfo{};
    iVmaInfo.usage = VMA_MEMORY_USAGE_AUTO;
    iVmaInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;
    iVmaInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    if (vmaCreateBuffer(screen.getAllocator(), &iInfo, &iVmaInfo, &indexBuffer, &indexBufferAlloc, nullptr) != VK_SUCCESS) {
        throw std::runtime_error("cannot create index buffer");
    }

    allocated = true;
}

void Mesh::destroy() {
    if (!allocated) {
        return;
    }

    auto& screen = Screen::getInstance();
    assert(screen.getAllocator());

    vkDeviceWaitIdle(screen.getDevice());
    vmaDestroyBuffer(screen.getAllocator(), vertexBuffer, vertexBufferAlloc);
    vmaDestroyBuffer(screen.getAllocator(), indexBuffer, indexBufferAlloc);

    allocated = false;
}

void Mesh::upload() {
    assert(allocated);

    auto& screen = Screen::getInstance();
    assert(screen.getAllocator());

    if (vmaCopyMemoryToAllocation(screen.getAllocator(), vertices.data(), vertexBufferAlloc, 0, sizeof(vertices[0]) * vertices.size()) != VK_SUCCESS) {
        throw std::runtime_error("cannot upload mesh vertices");
    };

    if (vmaCopyMemoryToAllocation(screen.getAllocator(), indices.data(), indexBufferAlloc, 0, sizeof(indices[0]) * indices.size()) != VK_SUCCESS) {
        throw std::runtime_error("cannot upload mesh indices");
    }
}

Mesh Mesh::triangle() {
    Mesh mesh;
    std::vector<Vertex> vertices = {
            Vertex(glm::vec3(0.0f, -0.5f, 0.0f),glm::vec3(1.0f,0.0f,0.0f)),
            Vertex(glm::vec3(0.5f, 0.5f, 0.0f),glm::vec3(0.0f,1.0f,0.0f)),
            Vertex(glm::vec3(-0.5f, 0.5f, 0.0f),glm::vec3(0.0f,0.0f,1.0f)),
    };
    mesh.setVertices(vertices);

    std::vector<uint16_t> indices = {
            0, 1, 2
    };
    mesh.setIndices(indices);

    mesh.createBuffers();
    mesh.upload();

    return mesh;
}

void Mesh::setVertices(std::vector<Vertex> verts) {
    vertices = std::move(verts);
}

void Mesh::draw(VkCommandBuffer cb) {
    assert(allocated);
    assert(cb);

    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};

    vkCmdBindVertexBuffers(cb, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(cb, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

//    vkCmdDraw(cb, static_cast<uint32_t>(vertices.size()), 1, 0, 0);
    vkCmdDrawIndexed(cb, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
}

void Mesh::setIndices(std::vector<uint16_t> idcs) {
    indices = std::move(idcs);
}

VkBuffer Mesh::getVertexBuffer() const {
    return vertexBuffer;
}

VkBuffer Mesh::getIndexBuffer() const {
    return indexBuffer;
}

uint32_t Mesh::getNumIndices() const {
    return static_cast<uint32_t>(indices.size());
}

Mesh Mesh::square() {
    Mesh mesh;
    std::vector<Vertex> vertices = {
            Vertex(glm::vec3(-0.5f, -0.5f, 0.0f),glm::vec3(1.0f,0.0f,0.0f),glm::vec2(0.0f,0.0f)),
            Vertex(glm::vec3(0.5f, -0.5f, 0.0f),glm::vec3(1.0f,0.0f,0.0f),glm::vec2(1.0f,0.0f)),
            Vertex(glm::vec3(0.5f, 0.5f, 0.0f),glm::vec3(0.0f,1.0f,0.0f),glm::vec2(1.0f,1.0f)),
            Vertex(glm::vec3(-0.5f, 0.5f, 0.0f),glm::vec3(0.0f,0.0f,1.0f),glm::vec2(0.0f,1.0f)),
    };
    mesh.setVertices(vertices);

    std::vector<uint16_t> indices = {
            0, 1, 2,
            2, 3, 0
    };
    mesh.setIndices(indices);

    mesh.createBuffers();
    mesh.upload();

    return mesh;
}
