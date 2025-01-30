//
// Created by patrick on 1/16/25.
//

#ifndef STAR_SHADER_HPP
#define STAR_SHADER_HPP

#include <vulkan/vulkan.hpp>

#include <string>
#include <vector>

class Shader {
public:
    static VkShaderModule openFile(VkDevice, const std::string&);
    static VkShaderModule createShaderModule(VkDevice, const std::vector<char>&);
};


#endif //STAR_SHADER_HPP
