//
// Created by patrick on 1/24/25.
//

#ifndef STAR_DESCRIPTORSET_HPP
#define STAR_DESCRIPTORSET_HPP

#include "Texture.hpp"
#include "UniformBufferData.hpp"

#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>

class DescriptorSet {
public:

    void create(uint32_t num, const std::vector<const Texture*>& textures);

    VkDescriptorSet operator[](int idx) {
        return descriptorSets[idx];
    }

    void setUniformData(uint32_t frame, UniformBufferData data);

    void destroy();

private:
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VmaAllocation> uniformBufferAllocations;
    std::vector<VkDescriptorSet> descriptorSets;
};


#endif //STAR_DESCRIPTORSET_HPP
