#include <iostream>
#include "SimpleDutBase.h"

class SimpleDut : public SimpleDutBase {
  public:
    void getCommand(std::string& command) override { std::getline(std::cin, command); }
    void output(const std::string& message) override {
        std::cout << message;
        if (!message.empty() && message.back() != '\n') {
            std::cout << std::endl;
        }
        std::cout << "> ";
    }
};