#pragma once

#include "lve_game_object.hpp"

namespace lve{
  class Command {
  public:
    virtual ~Command() {}
    virtual void execute(LveGameObject& gameObj) = 0;
  };

  class moveLeftCommand : Command {
  public:
    virtual void execute(LveGameObject& gameObj) {
      gameObj.moveLeft(1.f);
    }
  };
}
