#pragma once

namespace lve{
  class Command {
    virtual ~Command() {}
    virtual void execute() = 0;
  };

  class lookRightCommand : Command {
  public:
    virtual void execute() {

    }
  }
}
