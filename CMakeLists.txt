cmake_minimum_required(VERSION 3.25)
project(star)

include(FetchContent)

FetchContent_Declare(
        sdl2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG release-2.30.11
)
FetchContent_MakeAvailable(sdl2)

set(BUILD_SHARED_LIBS OFF)
FetchContent_Declare(
        sdl_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
        GIT_TAG release-2.8.4
)
FetchContent_MakeAvailable(sdl_image)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
        flecs
        GIT_REPOSITORY https://github.com/SanderMertens/flecs
        GIT_TAG v4.0.4
)
FetchContent_MakeAvailable(flecs)

FetchContent_Declare(
        VulkanMemoryAllocator
        GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
        GIT_TAG v3.2.0
)
FetchContent_MakeAvailable(VulkanMemoryAllocator)

set(ASSIMP_NO_EXPORT ON)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_INSTALL OFF)
FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp
        GIT_TAG v5.4.3
)
FetchContent_MakeAvailable(assimp)

find_package(Vulkan REQUIRED)

set(CMAKE_CXX_STANDARD 23)

add_executable(star main.cpp
        Screen.cpp
        Screen.hpp
        Shader.cpp
        Shader.hpp
        Vertex.cpp
        Vertex.hpp
        Mesh.cpp
        Mesh.hpp
        VmaUsage.cpp
        UniformBufferData.hpp
        Texture.cpp
        Texture.hpp
        DescriptorSet.cpp
        DescriptorSet.hpp
        ModelLoader.cpp
        ModelLoader.hpp
)
target_link_libraries(star PRIVATE
        SDL2-static
        SDL2_image::SDL2_image-static
        glm-header-only
        flecs::flecs_static
        Vulkan::Vulkan
        GPUOpen::VulkanMemoryAllocator
        assimp::assimp
)

add_custom_command(
        TARGET star
        PRE_BUILD
        COMMAND cd ${CMAKE_SOURCE_DIR}/shaders && python3 compile.py
)

