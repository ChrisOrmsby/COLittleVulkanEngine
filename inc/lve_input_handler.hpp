#pragma once

#include "lve_command.hpp"
#include "keyboard_movement_controller.hpp"

namespace lve {
  class InputHandler {
  public:
    Command* handleInput(GLFWwindow* window);

  private:
    KeyboardMovementController keyboardController;

    Command* buttonW;
    Command* buttonS;
    Command* buttonA;
    Command* buttonD;
  };
}
