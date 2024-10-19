#include <sstream>
#include <string>
#include "ProfileBase.h"

class GattBase : public ProfileBase {
  public:
    std::string name() override { return "gatt"; }

    std::string help() override { return "gatt <start/stop/status/disconnect>"; }

    std::string process(const std::string& params) override {
        if (params.empty()) {
            return help();
        }

        if (params.find("start") == 0) {
            std::string name;
            int interval;
            std::istringstream iss(params);
            iss >> name >> interval;
            return start(name, interval);
        } else if (params == "stop") {
            return stop();
        } else if (params == "status") {
            return status();
        } else if (params.find("disconnect") == 0) {
            return disconnect();
        } else if (params == "help") {
            return help();
        } else {
            return "Unknown command";
        }
    }

    virtual std::string start(const std::string& name, int interval) = 0;
    virtual std::string stop() = 0;
    virtual std::string status() = 0;
    virtual std::string disconnect() = 0;
};