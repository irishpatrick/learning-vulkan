//
// Created by patrick on 1/26/25.
//

#ifndef STAR_MODELLOADER_HPP
#define STAR_MODELLOADER_HPP


#include "Mesh.hpp"
#include <assimp/scene.h>

#include <vector>

class ModelLoader {
public:
    static ModelLoader& getInstance();

    ~ModelLoader() = default;

    std::vector<Mesh> load(const std::string& fn);

private:
    ModelLoader() = default;

    void processNode(std::vector<Mesh>& meshes, const aiScene* scene, aiNode* node);
};


#endif //STAR_MODELLOADER_HPP
