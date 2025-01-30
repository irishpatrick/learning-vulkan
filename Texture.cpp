//
// Created by patrick on 1/23/25.
//

#include "Texture.hpp"
#include "Screen.hpp"

#include <SDL_image.h>
#include <stdexcept>
#include <iostream>

void Texture::load(const std::string &fn) {
    SDL_Surface* surf = IMG_Load(fn.c_str());
    if (!surf) {
        throw std::runtime_error("cannot open image");
    }
    SDL_Surface* conv = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_RGBA32, 0);
    if (!conv) {
        throw std::runtime_error("cannot convert colors");
    }
    SDL_FreeSurface(surf);

    VkDeviceSize imageSize = conv->w * conv->h * conv->format->BytesPerPixel;
    VkBuffer staging;
    VmaAllocation alloc;

    VkBufferCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    info.size = imageSize;
    info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo vmaInfo{};
    vmaInfo.usage = VMA_MEMORY_USAGE_AUTO;
    vmaInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;
    vmaInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    if (vmaCreateBuffer(Screen::getInstance().getAllocator(), &info, &vmaInfo, &staging, &alloc, nullptr) != VK_SUCCESS) {
        throw std::runtime_error("cannot create vertex buffer");
    }
    vmaCopyMemoryToAllocation(Screen::getInstance().getAllocator(), (uint8_t*)conv->pixels, alloc, 0, imageSize);

    width = conv->w;
    height = conv->h;
    SDL_FreeSurface(conv);

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = static_cast<uint32_t>(width);
    imageInfo.extent.height = static_cast<uint32_t>(height);
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0;

    VmaAllocationCreateInfo  imgVmaInfo{};
    imgVmaInfo.usage = VMA_MEMORY_USAGE_AUTO;
    imgVmaInfo.flags = VMA_ALLOCATION_CREATE_DONT_BIND_BIT;
    imgVmaInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    if (const auto result = vmaCreateImage(Screen::getInstance().getAllocator(), &imageInfo, &imgVmaInfo, &textureImage, &textureImageAlloc, nullptr); result != VK_SUCCESS) {
        std::cout << result << std::endl;
        throw std::runtime_error("cannot allocate image memory");
    };
    vmaBindImageMemory(Screen::getInstance().getAllocator(), textureImageAlloc, textureImage);

    Screen::getInstance().transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    Screen::getInstance().copyBufferToImage(staging, textureImage, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
    Screen::getInstance().transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(Screen::getInstance().getDevice(), staging, nullptr);
    vmaFreeMemory(Screen::getInstance().getAllocator(), alloc);

    view = Screen::getInstance().createImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
}

void Texture::destroy() {
    assert(view);
    assert(textureImage);;
    assert(textureImageAlloc);
    vkDestroyImageView(Screen::getInstance().getDevice(), view, nullptr);
    vmaDestroyImage(Screen::getInstance().getAllocator(), textureImage, textureImageAlloc);
    view = VK_NULL_HANDLE;
    textureImage = VK_NULL_HANDLE;
    textureImageAlloc = VK_NULL_HANDLE;
}

VkImageView Texture::getImageView() const {
    return view;
}

void Texture::createDepthBuffer(uint32_t w, uint32_t h, VkFormat format) {
    width = w;
    height = h;

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = static_cast<uint32_t>(width);
    imageInfo.extent.height = static_cast<uint32_t>(height);
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0;

    VmaAllocationCreateInfo  imgVmaInfo{};
    imgVmaInfo.usage = VMA_MEMORY_USAGE_AUTO;
    imgVmaInfo.flags = VMA_ALLOCATION_CREATE_DONT_BIND_BIT;
    imgVmaInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    if (const auto result = vmaCreateImage(Screen::getInstance().getAllocator(), &imageInfo, &imgVmaInfo, &textureImage, &textureImageAlloc, nullptr); result != VK_SUCCESS) {
        std::cout << result << std::endl;
        throw std::runtime_error("cannot allocate image memory");
    };
    vmaBindImageMemory(Screen::getInstance().getAllocator(), textureImageAlloc, textureImage);

    view = Screen::getInstance().createImageView(textureImage, format, VK_IMAGE_ASPECT_DEPTH_BIT);
    if (view == VK_NULL_HANDLE) {
        throw std::runtime_error("null imageview");
    }

    Screen::getInstance().transitionImageLayout(textureImage, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}
