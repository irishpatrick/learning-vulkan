//
// Created by patrick on 1/23/25.
//

#ifndef STAR_TEXTURE_HPP
#define STAR_TEXTURE_HPP

#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>

#include <cstdint>
#include <string>

class Texture {
public:
    Texture() = default;
    ~Texture() = default;

    Texture(const Texture& other) = delete;
    Texture& operator=(const Texture& other) = delete;

    Texture(Texture&& other) {
        width = other.width;
        height = other.height;
        channels = other.channels;
        textureImage = other.textureImage;
        other.textureImage = VK_NULL_HANDLE;
        textureImageAlloc = other.textureImageAlloc;
        other.textureImageAlloc = VK_NULL_HANDLE;
        view = other.view;
        other.view = VK_NULL_HANDLE;
    }

    Texture& operator=(Texture&& other) {
        if (this != &other) {
            width = other.width;
            height = other.height;
            channels = other.channels;
            textureImage = other.textureImage;
            other.textureImage = VK_NULL_HANDLE;
            textureImageAlloc = other.textureImageAlloc;
            other.textureImageAlloc = VK_NULL_HANDLE;
            view = other.view;
            other.view = VK_NULL_HANDLE;
        }

        return *this;
    }

    void load(const std::string& fn);
    void createDepthBuffer(uint32_t w, uint32_t h, VkFormat format);
    void destroy();

    VkImageView getImageView() const;

private:
    int32_t width;
    int32_t height;
    int32_t channels;

    VkImage textureImage{VK_NULL_HANDLE};
    VmaAllocation textureImageAlloc{VK_NULL_HANDLE};
    VkImageView view{VK_NULL_HANDLE};
};


#endif //STAR_TEXTURE_HPP
