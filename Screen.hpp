//
// Created by patrick on 1/15/25.
//

#ifndef STAR_SCREEN_HPP
#define STAR_SCREEN_HPP

#include "Shader.hpp"
#include "UniformBufferData.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>

#include <functional>
#include <vector>

struct SDL_Window;

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentMode;
};

class FrameProcessor {
public:
    FrameProcessor() = default;

    ~FrameProcessor() {
        destroy();
    }

    void init(VkDevice dev, VkCommandPool pool, uint32_t maxFrames);
    void destroy();

    VkFence* fence() {
        return &inFlightFences[currentFrame];
    }

    VkSemaphore* imageAvailableSem() {
        return &imageAvailableSems[currentFrame];
    }

    VkSemaphore* renderFinishedSem() {
        return &renderFinishedSems[currentFrame];
    }

    VkCommandBuffer* commandBuffer() {
        return &commandBuffers[currentFrame];
    }

    void nextFrame() {
        currentFrame = (currentFrame + 1) % numFrames;
    }

    uint32_t getCurrentFrame() const;

private:
    uint32_t numFrames{0};
    uint32_t currentFrame = 0;
    VkDevice device{VK_NULL_HANDLE};
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkSemaphore> imageAvailableSems;
    std::vector<VkSemaphore> renderFinishedSems;
    std::vector<VkFence> inFlightFences;
};

class SwapChain {
public:
    void create(VkDevice device, QueueFamilyIndices idcs, SwapChainSupportDetails dets, VkSurfaceKHR surface, SDL_Window* window);
    void create(VkDevice device, VkSurfaceKHR surface, SDL_Window* window);
    void setRenderPass(VkDevice device, VkRenderPass ps);
    void recreate(VkDevice device, VkSurfaceKHR surface, SDL_Window* window);
    void destroy(VkDevice device);

    [[nodiscard]] VkExtent2D getExtent() const {
        return extent;
    }

    VkFormat getImageFormat() {
        return imageFormat;
    }

    VkFramebuffer getFrameBuffer(uint32_t index) {
        return framebuffers[index];
    }

    VkSwapchainKHR getChain() {
        return swapChain;
    }

    void setDepthImage(Texture& texture);

private:
    VkRenderPass pass{VK_NULL_HANDLE};
    VkSwapchainKHR swapChain;
    std::vector<VkImage> images;
    std::vector<VkImageView> imageViews;
    VkFormat imageFormat;
    VkExtent2D extent;
    std::vector<VkFramebuffer> framebuffers;
    SwapChainSupportDetails details;
    QueueFamilyIndices indices;
    Texture depthImage;
};

class Screen {
public:
    ~Screen() {
        destroy();
    }
    Screen(Screen const&) = delete;
    void operator=(Screen const&) = delete;

    void create();
    void destroy();

    static Screen& getInstance();

    void recordCommandBuffer(VkCommandBuffer cb, uint32_t imageIndex);
    void endCommandBuffer(VkCommandBuffer cb);
    void doRenderPass(std::function<void(Screen&)> draws, VkCommandBuffer cb, uint32_t imageIndex);
    void drawFrame(std::function<void(Screen&)> draws);
    void addVertexShader(const std::string& name, const std::vector<char>&);
    void addFragmentShader(const std::string& name, const std::vector<char>&);

    VmaAllocator getAllocator();

    VkCommandBuffer* getCommandBuffer();

    VkDevice getDevice();

    void setUniformData(UniformBufferData data);
    void drawMesh(const Mesh& mesh);

    float getWidth() const;

    float getHeight() const;

    VkCommandBuffer beginSingleTimeCommandBuffer();
    void endSingleTimeCommandBuffer(VkCommandBuffer cb);
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t w, uint32_t h);

    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    std::pair<VkBuffer, VmaAllocation> createBuffer(VkDeviceSize size, VkBufferUsageFlags useFlags);
    std::vector<VkDescriptorSet> allocDescriptorSets(uint32_t num);

    VkSampler getSampler();

    void bindDescriptorSet(VkDescriptorSet descriptorSet);

    uint32_t getCurrentFrame();

private:
    Screen() = default;

    void createUniformBuffers();
    void createDescriptorPool();
    void createDescriptorSets();
    void createTextureSampler();
    void createDepthResources();
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    SDL_Window *window{};
    VkInstance instance;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkSurfaceKHR surface;
    SwapChain swapChain;
    std::vector<VkShaderModule> shaders;
    std::vector<VkPipelineShaderStageCreateInfo> pipelineShaders;
    VkPipelineLayout pipelineLayout;
    VkDescriptorSetLayout descriptorSetLayout;
    VkRenderPass renderPass;
    VkPipeline graphicsPipeline;
    VkCommandPool commandPool;
    FrameProcessor frameProcessor;
    VmaAllocator allocator;
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VmaAllocation> uniformBuffersAlloc;
    VkDescriptorPool descriptorPool;
    uint32_t descriptorCount{0};
    uint32_t descriptorMax{0};
    std::vector<VkDescriptorSet> descriptorSets;
    VkSampler sampler{VK_NULL_HANDLE};
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    VkPhysicalDevice pDevice;

    VkFormat findDepthFormat();
};


#endif //STAR_SCREEN_HPP
