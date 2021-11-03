#include "../inc/lve_input_handler.hpp"

#include "../inc/lve_window.hpp"

namespace lve {
  Command* InputHandler::handleInput(GLFWwindow* window) {
    if (glfwGetKey(window, keyboardController.keys.moveForward) == GLFW_PRESS) {
      return buttonW;
    }
    if (glfwGetKey(window, keyboardController.keys.moveBackward) == GLFW_PRESS) {
      return buttonS;
    }
    if (glfwGetKey(window, keyboardController.keys.moveLeft) == GLFW_PRESS) {
      return buttonA;
    }
    if (glfwGetKey(window, keyboardController.keys.moveRight) == GLFW_PRESS) {
      return buttonD;
    }

    // else
    return NULL;
  }
}
