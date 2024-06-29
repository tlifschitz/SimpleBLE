#pragma once

#include <simplebluez/Types.h>
#include <simpledbus/advanced/Interface.h>

#include <string>

namespace SimpleBluez {

class GattService1 : public SimpleDBus::Interface {
  public:
    GattService1(std::shared_ptr<SimpleDBus::Connection> conn, std::string path);
    virtual ~GattService1() = default;

    // ----- METHODS -----

    // ----- PROPERTIES -----
    BluetoothUUID UUID();

  protected:
    void property_changed(std::string option_name) override;

    BluetoothUUID _uuid;
};

}  // namespace SimpleBluez
