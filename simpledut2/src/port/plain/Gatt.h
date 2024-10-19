#include "GattBase.h"

class Gatt : public GattBase {
  public:
    std::string start(const std::string& name, int interval) override { return "starting gatt server"; }

    std::string stop() override { return "stopping gatt server"; }

    std::string status() override { return "gatt server status"; }

    std::string disconnect() override { return "disconnecting from device"; }
};