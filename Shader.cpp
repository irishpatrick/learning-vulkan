//
// Created by patrick on 1/16/25.
//

#include "Shader.hpp"

#include <fstream>

static std::vector<char> readFile(const std::string& filename) {
    std::ifstream in(filename, std::ios::ate | std::ios::binary);
    if (!in) {
        throw std::runtime_error("cannot open shader file " + filename);
    }

    const auto size = static_cast<size_t>(in.tellg());
    std::vector<char> buffer(size);

    in.seekg(0);
    in.read(buffer.data(), size);
    in.close();

    return buffer;
}

VkShaderModule Shader::createShaderModule(VkDevice device, const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule module;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &module) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module");
    }

    return module;
}

VkShaderModule Shader::openFile(VkDevice device, const std::string &fn) {
    const std::vector<char> data = readFile(fn);
    return createShaderModule(device, data);
}
