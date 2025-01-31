#include "DescriptorSet.hpp"
#include "Mesh.hpp"
#include "Screen.hpp"
#include "ModelLoader.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>

#include <SDL.h>

int main() {
    auto& screen = Screen::getInstance();
    screen.create();

    auto viking = ModelLoader::getInstance().load("../assets/viking_room.obj");
    assert(viking.size() == 1);
    viking[0].createBuffers();
    viking[0].upload();

    Texture tex;
    tex.load("../assets/viking_room.png");
//    tex.load("../assets/test.jpg");

    DescriptorSet ds;
    DescriptorSet ds2;

    std::vector<const Texture*> textureList;
    textureList.reserve(1);
    textureList.push_back(&tex);

    ds.create(2, textureList);
    ds2.create(2, textureList);

    auto mesh = Mesh::square();

    auto model = glm::mat4(1.0f);
    model = glm::rotate(model, M_PIf, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, -M_PIf / 2.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    auto model2 = glm::mat4(1.0f);
    model2 = glm::translate(model2, glm::vec3(0.5f, 0.0f, 0.1f));

    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
                break;
            }
        }

        screen.drawFrame([&] (Screen& sc){
            UniformBufferData data{};
//            model = glm::rotate(model, 0.01f, glm::vec3(0, 0, 1));
            data.view = glm::lookAt(glm::vec3(2.0f, 2.0f, -2.0f), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            data.proj = glm::perspective(glm::radians(45.0f), sc.getWidth() / sc.getHeight(), 0.1f, 10.0f);

            data.model = model;
            ds.setUniformData(sc.getCurrentFrame(), data);
            sc.bindDescriptorSet(ds[sc.getCurrentFrame()]);

            sc.drawMesh(viking[0]);
        });
    }

    viking[0].destroy();
    mesh.destroy();
    ds.destroy();
    ds2.destroy();
    tex.destroy();
    screen.destroy();

    return 0;
}
