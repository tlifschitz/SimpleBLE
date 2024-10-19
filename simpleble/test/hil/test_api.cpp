#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include "environment.hpp"
#include "simpleble/SimpleBLE.h"

TEST(SimpleBLEHilTest, Connect) {
    SimpleBLE::Peripheral& simpledut = SimpleBLEHilTest::getPeripheral();

    simpledut.set_callback_on_connected([]() { std::cout << "Connected callback triggered" << std::endl; });
    simpledut.set_callback_on_disconnected([]() { std::cout << "Disconnected callback triggered" << std::endl; });

    std::cout << "Connecting to " << simpledut.identifier() << " [" << simpledut.address() << "]" << std::endl;
    simpledut.connect();

    ASSERT_TRUE(simpledut.is_connected());
}

TEST(SimpleBLEHilTest, Read) {
    SimpleBLE::Peripheral& simpledut = SimpleBLEHilTest::getPeripheral();

    EXPECT_TRUE(simpledut.services().size() == 1);
    EXPECT_TRUE(simpledut.services()[0].uuid() == "937312e0-2354-11eb-9f10-fbc30a62cf38");
    SimpleBLE::Service service = simpledut.services()[0];

    for (auto service : simpledut.services()) {
        for (auto characteristic : service.characteristics()) {
            if (characteristic.uuid() == "937312e0-2354-11eb-9f10-fbc30a62cf38") {
                SimpleBLE::ByteArray rx_data = simpledut.read(service.uuid(), characteristic.uuid());
                EXPECT_TRUE(rx_data.size() == 17);
                EXPECT_TRUE(std::string{rx_data} == std::string{"Hello Bare-Metal "});
            }
        }
    }
}

TEST(SimpleBLEHilTest, Write) {
    SimpleBLE::Peripheral& simpledut = SimpleBLEHilTest::getPeripheral();

    SimpleBLE::ByteArray tx_data = "Characteristic write test";  // std::vector<uint8_t>{0x48, 0x65, 0x6c, 0x6c, 0x6f,
                                                                 // 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21};

    SimpleBLE::Service service = simpledut.services()[0];

    for (auto service : simpledut.services()) {
        for (auto characteristic : service.characteristics()) {
            if (characteristic.uuid() == "937312e0-2354-11eb-9f10-fbc30a62cf38") {
                simpledut.write_request(service.uuid(), characteristic.uuid(), tx_data);

                SimpleBLE::ByteArray rx_data = simpledut.read(service.uuid(), characteristic.uuid());
                EXPECT_TRUE(rx_data == tx_data) << "Actual: " << std::string{rx_data} << std::endl
                                                << "Expected: " << std::string{tx_data};
            }
        }
    }
}

TEST(SimpleBLEHilTest, Notify) {
    SimpleBLE::Peripheral& simpledut = SimpleBLEHilTest::getPeripheral();

    SimpleBLE::ByteArray tx_data = std::vector<uint8_t>{0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20,
                                                        0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21};

    for (auto service : simpledut.services()) {
        for (auto characteristic : service.characteristics()) {
            if (characteristic.uuid() == "987312e0-2354-11eb-9f10-fbc30a62cf38") {
                bool notification_received = false;
                simpledut.notify(service.uuid(), characteristic.uuid(), [&](SimpleBLE::ByteArray bytes) {
                    std::cout << "Notification: " << bytes << std::endl;
                    notification_received = true;
                });

                simpledut.write_request(service.uuid(), characteristic.uuid(), tx_data);
                std::this_thread::sleep_for(std::chrono::seconds(5));

                simpledut.unsubscribe(service.uuid(), characteristic.uuid());
                simpledut.write_request(service.uuid(), characteristic.uuid(), tx_data);

                EXPECT_TRUE(notification_received) << "Notification not received";
            }
        }
    }
}