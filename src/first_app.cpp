#include "../inc/first_app.hpp"

#include "../inc/lve_camera.hpp"
#include "../inc/keyboard_movement_controller.hpp"
#include "../inc/simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <chrono>
#include <stdexcept>

namespace lve {

  /*
   * Constructor
   */
  FirstApp::FirstApp() {
    loadGameObjects();
  }

  FirstApp::~FirstApp() {}

  /*
   * run
   */
  void FirstApp::run() {
    SimpleRenderSystem simpleRenderSystem{lveDevice, lveRenderer.getSwapChainRenderPass()};
    LveCamera camera{};
    camera.setViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

    auto viewerObject = LveGameObject::createGameObject();
    KeyboardMovementController cameraController{};

    auto currentTime = std::chrono::high_resolution_clock::now();

    while (!lveWindow.shouldClose()) {
      glfwPollEvents();

      auto newTime = std::chrono::high_resolution_clock::now();
      float frameTime =
        std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
      currentTime = newTime;

      cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), frameTime, viewerObject);
      camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

      float aspect = lveRenderer.getAspectRatio();
      camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

      if (auto commandBuffer = lveRenderer.beginFrame()) {
        lveRenderer.beginSwapChainRenderPass(commandBuffer);
        simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
        lveRenderer.endSwapChainRenderPass(commandBuffer);
        lveRenderer.endFrame();
      }
    }

    vkDeviceWaitIdle(lveDevice.device());
  }

  /*
   * loadGameObjects
   */
  void FirstApp::loadGameObjects() {
    std::shared_ptr<LveModel> lveModel = LveModel::createModelFromFile(lveDevice, "../models/flat_vase.obj");

    auto gameObj = LveGameObject::createGameObject();
    gameObj.model = lveModel;
    gameObj.transform.translation = {.0f, .5f, 2.5f};
    gameObj.transform.scale = glm::vec3(3.f);
    gameObjects.push_back(std::move(gameObj));
  }
}
