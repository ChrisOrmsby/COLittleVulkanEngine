#include "../inc/first_app.hpp"

// std
#include <stdexcept>
#include <array>

namespace lve {

  /*
   * Constructor
   */
  FirstApp::FirstApp() {
    loadModels();
    createPipelineLayout();
    createPipeline();
    createCommandBuffers();
  }

  FirstApp::~FirstApp() {
    vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
  }

  /*
   * loadModels
   */
  void FirstApp::loadModels() {
    std::vector<LveModel::Vertex> vertices {
      {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
      {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

    // std::vector<LveModel::Vertex> input {
    //   {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    //   {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    //   {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    // };

    // std::vector<LveModel::Vertex> vertices;
    // sierpinskiTriangle(input[0], input[1], input[2], 5, vertices);

    lveModel = std::make_unique<LveModel>(lveDevice, vertices);
  }

  /*
   * run
   */
  void FirstApp::run() {
    while (!lveWindow.shouldClose()) {
      glfwPollEvents();
      drawFrame();
    }

    vkDeviceWaitIdle(lveDevice.device());
  }

  /*
   * createPipelineLayout
   */
  void FirstApp::createPipelineLayout() {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to create pipeline layout.");
    }
  }

  /*
   * createPipeline
   */
  void FirstApp::createPipeline() {
    auto pipelineConfig = LvePipeline::defaultPipelineConfigInfo(lveSwapChain.width(), lveSwapChain.height());
    pipelineConfig.renderPass = lveSwapChain.getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    lvePipeline = std::make_unique<LvePipeline>(
                                                lveDevice,
                                                "../shaders/simple_shader.vert.spv",
                                                "../shaders/simple_shader.frag.spv",
                                                pipelineConfig);
  }

  /*
   * createCommandBuffers
   */
  void FirstApp::createCommandBuffers() {
    commandBuffers.resize(lveSwapChain.imageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = lveDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(lveDevice.device(), &allocInfo, commandBuffers.data()) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to allocate command buffers.");
    }

    for (int i = 0; i < commandBuffers.size(); i++) {
      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer.");
      }

      VkRenderPassBeginInfo renderPassInfo{};
      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassInfo.renderPass = lveSwapChain.getRenderPass();
      renderPassInfo.framebuffer = lveSwapChain.getFrameBuffer(i);

      renderPassInfo.renderArea.offset = {0, 0};
      renderPassInfo.renderArea.extent = lveSwapChain.getSwapChainExtent();

      std::array<VkClearValue, 2> clearValues{};
      clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
      clearValues[1].depthStencil = {1.0f, 0};
      renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
      renderPassInfo.pClearValues = clearValues.data();

      vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      lvePipeline->bind(commandBuffers[i]);
      lveModel->bind(commandBuffers[i]);
      lveModel->draw(commandBuffers[i]);

      vkCmdEndRenderPass(commandBuffers[i]);
      if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer.");
      }
    }
  }

  /*
   * drawFrame
   */
  void FirstApp::drawFrame() {
    uint32_t imageIndex;
    auto result = lveSwapChain.acquireNextImage(&imageIndex);

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      throw std::runtime_error("Failed to acquire next swap chain image.");
    }

    result = lveSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
    if (result != VK_SUCCESS) {
      throw std::runtime_error("Failed to present swap chain image.");
    }
  }

  /*
   * sierpinskiTriangle
   */
  void FirstApp::sierpinskiTriangle(LveModel::Vertex p1,
                                    LveModel::Vertex p2,
                                    LveModel::Vertex p3,
                                    int degree,
                                    std::vector<LveModel::Vertex> &vertices) {
    if (degree <= 0) {
      vertices.push_back(p1);
      vertices.push_back(p2);
      vertices.push_back(p3);
    }
    else {
      sierpinskiTriangle(p1, getMid(p1, p2), getMid(p1, p3), degree - 1, vertices);
      sierpinskiTriangle(p2, getMid(p2, p1), getMid(p2, p3), degree - 1, vertices);
      sierpinskiTriangle(p3, getMid(p3, p1), getMid(p3, p2), degree - 1, vertices);
    }
  }

  /*
   * getMid
   */
  LveModel::Vertex FirstApp::getMid(LveModel::Vertex p1, LveModel::Vertex p2) {
    float xMid = (p1.position[0] + p2.position[0]) / 2;
    float yMid = (p1.position[1] + p2.position[1]) / 2;

    return LveModel::Vertex {{xMid, yMid}};
  }
}
