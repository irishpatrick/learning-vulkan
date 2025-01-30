//
// Created by patrick on 1/26/25.
//

#include "ModelLoader.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

ModelLoader &ModelLoader::getInstance() {
    static ModelLoader loader;

    return loader;
}

std::vector<Mesh> ModelLoader::load(const std::string &fn) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
            fn.c_str(),
            aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);
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
        mesh->mVertices;
        mesh->mNormals;
        mesh->mColors;
        mesh->mTextureCoords;
    }

    for (size_t i = 0; i < node->mNumChildren; ++i) {
        processNode(meshes, scene, node->mChildren[i]);
    }
}
