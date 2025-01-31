//
// Created by patrick on 1/26/25.
//

#include "ModelLoader.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

ModelLoader &ModelLoader::getInstance() {
    static ModelLoader loader;

    return loader;
}

std::vector<Mesh> ModelLoader::load(const std::string &fn) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
            fn.c_str(),
            aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_FlipUVs);
    if (scene == nullptr) {
        throw std::runtime_error("cannot open model file " + fn);
    }

    std::vector<Mesh> meshes;

    processNode(meshes, scene, scene->mRootNode);

    return meshes;
}

void ModelLoader::processNode(std::vector<Mesh> &meshes, const aiScene* scene, aiNode *node) {
    for (size_t i = 0; i < node->mNumMeshes; ++i) {
        const auto meshIdx = node->mMeshes[i];
        const auto mesh = scene->mMeshes[meshIdx];
        const auto numVerts = mesh->mNumVertices;
        const auto numFaces = mesh->mNumFaces;
        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;
        vertices.reserve(numVerts);

        for (size_t j = 0; j < numVerts; ++j) {
            const auto meshPos = mesh->mVertices + j;
            const auto meshNorm = mesh->mNormals + j;
            glm::vec3 tex(0.0f);
            if (mesh->mTextureCoords[0]) {
                tex.x = mesh->mTextureCoords[0][j].x;
                tex.y = mesh->mTextureCoords[0][j].y;
            } else {
                std::cout << "warning: no texture data" << std::endl;
            }
            const glm::vec3 pos(meshPos->x, meshPos->y, meshPos->z);
            const glm::vec3 norm(meshNorm->x, meshNorm->y, meshNorm->z);

            vertices.emplace_back(pos, norm, pos, tex);
        }

        for (size_t j = 0; j < numFaces; ++j) {
            const auto face = mesh->mFaces + j;
            for (size_t k = 0; k < face->mNumIndices; ++k) {
                indices.push_back(face->mIndices[k]);
            }
        }

        Mesh loaded;
        loaded.setVertices(vertices);
        loaded.setIndices(indices);
        meshes.push_back(std::move(loaded));
    }

    for (size_t i = 0; i < node->mNumChildren; ++i) {
        processNode(meshes, scene, node->mChildren[i]);
    }
}
