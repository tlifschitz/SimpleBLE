#pragma once

#include <gtest/gtest.h>
#include "simpleble/SimpleBLE.h"

/**
 * @class SimpleBLEHilTest
 * @brief A hardware in the loop test environment.
 *
 * This class provides a singleton instance to manage a hardware device
 * used for hardware in the loop testing.
 */
class SimpleBLEHilTest : public ::testing::Environment {
  public:
    static SimpleBLEHilTest& getInstance() {
        static SimpleBLEHilTest instance;
        return instance;
    }

    static SimpleBLE::Peripheral& getPeripheral() { return getInstance().simpledut; }

    ~SimpleBLEHilTest() override {}

    SimpleBLE::Peripheral simpledut;  ///< The SimpleBLE::Peripheral object used in tests.

    /**
     * @brief Set up the test environment.
     *
     * This function is called before the test suite is run. It checks if Bluetooth
     * is enabled, retrieves the list of adapters, and starts a scan to find the
     * peripheral with the identifier "esp32".
     */
    void SetUp() override {
        ASSERT_TRUE(SimpleBLE::Adapter::bluetooth_enabled());

        std::vector<SimpleBLE::Adapter> adapter_list = SimpleBLE::Adapter::get_adapters();

        ASSERT_FALSE(adapter_list.empty());

        SimpleBLE::Adapter adapter = adapter_list.at(0);

        bool found = false;

        adapter.set_callback_on_scan_found([&](SimpleBLE::Peripheral peripheral) {
            if (peripheral.identifier() == "esp32") {
                simpledut = peripheral;
                found = true;
            }
        });

        adapter.scan_for(5000);

        ASSERT_TRUE(found);
    }

    /**
     * @brief Tear down the test environment.
     *
     * This function is called after the test suite has run. It is currently commented out.
     */
    void TearDown() override {
        /*ASSERT_TRUE(simpledut.is_connected());
        std::cout << "Disconnecting from " << simpledut.identifier() << " [" << simpledut.address() << "]" << std::endl;
        simpledut.disconnect();
        ASSERT_FALSE(simpledut.is_connected());
        std::cout << "Disconnecting" << std::endl;*/
    }

  private:
    /**
     * @brief Private constructor to enforce singleton pattern.
     */
    SimpleBLEHilTest() {}
    SimpleBLEHilTest(const SimpleBLEHilTest&) = delete;
    SimpleBLEHilTest& operator=(const SimpleBLEHilTest&) = delete;
};
