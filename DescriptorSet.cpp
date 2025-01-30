//
// Created by patrick on 1/24/25.
//

#include "DescriptorSet.hpp"
#include "Screen.hpp"
#include "UniformBufferData.hpp"

#include <iostream>
#include <vector>

void DescriptorSet::create(uint32_t num, const std::vector<const Texture*>& textures) {
    descriptorSets = Screen::getInstance().allocDescriptorSets(num);

    uniformBuffers.resize(num);
    uniformBufferAllocations.resize(num);
    const VkDeviceSize bufferSize = sizeof(UniformBufferData);
    for (size_t i = 0; i < uniformBuffers.size(); ++i) {
        VkBufferCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        info.size = bufferSize;
        info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
        info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo vmaInfo{};
        vmaInfo.usage = VMA_MEMORY_USAGE_AUTO;
        vmaInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;
        vmaInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

        if (vmaCreateBuffer(Screen::getInstance().getAllocator(), &info, &vmaInfo, &uniformBuffers[i], &uniformBufferAllocations[i], nullptr) != VK_SUCCESS) {
            throw std::runtime_error("cannot create uniform buffer for frame");
        }
    }

    for (uint32_t i = 0; i < num; ++i) {
        assert(i < uniformBuffers.size());
        assert(i < uniformBufferAllocations.size());
        assert(i < descriptorSets.size());

        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferData);

        std::vector<VkWriteDescriptorSet> writes;
        writes.resize(2 + textures.size());

        assert(writes.size() >= 1);
        writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writes[0].dstSet = descriptorSets[i];
        writes[0].dstBinding = 0;
        writes[0].dstArrayElement = 0;
        writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writes[0].descriptorCount = 1;
        writes[0].pBufferInfo = &bufferInfo;

        assert(writes.size() >= 2);
        VkDescriptorImageInfo samplerInfo{};
        samplerInfo.sampler = Screen::getInstance().getSampler();
        samplerInfo.imageView = VK_NULL_HANDLE;
        writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writes[1].dstSet = descriptorSets[i];
        writes[1].dstBinding = 1;
        writes[1].dstArrayElement = 0;
        writes[1].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
        writes[1].descriptorCount = 1;
        writes[1].pImageInfo = &samplerInfo;

        std::vector<VkDescriptorImageInfo> infoStore;
        infoStore.reserve(textures.size());

        for (size_t j = 0; j < textures.size(); ++j) {
            assert(textures[j]);
            if (textures[j]->getImageView() == VK_NULL_HANDLE) {
                throw std::runtime_error("null image view");
            }
            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL;
            imageInfo.imageView = textures[j]->getImageView();
            assert(imageInfo.imageView);
            imageInfo.sampler = nullptr;
            infoStore.push_back(imageInfo);

            const size_t w = j + 2;
            assert(w < writes.size());
            writes[w].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writes[w].dstSet = descriptorSets[i];
            writes[w].dstBinding = 2;
            writes[w].dstArrayElement = 0;
            writes[w].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
            writes[w].descriptorCount = 1;
            writes[w].pImageInfo = &infoStore[infoStore.size()-1];
        }

        vkUpdateDescriptorSets(Screen::getInstance().getDevice(), static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
    }
}

void DescriptorSet::setUniformData(uint32_t frame, UniformBufferData data) {
    vmaCopyMemoryToAllocation(Screen::getInstance().getAllocator(), &data, uniformBufferAllocations[frame], 0, sizeof(data));
}

void DescriptorSet::destroy() {
    assert(uniformBuffers.size() == uniformBufferAllocations.size());
    for (size_t i = 0; i < uniformBuffers.size(); ++i) {
        vmaDestroyBuffer(Screen::getInstance().getAllocator(), uniformBuffers[i], uniformBufferAllocations[i]);
    }
    uniformBufferAllocations.clear();
    uniformBuffers.clear();

    descriptorSets.clear();
}
