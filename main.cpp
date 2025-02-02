#include "DescriptorSet.hpp"
#include "Mesh.hpp"
#include "Screen.hpp"
#include "ModelLoader.hpp"
#include "components.hpp"
#include "entities.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/glm.hpp>
#include <flecs.h>
#include <SDL.h>

int main() {
    flecs::world world;
    world.import<component::Object3D>();

    flecs::system buildModelMatrix = world.system<component::ModelMatrix, const component::Position, const component::Rotation, const component::Scale>(
            "buildModelMatrix").each(
            [](component::ModelMatrix &m, const component::Position &p, const component::Rotation &r,
               const component::Scale &s) {
                m.m = glm::translate(glm::mat4(1.0f), p.v) * mat4_cast(r.q) * glm::scale(glm::mat4(1.0f), s.v);
            });

    auto &screen = Screen::getInstance();
    screen.create();

    auto viking = ModelLoader::getInstance().load("../assets/models/viking_room.obj");
    assert(viking.size() == 1);
    viking[0].createBuffers();
    viking[0].upload();

    auto skybox = ModelLoader::getInstance().loadSkybox("../assets/models/skybox.obj");

    auto vikingModel = entity::Player(world);

    Texture tex;
    Texture sky;
    tex.load("../assets/textures/viking_room.png");
    sky.load("../assets/textures/skybox.png");

    DescriptorSet ds;
    DescriptorSet skyDs;

    std::vector<const Texture *> textureList;
    textureList.reserve(1);
    textureList.push_back(&tex);

    std::vector<const Texture*> skyTlist;
    skyTlist.reserve(1);
    skyTlist.push_back(&sky);

    ds.create(2, textureList);
    skyDs.create(2, skyTlist);

    vikingModel.get_mut<component::Rotation>()->q *= glm::angleAxis(M_PIf, glm::vec3(1.0f, 0.0f, 0.0f));
    vikingModel.get_mut<component::Rotation>()->q *= glm::angleAxis(-M_PIf / 2.0f, glm::vec3(0.0f, 0.0f, 1.0f));

    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
                break;
            }
        }

        screen.drawFrame([&](Screen &sc) {
            buildModelMatrix.run();
            UniformBufferData data{};
            data.view = glm::lookAt(glm::vec3(2.0f, 2.0f, -2.0f), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            data.proj = glm::perspective(glm::radians(45.0f), sc.getWidth() / sc.getHeight(), 0.1f, 100.0f);

            data.model = glm::scale(glm::rotate(glm::mat4(1.0f), M_PIf, glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(10.0f));
            skyDs.setUniformData(sc.getCurrentFrame(), data);
            sc.bindDescriptorSet(skyDs[sc.getCurrentFrame()]);
            sc.drawMesh(skybox);

            data.model = vikingModel.get_mut<component::ModelMatrix>()->m;
            ds.setUniformData(sc.getCurrentFrame(), data);
            sc.bindDescriptorSet(ds[sc.getCurrentFrame()]);
            sc.drawMesh(viking[0]);


        });
    }

    viking[0].destroy();
    skybox.destroy();
    ds.destroy();
    skyDs.destroy();
    sky.destroy();
    tex.destroy();
    screen.destroy();

    return 0;
}
