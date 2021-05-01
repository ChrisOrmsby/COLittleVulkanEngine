#pragma once

#include "lve_device.hpp"
#include "lve_pipeline.hpp"
#include "lve_swap_chain.hpp"
#include "lve_window.hpp"
#include "lve_model.hpp"

// std
#include <memory>
#include <vector>

namespace lve {
  class FirstApp {
  public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    FirstApp();
    ~FirstApp();

    FirstApp(const FirstApp &) = delete;
    FirstApp &operator=(const FirstApp &) = delete;

    void run();

  private:
    void loadModels();
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void drawFrame();

    // Creates a model representing a Sierpinski Triangle
    void sierpinskiTriangle(LveModel::Vertex p1,
                            LveModel::Vertex p2,
                            LveModel::Vertex p3,
                            int degree,
                            std::vector<LveModel::Vertex> &vertices);

    // Helper method for sierspinkiTiriangle method
    LveModel::Vertex getMid(LveModel::Vertex p1, LveModel::Vertex p2);

    LveWindow lveWindow{WIDTH, HEIGHT, "Test"};
    LveDevice lveDevice{lveWindow};
    LveSwapChain lveSwapChain{lveDevice, lveWindow.getExtent()};
    std::unique_ptr<LvePipeline> lvePipeline;
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;
    std::unique_ptr<LveModel> lveModel;
  };
}
